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
  * Implementation for mplayer service for mplayer module of UCSR player.
  */

#include <butil/logging.h>
#include <brpc/server.h>
#include "mplayer_service.hpp"

namespace player_service {

void MplayerService::PlayerProperties(::google::protobuf::RpcController* controller,
                       const ::player_service::PlayerPropertiesRequest* request,
                       ::player_service::PlayerPropertiesResponse* response,
                       ::google::protobuf::Closure* done)
{
    brpc::ClosureGuard done_guard(done);
    // Fill response.
    // TODO this might be better to get from config
    response->add_supported_file_types(std::string("mp3"));
    response->add_supported_file_types(std::string("mp4"));
    response->add_supported_file_types(std::string("wmv"));
    response->add_supported_file_types(std::string("avi"));
    response->add_supported_file_types(std::string("mpg"));
    response->add_supported_file_types(std::string("mpeg"));
    response->add_supported_file_types(std::string("ogg"));
    response->add_supported_file_types(std::string("wav"));
}

void MplayerService::ResetPlaylist(::google::protobuf::RpcController* controller,
                       const ::player_service::ResetRequest* request,
                       ::player_service::ResetResponse* response,
                       ::google::protobuf::Closure* done)
{
    brpc::ClosureGuard done_guard(done);
    // Fill response.
    response->set_error_code(0);
    response->set_error_message(std::string("OK"));
}

void MplayerService::PushToPlaylist(::google::protobuf::RpcController* controller,
                       const ::player_service::PushToPlaylistRequest* request,
                       ::player_service::PushToPlaylistResponse* response,
                       ::google::protobuf::Closure* done)
{
    brpc::ClosureGuard done_guard(done);
    // Fill response.
    response->set_error_code(0);
    response->set_error_message(std::string("OK"));
    response->set_playlist_index(0);
}

void MplayerService::PlaybackControl(::google::protobuf::RpcController* controller,
                       const ::player_service::PlaybackControlRequest* request,
                       ::player_service::PlaybackControlResponse* response,
                       ::google::protobuf::Closure* done)
{
    brpc::ClosureGuard done_guard(done);
    // Fill response.
    response->set_error_code(0);
    response->set_error_message(std::string("OK"));
}

} // namespace player_service
