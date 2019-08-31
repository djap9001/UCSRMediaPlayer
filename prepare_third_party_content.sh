#!/bin/sh
# TODO currently a list of steps to geth things to build on my env. prepare a script later
# linking all statically...
# brpc_raspbian_build.sh (from https://github.com/djap9001/brpc-raspberryPI-buildscripts)
# has been run under ~/brpc_test

# LIBRARIES
#cd ~/brpc_test/leveldb/out-static
#ar rcs libleveldb.a db/*.o helpers/memenv/*.o port/*.o table/*.o util/*.o
#cp libleveldb.a ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/lib/

#cd ~/brpc_test/gflags/lib
#cp libgflags.a ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/lib/

#cd ~/brpc_test/protobuf/src/.libs
#cp libprotobuf.a ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/lib/

#cd ~/brpc_test/protobuf/src/.libs
#cp libprotobuf.a ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/lib/

#cd ~/brpc_test/incubator-brpc/output/lib
#cp libbrpc.a ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/lib/


# HEADERS
#cd ~/brpc_test/incubator-brpc/output/include
#cp -r * ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/include

#cd /home/pi/brpc_test/gflags/include/
#cp -r gflags ../../../UCSRMediaPlayer2/UCSRMediaPlayer/third_party/brpc/include

# OTHER DEPENDENT LIBRARIES LINKED DYNAMICALLY
pthread,ssl,crypto,dl,z