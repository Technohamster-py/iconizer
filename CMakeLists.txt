cmake_minimum_required(VERSION 3.27)
project(iconizer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "/usr/local/qt/Qt-6.6.1")

set(TARGET iconizer)

add_executable(${TARGET}
    src/core/main.cpp
)

target_link_libraries(${TARGET}
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)