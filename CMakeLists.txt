cmake_minimum_required(VERSION 3.10)
project(BattlefieldAudio)
set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(OPUS REQUIRED opus)
pkg_check_modules(RTAUDIO REQUIRED rtaudio)
pkg_check_modules(WEBRTC REQUIRED webrtc-audio-processing)

include_directories(${OPUS_INCLUDE_DIRS} ${RTAUDIO_INCLUDE_DIRS} ${WEBRTC_INCLUDE_DIRS})
add_executable(BattlefieldAudio main.cpp JitterBuffer.cpp)
target_link_libraries(BattlefieldAudio ${OPUS_LIBRARIES} ${RTAUDIO_LIBRARIES} ${WEBRTC_LIBRARIES} pthread)