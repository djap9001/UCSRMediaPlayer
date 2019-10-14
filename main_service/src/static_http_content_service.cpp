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
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <brpc/server.h>
#include <brpc/restful.h>
#include "static_http_content_service.hpp"

DEFINE_string(static_content_root,
              "/home/djap/ucsr_player_test/static",
              "Path for searching files for static content");
DEFINE_string(static_content_allow_files,
              "/home/djap/ucsr_player_test/static/whitelist.txt",
              "A list of files accessible from this interface."\
              " If a requested subpath is not on this list, 404 is returned even if the requested file exists and would be accessible by server process.");


namespace main_service {
void StaticHttpContentServiceImpl::PageRequest(google::protobuf::RpcController* cntl_base,
          const HttpRequest*,
          HttpResponse*,
          google::protobuf::Closure* done) {
    // This object helps you to call done->Run() in RAII style. If you need
    // to process the request asynchronously, pass done_guard.release().
    brpc::ClosureGuard done_guard(done);

    brpc::Controller* cntl =
        static_cast<brpc::Controller*>(cntl_base);
    // Fill response.
    cntl->http_response().set_content_type("text/plain");
    butil::IOBufBuilder os;
    os << "queries:";
    for (brpc::URI::QueryIterator it = cntl->http_request().uri().QueryBegin();
            it != cntl->http_request().uri().QueryEnd(); ++it) {
        os << ' ' << it->first << '=' << it->second;
    }
    os << "\nbody: " << cntl->request_attachment() << '\n';
    os.move_to(cntl->response_attachment());
}
}
