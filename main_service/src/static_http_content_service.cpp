/*
 * Copyright 2019 Väinö Lappi
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under
 * the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Service for static html/js/other content from file system
  */
#include <iostream>
#include <fstream>
#include <utility>
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/restful.h>
#include "static_http_content_service.hpp"
#include "djap_utils/include/mutex_guard.hpp"

DEFINE_string(static_content_root,
              "/home/djap/player_project/UCSRMediaPlayer/main_service/UCSR_main_service/static_web_content",
              "Path for searching files for static content");
DEFINE_string(static_content_allow_files,
              "/home/djap/player_project/UCSRMediaPlayer/main_service/UCSR_main_service/conf/static_content_whitelist.txt",
              "A list of files accessible from this interface."\
              " If a requested subpath is not on this list, 404 is returned even if the requested file exists and would be accessible by server process.");
DEFINE_bool(lazy_load_white_list, false,
            "If set to false, all static content is loaded to memory on start up."\
            " If set to true, static content is loaded to memory only as it's needed.");
DEFINE_bool(require_login, true,
            "Are users required to login or is this public available to everyone."
            " NOTE: don't forget to enable HTTPS if this is needed!");
DEFINE_string(login_page_path, "/static/login.html", "Path to login page");
DEFINE_string(error_page_list,
              "/home/djap/player_project/UCSRMediaPlayer/main_service/UCSR_main_service/conf/error_pages.txt",
              "List of custom error pages for different http error codes");

using std::ifstream;
using std::string;

namespace main_service {

const static std::string LOGIN_COOKIE_NAME("UCSRMediaPlayerSession");

void StaticHttpContentServiceImpl::PageRequest(
          google::protobuf::RpcController* cntl_base,
          const HttpRequest*,
          HttpResponse*,
          google::protobuf::Closure* done) {
    // This object helps you to call done->Run() in RAII style. If you need
    // to process the request asynchronously, pass done_guard.release().
    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);
    // Fill response.
    _whitelisted_content_lock.read_lock();

    // Check if the requested resource is whitelisted
    LOG(DEBUG) << "Request PATH: " << cntl->http_request().uri().path();
    std::map<std::string,ServeableContentNode>::iterator read_node = _whitelisted_content.find(cntl->http_request().uri().path());
    if (_whitelisted_content.end() != read_node) {
        // Have we login or is the user requesting login page/posting login credentials?
        if (false == check_login(cntl_base, FLAGS_login_page_path)) {
            _whitelisted_content_lock.read_unlock();
            return;
        }

        // Is the user posting login credentials
        // TODO, get rid of the hard coded value?
        if (true == do_login(cntl_base, "/static/index.html", FLAGS_login_page_path)) {
            _whitelisted_content_lock.read_unlock();
            return;
        }
        // TODO lazy loading if the content is not loaded
        cntl->http_response().set_content_type(read_node->second._filetype);
        cntl->response_attachment().append(read_node->second._file_content.c_str(),
                                           read_node->second._file_content.size());
        cntl->http_response().set_status_code(200);
    } else {
        // TODO 404 page
        cntl->http_response().set_status_code(404);
        std::map <int, std::string>::iterator error_content = _error_pages.find(404);
        if (error_content != _error_pages.end()) {
            cntl->http_response().set_content_type(string("text/html"));
            cntl->response_attachment().append(error_content->second.c_str(),
                                               error_content->second.size());
        }
    }

    _whitelisted_content_lock.read_unlock();
}

void StaticHttpContentServiceImpl::load_static_content_whitelist() {
    ifstream whitelist(FLAGS_static_content_allow_files);
    if (true == whitelist.is_open()) {
        string whitelist_entry;
        while (getline(whitelist, whitelist_entry)) {
            ServeableContentNode add_node;
            string file_path;
            size_t index = whitelist_entry.find(",");
            if (index != string::npos) {
                file_path = whitelist_entry.substr(0, index);
                add_node._filetype = whitelist_entry.substr(index+1, string::npos);
            } else if (whitelist_entry.length() > 0) {
                file_path = whitelist_entry;    // no type specified
                add_node._filetype = string("text/plain");
            } else {
                continue;
            }
            std::map<std::string,ServeableContentNode>::iterator added_node =
                    _whitelisted_content.insert(make_pair(string("/static/") + file_path, add_node)).first;
            if (false == FLAGS_lazy_load_white_list) {
                ifstream file_content(FLAGS_static_content_root + string("/") + file_path);
                if (true == file_content.is_open()) {
                    added_node->second._file_content = std::string((std::istreambuf_iterator<char>(file_content)),
                                                                   std::istreambuf_iterator<char>());
                    added_node->second._loaded = true;
                    LOG(DEBUG) << "File content: " << added_node->second._file_content;
                } else {
                    LOG(WARNING) << "Unable to open static content file file from " << FLAGS_static_content_root <<  string("/") << file_path;
                    continue;
                }
            }
        }
    } else {
        LOG(WARNING) << "Unable to open static content whitelist file from " << FLAGS_lazy_load_white_list;
    }

    // load error pages
    ifstream errorlist(FLAGS_error_page_list);
    if (true == errorlist.is_open()) {
        string errorlist_entry;
        while (getline(errorlist, errorlist_entry)) {
            string file_path;
            int code;
            size_t index = errorlist_entry.find(",");
            if (index != string::npos) {
                code = static_cast<int>(strtol(errorlist_entry.substr(0, index).c_str(), nullptr, 10));
                file_path = errorlist_entry.substr(index+1, string::npos);
                if (code <= 0) {
                    continue;
                }
            } else {
                continue;
            }
            ifstream file_content(FLAGS_static_content_root + string("/") + file_path);
            if (true == file_content.is_open()) {
                _error_pages[code] = std::string((std::istreambuf_iterator<char>(file_content)),
                                                 std::istreambuf_iterator<char>());
            } else {
                LOG(WARNING) << "Unable to open static content file file from " << FLAGS_static_content_root <<  string("/") << file_path;
            }
        }
    } else {
        LOG(WARNING) << "Unable to open static content whitelist file from " << FLAGS_lazy_load_white_list;
    }

}

