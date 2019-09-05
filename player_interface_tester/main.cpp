/*
 * Simple tester for get some requests going to mplayer_service for testing and development purposes.
 * Shameless copy, paste and modify from provided examples in incubator-brpc repo.
 */
#include <stdio.h>
#include <gflags/gflags.h>
#include <butil/logging.h>
#include <butil/time.h>
#include <brpc/channel.h>
//#include "echo.pb.h"
#include "player_service.pb.h"

DEFINE_string(protocol, "baidu_std", "Protocol type. Defined in src/brpc/options.proto");
DEFINE_string(connection_type, "", "Connection type. Available values: single, pooled, short");
DEFINE_string(server, "127.0.0.1:8000", "IP Address of server");
DEFINE_string(load_balancer, "", "The algorithm for load balancing");
DEFINE_int32(timeout_ms, 100, "RPC timeout in milliseconds");
DEFINE_int32(max_retry, 3, "Max retries(not including the first RPC)");
DEFINE_int32(interval_ms, 1000, "Milliseconds between consecutive requests");

DEFINE_string(test_mp3_file, "/home/pi/Documents/amaranth.mp3", "Path for test file while pushing to playlist");

int log_id = 0;

void send_player_properties_request(player_service::PlayerService_Stub &stub) {
    player_service::PlayerPropertiesRequest request;
    player_service::PlayerPropertiesResponse response;
    brpc::Controller cntl;

    cntl.set_log_id(log_id ++);  // set by user

    stub.PlayerProperties(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Received PlayerPropertiesResponse from " << cntl.remote_side()
            << " to " << cntl.local_side()
            << " supported filetypes: ";
            for (int i = 0; i < response.supported_file_types_size(); i++) {
                LOG(INFO) << response.supported_file_types(i) << " ";
            }
            LOG(INFO) << "Player name: " << response.player_name();
            LOG(INFO) << "Player description: " << response.player_description();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }
}

void send_reset_playlist_request(player_service::PlayerService_Stub &stub) {
    player_service::ResetRequest request;
    player_service::ResetResponse response;
    brpc::Controller cntl;

    cntl.set_log_id(log_id ++);  // set by user
    stub.ResetPlaylist(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Received ResetResponse from " << cntl.remote_side()
            << " to " << cntl.local_side()
            << " Error code: " << response.error_code()
            << " Error description" << response.error_message();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }
}

void send_push_to_playlist_request(player_service::PlayerService_Stub &stub) {
    player_service::PushToPlaylistRequest request;
    player_service::PushToPlaylistResponse response;
    brpc::Controller cntl;

    cntl.set_log_id(log_id ++);  // set by user
    request.set_original_path(FLAGS_test_mp3_file);
    request.set_push_to_index(-1); // -1 push to end of list, other indexes will insert the file to given index and pushes the others behind it.
                                    // if index is over the list size, the file will be pushed to the end of the list, actual index is returned in
                                    // response
    request.set_file_type(std::string("mp3"));
    // populate actual file data
    std::string* file_data = request.mutable_file_data();
    FILE* fp = fopen(FLAGS_test_mp3_file.c_str(), "rb");
    if (nullptr != fp) {
        uint8_t read_buf[100000];
        while (!feof(fp)) {
            size_t read_count = fread(read_buf, 1, sizeof(read_buf), fp);
            if (0 < read_count) {
                file_data->append((const char*)read_buf, read_count);
            }
        }
        fclose(fp);
    }

    stub.PushToPlaylist(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Received PushToPlaylistResponse from " << cntl.remote_side()
            << " to " << cntl.local_side()
            << " Error code: " << response.error_code()
            << " Error description" << response.error_message();
            if (response.has_playlist_index()) {
                LOG(INFO) << "Placed in index: " << response.playlist_index();
            }
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }
}

void send_playback_control_request(player_service::PlayerService_Stub &stub) {
    player_service::PlaybackControlRequest request;
    player_service::PlaybackControlResponse response;
    brpc::Controller cntl;

    cntl.set_log_id(log_id ++);  // set by user

    request.set_control_type(player_service::PlaybackControlRequest_ControlType_PLAY);
    request.set_play_index(3); // play 4th file on the list, 0 based, -1 stands for continue where you were (resume on paused, last played index on stopped)
    request.set_volume_percent(50); // start at 50% volume (ControlType VOLUME allows adjusting this)

    stub.PlaybackControl(&cntl, &request, &response, NULL);
    if (!cntl.Failed()) {
        LOG(INFO) << "Received PushToPlaylistResponse from " << cntl.remote_side()
            << " to " << cntl.local_side()
            << " Error code: " << response.error_code()
            << " Error description" << response.error_message();
    } else {
        LOG(WARNING) << cntl.ErrorText();
    }
}

int main(int argc, char* argv[]) {

    GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);

    brpc::Channel channel;

    brpc::ChannelOptions options;
    options.protocol = FLAGS_protocol;
    options.connection_type = FLAGS_connection_type;
    options.timeout_ms = FLAGS_timeout_ms;
    options.max_retry = FLAGS_max_retry;
    if (channel.Init(FLAGS_server.c_str(), FLAGS_load_balancer.c_str(), &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return -1;
    }

    player_service::PlayerService_Stub stub(&channel);

    // Loop to send test requests
    while (!brpc::IsAskedToQuit()) {
        send_player_properties_request(stub);
        send_push_to_playlist_request(stub);
        send_playback_control_request(stub);
        send_reset_playlist_request(stub);
        usleep(FLAGS_interval_ms * 1000L);
    }

    LOG(INFO) << "EchoClient is going to quit";
    return 0;
}
