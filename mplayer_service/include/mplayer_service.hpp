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
#ifndef _MPLAYER_SERVICE_MPLAYER_SERVICE_HPP_
#define _MPLAYER_SERVICE_MPLAYER_SERVICE_HPP_

#include "player_service.pb.h"

namespace player_service {

class MplayerService : public PlayerService {
public:
    MplayerService() {};
    virtual ~MplayerService() {};
    void PlayerProperties(::google::protobuf::RpcController* controller,
                       const ::player_service::PlayerPropertiesRequest* request,
                       ::player_service::PlayerPropertiesResponse* response,
                       ::google::protobuf::Closure* done);
    void ResetPlaylist(::google::protobuf::RpcController* controller,
                       const ::player_service::ResetRequest* request,
                       ::player_service::ResetResponse* response,
                       ::google::protobuf::Closure* done);
    void PushToPlaylist(::google::protobuf::RpcController* controller,
                       const ::player_service::PushToPlaylistRequest* request,
                       ::player_service::PushToPlaylistResponse* response,
                       ::google::protobuf::Closure* done);
    void PlaybackControl(::google::protobuf::RpcController* controller,
                       const ::player_service::PlaybackControlRequest* request,
                       ::player_service::PlaybackControlResponse* response,
                       ::google::protobuf::Closure* done);
};

}

#endif // _MPLAYER_SERVICE_MPLAYER_SERVICE_HPP_
