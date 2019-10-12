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
#include"djap_utils/include/mutex_guard.hpp"
#include <bthread/unstable.h>   // bthread_fd_timedwait()
#include <bthread/bthread.h>
#include <gflags/gflags.h>

DEFINE_int32(mplayer_progress_update_interval_ms,
            500,
             "Interval for asking playback progress updates from mplayer");

namespace player_service {

//[Playing /home/djap/palvoja.mp3.
const std::string g_playing_event_identifier("Playing ");
const std::string g_player_version_event_identifier(" MPlayer ");
const std::string g_length_event_identifier("ANS_length");
const std::string g_v_ratio_event_identifier("ANS_aspect");
const std::string g_v_width_event_identifier("ANS_width");
const std::string g_v_height_event_identifier("ANS_height");
const std::string g_file_name_event_identifier("ANS_filename");
const std::string g_play_pos_event_identifier("ANS_time_pos=");
const std::string g_track_title_event_identifier(" Title: ");
const std::string g_track_title2_event_identifier(" title: ");
const std::string g_track_name_event_identifier(" Name: ");
const std::string g_track_artist_event_identifier(" Artist: ");
const std::string g_track_author_event_identifier(" author: ");
const std::string g_track_create_date_event_identifier("Creation");
const std::string g_track_year_event_identifier(" Year:");
const std::string g_track_album_event_identifier(" Album:");

void MplayerReader::set_read_fd(int fd) {
    _read_fd = fd;
}

void MplayerReader::set_delegate(DjapUtils::WeakPointer<MplayerControllerDelegate> delegate) {
    DjapUtils::MutexGuard lock(_delegate_lock);
    _delegate = delegate;
}

void MplayerReader::stop() {
    _stop_requested = true;
    join();
}

MplayerReader::MplayerReader() {
    _stop_requested = false;
}

MplayerReader::~MplayerReader() {

}

inline int first_line_delimiter_index(const char* buffer) {
    int ret = -1;
    int idx = 0;
    while (*buffer != '\0') {
        if ((*buffer == '\r') || (*buffer == '\n')) {
            ret = idx;
            break;
        }
        buffer++;
        idx++;
    }
    return ret;
}

void MplayerReader::thread_main() {
    char read_buf[1024];
    std::string event_buffer;
    while (false == _stop_requested) {
        timespec t = butil::milliseconds_from_now(3000);
        int ret = bthread_fd_timedwait(_read_fd, EPOLLIN, &t);
        LOG(DEBUG) << "bthread_fd_timedwait return " << ret;
        if (ret >= 0) {
            //  Has something to read
            ssize_t got_bytes_count = read(_read_fd, read_buf, 1024);
            if  (got_bytes_count > 0) {
                event_buffer.append(read_buf, got_bytes_count);
                int line_end_idx = 0;
                while (0 <= line_end_idx) {
                    line_end_idx = first_line_delimiter_index(event_buffer.c_str());
                    if (line_end_idx > 0) {
                        std::string event_line(event_buffer.c_str(), line_end_idx);
                        process_event(event_line);
                    }
                    if (line_end_idx >= 0) {
                        event_buffer.erase(0, (line_end_idx + 1));
                    }
                }
            }
        }
    }
}

void MplayerReader::process_event(const std::string& event_buffer) {
    LOG(DEBUG) << "Mplayer event: [" << event_buffer << "]";
    if (0 == event_buffer.find(g_playing_event_identifier)) {
        LOG(DEBUG) << "begin playback event";
        DjapUtils::MutexGuard lock(_delegate_lock);
        DjapUtils::SharedPointer<MplayerControllerDelegate> cb_delegate = _delegate;
        if (nullptr != cb_delegate.raw_ptr()) {
            cb_delegate->did_start(
                        event_buffer.substr(
                            g_playing_event_identifier.size(),
                            (event_buffer.size() - g_playing_event_identifier.size())
                            )
                        );
        }
    }
    /*
    if (0 == event_buffer.find(g_player_version_event_identifier)) {

    }
    */
    if (0 == event_buffer.find(g_length_event_identifier)) {

    }
    /*
    if (0 == event_buffer.find(g_v_ratio_event_identifier)) {

    }
    if (0 == event_buffer.find(g_v_width_event_identifier)) {

    }
    if (0 == event_buffer.find(g_v_height_event_identifier)) {

    }
    if (0 == event_buffer.find(g_file_name_event_identifier)) {

    }
    */
    if (0 == event_buffer.find(g_play_pos_event_identifier)) {
        DjapUtils::MutexGuard lock(_delegate_lock);
        DjapUtils::SharedPointer<MplayerControllerDelegate> cb_delegate = _delegate;
        if (nullptr != cb_delegate.raw_ptr()) {
            std::string progress_str = event_buffer.substr(
                        g_play_pos_event_identifier.size(),
                        (event_buffer.size() - g_play_pos_event_identifier.size())
                        );
            double progress_second = strtod(progress_str.c_str(), nullptr);
            cb_delegate->did_get_info_track_progress(static_cast<uint64_t>(progress_second * 1000));
        }
    }
    if (0 == event_buffer.find(g_track_title_event_identifier)) {

    }
    if (0 == event_buffer.find(g_track_title2_event_identifier)) {

    }
    if (0 == event_buffer.find(g_track_name_event_identifier)) {
        DjapUtils::MutexGuard lock(_delegate_lock);
        DjapUtils::SharedPointer<MplayerControllerDelegate> cb_delegate = _delegate;
        if (nullptr != cb_delegate.raw_ptr()) {
            cb_delegate->did_get_info_track_name(
                        event_buffer.substr(
                            g_track_name_event_identifier.size(),
                            (event_buffer.size() - g_track_name_event_identifier.size())
                            )
                        );
        }
    }
    if (0 == event_buffer.find(g_track_artist_event_identifier)) {
        DjapUtils::MutexGuard lock(_delegate_lock);
        DjapUtils::SharedPointer<MplayerControllerDelegate> cb_delegate = _delegate;
        if (nullptr != cb_delegate.raw_ptr()) {
            cb_delegate->did_get_info_track_artist(
                        event_buffer.substr(
                            g_track_artist_event_identifier.size(),
                            (event_buffer.size() - g_track_artist_event_identifier.size())
                            )
                        );
        }
    }
    /*
    if (0 == event_buffer.find(g_track_author_event_identifier)) {

    }
    if (0 == event_buffer.find(g_track_create_date_event_identifier)) {

    }
    if (0 == event_buffer.find(g_track_year_event_identifier)) {

    }
    */
    if (0 == event_buffer.find(g_track_album_event_identifier)) {
        DjapUtils::MutexGuard lock(_delegate_lock);
        DjapUtils::SharedPointer<MplayerControllerDelegate> cb_delegate = _delegate;
        if (nullptr != cb_delegate.raw_ptr()) {
            cb_delegate->did_get_info_track_album(
                        event_buffer.substr(
                            g_track_album_event_identifier.size(),
                            (event_buffer.size() - g_track_album_event_identifier.size())
                            )
                        );
        }
    }
}

const int MplayerWriter::WRITER_COMMAND_TYPE_STOP = 0;
const int MplayerWriter::WRITER_COMMAND_TYPE_WRITE_TO_MPLAYER = 1;

void MplayerWriter::set_write_fd(int fd) {
    _write_fd = fd;
}

void MplayerWriter::write_command(const std::string& command) {
    push_message(WRITER_COMMAND_TYPE_WRITE_TO_MPLAYER, command);
}

void MplayerWriter::stop() {
    push_message(WRITER_COMMAND_TYPE_STOP, std::string(""));
    join();
}

MplayerWriter::MplayerWriter() {
    _write_fd = -1;
}

MplayerWriter::~MplayerWriter() {

}

void MplayerWriter::thread_main() {
    while (true) {
        std::pair<int,std::string> command = pop_message();
        if (command.first == WRITER_COMMAND_TYPE_WRITE_TO_MPLAYER) {
            // TODO do the non-blocking write and check for success (EWOULDBLOCK or something like that).
            int res = write(_write_fd, command.second.c_str(), command.second.size());
        } else if (command.first == WRITER_COMMAND_TYPE_STOP) {
            break;
        }
    }
}

void MplayerWriter::push_message(int command_type, const std::string& command_param) {
    _message_queue_mutex.lock();
    _message_queue.push_back(std::make_pair(command_type, command_param));
    _message_queue_mutex.unlock();
    _message_queue_mutex.Signal();
}

std::pair<int,std::string> MplayerWriter::pop_message() {
    std::pair<int,std::string> ret;
    _message_queue_mutex.lock();
    while (_message_queue.empty()) {
        _message_queue_mutex.Wait();
    }
    ret = _message_queue.front();
    _message_queue.pop_front();
    _message_queue_mutex.unlock();
    return ret;
}

void PlaybackProgressUpdater::set_writer(DjapUtils::WeakPointer<MplayerWriter> writer) {
    _writer = writer;
}

PlaybackProgressUpdater::PlaybackProgressUpdater() {
    _stop = false;
}

PlaybackProgressUpdater::~PlaybackProgressUpdater() {

}
void PlaybackProgressUpdater::stop() {
    _sleeper.lock();
    _stop = true;
    _sleeper.unlock();
    _sleeper.Signal();
}
void PlaybackProgressUpdater::thread_main() {
    _sleeper.lock();
    while (false == _stop) {
        do {
            DjapUtils::SharedPointer<MplayerWriter> writer = _writer;
            if (nullptr != writer.raw_ptr()) {
                writer->write_command("get_property time_pos\n");
            }
        } while (false);
        _sleeper.timed_wait(FLAGS_mplayer_progress_update_interval_ms); // interruptible sleep
    }
    _sleeper.unlock();
}


MPlayerController::MPlayerController() {
    LOG(DEBUG) << "Alloc mplayer controller!";
    init_mplayer_process();
    _is_playing = false;
}

MPlayerController::~MPlayerController() {
    _writer->write_command("quit\n");
    _reader->stop();
    _writer->stop();
    _progress_updater->stop();
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

        _writer = MplayerWriter::alloc<MplayerWriter>();
        _writer->set_write_fd(_mplayer_process_write_fd);
        _writer->start();

        _progress_updater = PlaybackProgressUpdater::alloc<PlaybackProgressUpdater>();
        _progress_updater->set_writer(_writer.to_weak());
        _progress_updater->start();
    }
}

