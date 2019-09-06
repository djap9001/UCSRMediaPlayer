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
  * support for controlling mplayer
  */
#include <unistd.h>
#include "mplayer_controller.hpp"

namespace player_service {

MPlayerController::MPlayerController() {

}

MPlayerController::~MPlayerController() {
}

void MPlayerController::init_mplayer_process() {

}

int MPlayerController::set_file(const std::string& file_path) {

}

int MPlayerController::play() {

}

int MPlayerController::pause() {

}

int MPlayerController::set_volume(int volume) {

}

int MPlayerController::stop() {

}

void MPlayerController::seek(int pos) {

}

}

