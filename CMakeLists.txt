cmake_minimum_required(VERSION 3.10)
project(Chip8)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(chip8 src/main.cpp src/chip8.cpp)
target_link_libraries(chip8 OpenGL::GL GLUT::GLUT)
