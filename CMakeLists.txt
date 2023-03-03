cmake_minimum_required(VERSION 3.10)

project(CamearAPI)

set(DCMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "-g -o0")

include_directories(
    ./
    ../../inc
)

link_directories(
    /usr/lib
    ../../lib/x86_64
    ./
)

add_executable(APITest
    APITest.c
    DHCameraControl.c
    )
target_link_libraries(APITest
    -lgxiapi
    -lpthread
    )
