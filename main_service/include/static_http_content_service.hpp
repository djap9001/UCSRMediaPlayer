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
#ifndef _MAIN_SERVICE_STATIC_HTTP_CONTENT_SERVICE_HPP_
#define _MAIN_SERVICE_STATIC_HTTP_CONTENT_SERVICE_HPP_
#include "main_service.pb.h"
#include "djap_utils/include/read_write_lock.hpp"
#include "djap_utils/include/mutex.hpp"

namespace main_service {
class ServeableContentNode {
public:
    ServeableContentNode() : _loaded(false) {}
    bool _loaded;
    std::string _filetype;
    std::string _file_content;
};

class LoggedInUserNode {
public:
    LoggedInUserNode(const std::string& user_name,
                     const std::string& user_id,
                     int expire_in_seconds) :
        _user_name(user_name),
        _user_id(user_id),
        _expire_period(expire_in_seconds)
    {
        if (_expire_period > 0) {
            _expire_time = time(nullptr) + _expire_period;
            if (_expire_time == 0) {
                _expire_time++; // quite unexpected
            }
        } else {
            _expire_time = 0;
        }
    }
    LoggedInUserNode() :
        _expire_time(1),
        _expire_period(1) {

    }
    const std::string& user_name() {
        return _user_name;
    }

    const std::string& user_id() {
        return _user_id;
    }

    bool has_expired(bool extend) {
        if (_expire_time == 0) {
            return false;   // never expire
        }
        time_t current_time = time(nullptr);
        if ((current_time > _expire_time) // expired
                || (current_time < (_expire_time - _expire_period)))  // some odd noncense with the clock
        {
            return true;
        }
        if (true == extend) {
            _expire_time = current_time + _expire_period;
        }
        return false;
    }
private:
    std::string _user_name;
    std::string _user_id;
    time_t _expire_time;
    int _expire_period;
};

class StaticHttpContentServiceImpl : public StaticHttpContentService {
public:
    StaticHttpContentServiceImpl() {}
    virtual ~StaticHttpContentServiceImpl() {}
    void PageRequest(google::protobuf::RpcController* cntl_base,
              const HttpRequest*,
              HttpResponse*,
              google::protobuf::Closure* done);
    void load_static_content_whitelist();
private:
    bool check_login(
            google::protobuf::RpcController* cntl_base,
            const std::string& redirect_login_page);
    bool do_login(
            google::protobuf::RpcController* cntl_base,
            const std::string& logged_in_page,
            const std::string& redirect_login_page);
    bool check_login_credentials(
            const std::string& user_name,
            const std::string& password);
    void do_redirect(
            google::protobuf::RpcController* cntl_base,
            const std::string& redirect_resource);
    void parse_cookie_jar(
            std::map<std::string, std::string>& cookies,
            const std::string& cookie_jar);

    std::string generate_login_cookie_hash();
private:
    std::map<std::string,ServeableContentNode> _whitelisted_content;
    DjapUtils::ReadWriteLock _whitelisted_content_lock;
    std::map <int, std::string> _error_pages;

    DjapUtils::Mutex _logged_in_users_lock;
    std::map<std::string, LoggedInUserNode> _logged_in_users;
};
} // namespace main_service

#endif // #ifndef _MAIN_SERVICE_STATIC_HTTP_CONTENT_SERVICE_HPP_
