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

namespace main_service {
class ServeableContentNode {
public:
    ServeableContentNode() : _loaded(false) {}
    bool _loaded;
    std::string _filetype;
    std::string _file_content;
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

private:
    std::map<std::string,ServeableContentNode> _whitelisted_content;
    DjapUtils::ReadWriteLock _whitelisted_content_lock;
};
} // namespace main_service

#endif // #ifndef _MAIN_SERVICE_STATIC_HTTP_CONTENT_SERVICE_HPP_