void MPlayerController::set_delegate(DjapUtils::SharedPointer<MplayerControllerDelegate> delegate) {
    _delegate = delegate;
    _reader->set_delegate(_delegate.to_weak());
}

int MPlayerController::set_file(const std::string& file_path) {
    _file_name = file_path;
    return 0;
}

int MPlayerController::play() {
    if (true == _is_playing) {
        return -1;
    }
    if (0 >= _file_name.length()) {
        return -2;
    }

    std::string command = std::string("loadfile ") + _file_name + std::string("\n");
    _writer->write_command(command);
    //LOG(DEBUG) << "Write to mplayer return: "
    //           << write(_mplayer_process_write_fd, command.c_str(), command.size());
    //write(_mplayer_process_write_fd, command.c_str(), command.size());
    /*
    iMplayerProcess->write( QString("get_property filename\n").toLocal8Bit() );
    iMplayerProcess->waitForBytesWritten(1000);
    iMplayerProcess->write( QString("get_property width\n").toLocal8Bit() );
    iMplayerProcess->waitForBytesWritten(1000);
    iMplayerProcess->write( QString("get_property height\n").toLocal8Bit() );
    iMplayerProcess->waitForBytesWritten(1000);
    iMplayerProcess->write( QString("get_property aspect\n").toLocal8Bit() );
    iMplayerProcess->waitForBytesWritten(1000);
    iMplayerProcess->write( QString("get_property length\n").toLocal8Bit() );
    iMplayerProcess->waitForBytesWritten(1000);
     */

    return 0;

}

int MPlayerController::pause() {
    _writer->write_command("pause\n");
    return 0;
}

int MPlayerController::set_volume(int volume) {
    if(volume > 100)
            volume = 100;
    if(volume < 0)
            volume = 0;
    std::stringstream ss;
    ss << "set_property volume " << volume << "\n";
    _writer->write_command(ss.str());
}

int MPlayerController::stop() {
    _writer->write_command("stop\n");
}

void MPlayerController::seek(int pos) {
    /*
        //Clear "now playing"
        emit gotMediaTitle(QString::null);
        emit gotMediaArtist(QString::null);
        emit gotMediaCreationDate(QString::null);
        emit gotMediaAlbum(QString::null);
        if(iIsPlaying)
            {
                emit stopPlayingProgressTimer();
                iIsPlaying = false;
                iMplayerProcess->write(QByteArray("stop\n"));
                iMplayerProcess->waitForBytesWritten(1000);
                iPaused = false;
                emit playingProgress( 0 );
                return true;
            }
        return false;
     */
}

}

