cmake_minimum_required(VERSION 3.10)
project(ConnectFour)

set(CMAKE_CXX_STANDARD 14)

# Update the path to your SFML installation if necessary
# For example, if SFML is installed in /usr/local/SFML-2.5.1:
set(SFML_DIR "/usr/local/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(ConnectFour main.cpp)
target_link_libraries(ConnectFour sfml-graphics sfml-window sfml-system)
