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

using std::ifstream;
using std::string;

namespace main_service {
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

    // TODO, check if the requested resource is whitelisted
    LOG(DEBUG) << "Request PATH: " << cntl->http_request().uri().path();
    std::map<std::string,ServeableContentNode>::iterator read_node = _whitelisted_content.find(cntl->http_request().uri().path());
    if (_whitelisted_content.end() != read_node) {
        // TODO lazy loading if the content is not loaded
        cntl->http_response().set_content_type(read_node->second._filetype);
        cntl->response_attachment().append(read_node->second._file_content.c_str(),
                                           read_node->second._file_content.size());
        cntl->http_response().set_status_code(200);
    } else {
        cntl->http_response().set_status_code(404);
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

}

}
