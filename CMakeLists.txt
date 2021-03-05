cmake_minimum_required(VERSION 3.0)
project( app C CXX )

set( HEADER include/ )
set( SOURCE src/BG96.cpp src/Common.cpp src/HttpPostFormData.cpp src/Serial.cpp )
set( BG96 main/test-BG96.cpp )

include_directories( ${HEADER} )
add_executable( BG96 ${BG96} ${SOURCE} )

target_compile_options( app PUBLIC -std=c++17 )