bool StaticHttpContentServiceImpl::check_login(google::protobuf::RpcController* cntl_base,
                                               const std::string& redirect_login_page)
{
    bool ret = false;   // Not logged in
    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);
    if ((cntl->http_request().uri().path() != FLAGS_login_page_path)
            && (true == FLAGS_require_login)) {
        // Check the cookie.
        brpc::HttpHeader::HeaderIterator it = cntl->http_request().HeaderBegin();
        while (it != cntl->http_request().HeaderEnd()) {
            LOG(DEBUG) << "Header: " << it->first << " = " << it->second;
            if (it->first == std::string("Cookie")) {
                std::map<std::string, std::string> cookies;
                parse_cookie_jar(cookies, it->second);
                for (std::map<std::string, std::string>::iterator cookie_it = cookies.begin();
                     cookie_it != cookies.end() && false == ret;
                     ++cookie_it) {
                    LOG(DEBUG) << "CName: " << cookie_it->first << " CVal: " << cookie_it->second;
                    if (cookie_it->first == LOGIN_COOKIE_NAME) {
                        // There is a login cookie
                        DjapUtils::MutexGuard guard(_logged_in_users_lock);
                        std::map<std::string, LoggedInUserNode>::iterator login_it = _logged_in_users.find(cookie_it->second);
                        if (login_it != _logged_in_users.end()) {
                            // I have an entry with the cookie
                            // Check expired
                            if (login_it->second.has_expired(true)) {
                                _logged_in_users.erase(login_it);
                                ret = false;
                            } else {
                                ret = true; // User is logged in
                            }
                        }
                        break;
                    }
                }
            }
            ++it;
        }
        if (false == ret) {
            // Send redirect response to login page if not set and return false.
            do_redirect(cntl_base, redirect_login_page);
        }
    } else {
        // Just return true if entering login page or no login required
        ret = true;
    }
    return ret;
}

bool StaticHttpContentServiceImpl::do_login(google::protobuf::RpcController* cntl_base,
                                            const std::string& logged_in_page,
                                            const std::string& redirect_login_page)
{
    bool ret = false;
    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);
    if (cntl->http_request().uri().path() == FLAGS_login_page_path) {
        // Check if we have login parameters, return false if not
        if (brpc::HttpMethod::HTTP_METHOD_POST != cntl->http_request().method()) {
            // was expecting credentials as POST
            LOG(DEBUG) << "NOT POST! Method was " << cntl->http_request().method();
            ret = false;
        } else {
            // May have something posted here, check it
            LOG(DEBUG) << "Attachement size: " << cntl->request_attachment().size();
            LOG(DEBUG) << "Attachement: " << cntl->request_attachment().to_string();
            // TODO parse credentials
            std::string user_name;
            std::string password;
            // TODO Check the login parameters
            if (true == check_login_credentials(user_name, password)) {
                // Set and store cookie if right (don't set cookie if wrong, just redirect back to login page)
                std::string login_cookie_value = generate_login_cookie_hash();
                cntl->http_response().SetHeader(std::string("Set-Cookie"), LOGIN_COOKIE_NAME + std::string("=") + login_cookie_value + std::string("; Max-Age=300"));
                _logged_in_users[login_cookie_value] = LoggedInUserNode(std::string("djap"), std::string("123"), 300);
                // Send redirect response to index
                do_redirect(cntl_base, logged_in_page);
            } else {
                // Send redirect response to login page
                do_redirect(cntl_base, redirect_login_page);
            }
            ret = true;
        }
    }
    return ret;
}

bool StaticHttpContentServiceImpl::check_login_credentials(
        const std::string& user_name,
        const std::string& password)
{
    // TODO
    return true;
}

void StaticHttpContentServiceImpl::do_redirect(google::protobuf::RpcController* cntl_base, const std::string& redirect_resource) {
    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);
    cntl->http_response().set_status_code(307);
    cntl->http_response().SetHeader("Location", redirect_resource);
}

void StaticHttpContentServiceImpl::parse_cookie_jar(
        std::map<std::string, std::string>& cookies,
        const std::string& cookie_jar)
{
    std::string cookie_delimiter("; ");
    size_t begin_index = 0;
    while (begin_index < cookie_jar.size()) {
        size_t end = cookie_jar.find(cookie_delimiter, begin_index);
        if (end != std::string::npos) {
            end -= begin_index;	// change to length
        }
        std::string cookie = cookie_jar.substr(begin_index, end);
        size_t cookie_name_size = cookie.find("=");
        std::string cookie_name = cookie.substr(0, cookie_name_size);
        std::string cookie_value;
        if (cookie_name_size != std::string::npos) {
            cookie_value = cookie.substr(cookie_name_size+1, std::string::npos);
        }
        cookies[cookie_name] = cookie_value;
        if (end != std::string::npos) {
            begin_index += end + cookie_delimiter.size();
        } else {
            break;
        }
    }
}

std::string StaticHttpContentServiceImpl::generate_login_cookie_hash()
{
    // TODO generate a random hash
    return std::string("ABCDEFG");
}

}
