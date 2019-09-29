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
#ifndef _MPLAYER_SERVICE_MPLAYER_CONTROLLER_HPP_
#define _MPLAYER_SERVICE_MPLAYER_CONTROLLER_HPP_

#include <string>
#include "djap_utils/include/thread.hpp"

namespace player_service {

/*
 * Callback interface to receive events from mplayer
 */
class MplayerControllerDelegate {
public:
    virtual void did_start(const std::string& filename) = 0;
    virtual void did_end(const std::string& error) = 0;
    virtual void did_pause() = 0;
    virtual void did_resume() = 0;
    virtual void did_get_info_track_artist(const std::string& artist) = 0;
    virtual void did_get_info_track_name(const std::string& name) = 0;
    virtual void did_get_info_track_album(const std::string& album) = 0;
    virtual void did_get_info_track_progress(uint64_t progress_ms) = 0;
    virtual void did_get_info_track_volume(uint64_t volume_percent) = 0;
};

/*
 * Reader thread for reading from mplayer process.
 */
class MplayerReader : public DjapUtils::Thread {
public:
    void set_read_fd(int fd);
    void set_delegate(DjapUtils::WeakPointer<MplayerControllerDelegate> delegate);
    void stop();
protected:
    MplayerReader();
    virtual ~MplayerReader();
    virtual void thread_main();
private:
    void process_event(const std::string& event_buffer);
private:
    bool _stop_requested;
    int _read_fd;
    DjapUtils::Mutex _delegate_lock;
    DjapUtils::WeakPointer<MplayerControllerDelegate> _delegate;
    friend class DjapUtils::SharedPointer<MplayerReader>;
    friend class DjapUtils::Thread;
};

class MPlayerController {
public:
    MPlayerController();
    virtual ~MPlayerController();
    void set_delegate(DjapUtils::SharedPointer<MplayerControllerDelegate> delegate);
    int set_file(const std::string& file_path);
    int play();
    int pause();
    int set_volume(int volume);
    int stop();
    void seek(int pos);
private:
    void init_mplayer_process();
    int _mplayer_process_write_fd;
    int _mplayer_process_read_fd;
    DjapUtils::SharedPointer<MplayerReader> _reader;
    DjapUtils::SharedPointer<MplayerControllerDelegate> _delegate;

    std::string _file_name;
    bool        _is_playing;
};

}

#endif // _MPLAYER_SERVICE_MPLAYER_CONTROLLER_HPP_
