cmake_minimum_required(VERSION 3.16)

project(Calculator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS
    Quick
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.h
)

add_executable(calculator_app
    main.cpp
    ${SOURCES}
    qml.qrc
)

target_link_libraries(calculator_app
    PRIVATE
    Qt6::Quick
)
