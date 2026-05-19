cmake_minimum_required(VERSION 3.20)
project(Higherthanthecloud LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://gitee.com/mirrors/FTXUI.git
    GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)

add_executable(Higherthanthecloud 
    src/main.cpp
    src/game.cpp
)

target_compile_options(Higherthanthecloud PRIVATE -Wall -Wextra)

target_link_libraries(Higherthanthecloud PRIVATE
    ftxui::screen
    ftxui::dom
    ftxui::component
)