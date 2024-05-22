cmake_minimum_required(VERSION 3.5)
project(simconnect-bus-toolbox LANGUAGES CXX VERSION 0.8.2)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/deps/BlockFactory")

find_package(Matlab COMPONENTS
    ENG_LIBRARY
    SIMULINK)

find_package(
        BlockFactory COMPONENTS Core
)

include_directories(
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Compile S-Function
matlab_add_mex(
    NAME SimConnectBusInterface
    SRC src/Simulink/SimConnectBusInterface.cpp
        src/Interface/SimConnectInterface.cpp
    LINK_TO BlockFactory::Core)

add_custom_command(
        TARGET SimConnectBusInterface
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE_DIR:SimConnectBusInterface>/SimConnectBusInterface.mexw64" "${CMAKE_SOURCE_DIR}/matlab"
)
