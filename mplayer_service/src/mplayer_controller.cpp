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
    init_mplayer_process();
}

MPlayerController::~MPlayerController() {
}

void MPlayerController::init_mplayer_process() {
    int controller_to_mplayer[2];
    int mplayer_to_controller[2];

    pipe(controller_to_mplayer);
    pipe(mplayer_to_controller);

    int mplayerPID = fork();

    if (mplayerPID == 0) {
        //this is child, make it mplayer...
        close(controller_to_mplayer[1]);    //Close the writing end of the incoming pipe
        close(mplayer_to_controller[0]);    //Close the reading end of the outgoing pipe

        dup2(controller_to_mplayer[0], STDIN_FILENO);   //replace stdin with incoming pipe
        dup2(mplayer_to_controller[1], STDOUT_FILENO);  //replace stdout with outgoing pipe

        //exec mplayer process
        char filename[] = "/usr/bin/mplayer";
        char *newargv[] = {"mplayer", "-slave", "-idle", "-quiet", NULL};
        char *newenviron[] = { NULL };
        execve(filename, newargv, newenviron);
    } else {
        //this is parent, keep as mplayer control server...
        close(mplayer_to_controller[0]);    //Close the reading end of the outgoing pipe.
        close(controller_to_mplayer[1]);    //Close the writing side of the incoming pipe.

        // store the remaining file descriptors for later use.
        _mplayer_process_write_fd = mplayer_to_controller[1];
        _mplayer_process_read_fd = controller_to_mplayer[0];
    }
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

