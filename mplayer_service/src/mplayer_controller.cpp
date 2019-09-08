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
#include <fcntl.h>
#include <sys/epoll.h>
#include "mplayer_controller.hpp"
#include <bthread/unstable.h>   // bthread_fd_timedwait()
#include <bthread/bthread.h>

namespace player_service {

void MplayerReader::set_read_fd(int fd) {
    _read_fd = fd;
}

void MplayerReader::stop() {
    _stop_requested = true;
    this->join();
}

MplayerReader::MplayerReader() {
    _stop_requested = false;
}

MplayerReader::~MplayerReader() {

}

void MplayerReader::thread_main() {
    char read_buf[1024];
    while (false == _stop_requested) {
        timespec t = butil::milliseconds_from_now(3000);
        int ret = bthread_fd_timedwait(_read_fd, EPOLLIN, &t);
        LOG(DEBUG) << "bthread_fd_timedwait return " << ret;
        if (ret >= 0) {
            //  Has something to read
            size_t got_bytes_count = read(_read_fd, read_buf, 1024);
            if  (got_bytes_count > 0) {
                LOG(DEBUG) << "Got " << got_bytes_count << " bytes from read fd";
                LOG(DEBUG) << "Mplayer said: [" << read_buf << "]";
            }
        }
    }
}

MPlayerController::MPlayerController() {
    LOG(DEBUG) << "Alloc mplayer controller!";
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
        // TODO a proper environment
        //char *newenviron[] = { NULL };
        execvp(filename, newargv);
        //execve(filename, newargv, newenviron);
    } else {
        LOG(DEBUG) << "MPlayer PID: " << mplayerPID;
        //this is parent, keep as mplayer control server...
        close(controller_to_mplayer[0]);    //Close the reading end of the outgoing pipe.
        close(mplayer_to_controller[1]);    //Close the writing side of the incoming pipe.

        // store the remaining file descriptors for later use.
        _mplayer_process_write_fd = controller_to_mplayer[1];
        _mplayer_process_read_fd = mplayer_to_controller[0];
        // make both non_blocking
        /*
        fcntl(_mplayer_process_write_fd,
              F_SETFL,
              (fcntl(_mplayer_process_write_fd, F_GETFL) | O_NONBLOCK));
        fcntl(_mplayer_process_read_fd,
              F_SETFL,
              (fcntl(_mplayer_process_read_fd, F_GETFL) | O_NONBLOCK));
        */
        _reader = MplayerReader::alloc<MplayerReader>();
        _reader->set_read_fd(_mplayer_process_read_fd);
        _reader->start();
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
    std::string command = std::string("loadfile ") + std::string("/home/djap/palvoja.mp3") + std::string("\n");
    LOG(DEBUG) << "Write to mplayer return: "
               << write(_mplayer_process_write_fd, command.c_str(), command.size());
    LOG(DEBUG) << "Sleeping...";
    bthread_usleep(1000*2000);
    LOG(DEBUG) << "Sleeped!";
}

void MPlayerController::seek(int pos) {

}

}

