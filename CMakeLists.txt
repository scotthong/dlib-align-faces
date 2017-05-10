#
# This is the CMake makefile to build align_faces and detect_faces based on the Dlib.
# You can find the cmake utility and information at http://www.cmake.org
# You can find the Dlib information at http://dlib.net/
#

cmake_minimum_required(VERSION 2.8.4)

PROJECT(dlib-align-faces)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include(dlib/dlib/cmake)

ADD_EXECUTABLE(align_faces align_faces.cpp)
TARGET_LINK_LIBRARIES(align_faces dlib)
