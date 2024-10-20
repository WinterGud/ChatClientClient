cmake_minimum_required(VERSION 3.29)
project(ChatClientClient)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "C:/boost")
set(Boost_INCLUDE_DIRS ${BOOST_ROOT}/include/boost-1_86/boost)
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/lib)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost)

add_executable(ChatClientClient
        main.cpp
        Client.cpp
        Client.h
        SignalServer.cpp
        SignalServer.h
)

target_include_directories(ChatClientClient PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(ChatClientClient PRIVATE
        ${Boost_LIBRARY_DIR}/libboost_system-mgw14-mt-x64-1_86.a
        ${Boost_LIBRARY_DIR}/libboost_thread-mgw14-mt-x64-1_86.a
        ws2_32
        mswsock
)
