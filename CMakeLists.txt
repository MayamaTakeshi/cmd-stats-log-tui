cmake_minimum_required(VERSION 2.8.10)
project(cmd-stats-log-tui)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(csl-tui main.c tui.c)
target_link_libraries(csl-tui ncurses)
