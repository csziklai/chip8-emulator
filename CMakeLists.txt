set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED True)

cmake_minimum_required(VERSION 3.10)
project(Chip8)

include_directories(/opt/homebrew/opt/freeglut/include/)
link_directories(/opt/homebrew/opt/freeglut/lib)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


add_executable(chip8 src/main.cpp src/chip8.cpp)
target_link_libraries(chip8 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
