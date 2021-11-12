cmake_minimum_required(VERSION 3.21)
project(bfs)

set(CMAKE_CXX_STANDARD 17)

find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(bfs main.cpp maze.cpp bfs.cpp)
target_include_directories(bfs PRIVATE
        ${GLEW_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIR})
target_link_libraries(bfs PRIVATE
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        ${GLEW_LIBRARIES})
