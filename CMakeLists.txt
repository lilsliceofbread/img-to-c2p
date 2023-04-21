cmake_minimum_required(VERSION 3.21.1)
project(imgtoc2p)

include_directories(inc)
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})