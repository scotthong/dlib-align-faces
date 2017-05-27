#
# This is the CMake makefile to build align_faces and detect_faces based on the Dlib.
# You can find the cmake utility and information at http://www.cmake.org
# You can find the Dlib information at http://dlib.net/
#

cmake_minimum_required(VERSION 2.8.4)

PROJECT(dlib-align-faces)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

if (WIN32 AND NOT CYGWIN) 
	SET(LIBJPEG_LIB ${CMAKE_SOURCE_DIR}/libjpeg/windows_x86_64/lib/libjpeg_a.lib)
	SET(LIBJPEG_INC ${CMAKE_SOURCE_DIR}/libjpeg/windows_x86_64/include)
elseif (APPLE)
	SET(LIBJPEG_LIB ${CMAKE_SOURCE_DIR}/libjpeg/mac_x86_64/lib/libjpeg.dynlib)
	SET(LIBJPEG_INC ${CMAKE_SOURCE_DIR}/libjpeg/mac_x86_64/include)
else()
	SET(LIBJPEG_LIB ${CMAKE_SOURCE_DIR}/libjpeg/unix_x86_64/lib/libjpeg.a)
	SET(LIBJPEG_INC ${CMAKE_SOURCE_DIR}/libjpeg/unix_x86_64/include)
endif()

include(dlib/dlib/cmake)

include_directories(${LIBJPEG_INC})

ADD_EXECUTABLE(align_faces align_faces.cpp)
TARGET_LINK_LIBRARIES(align_faces dlib ${LIBJPEG_LIB})

ADD_EXECUTABLE(train_object_detector train_object_detector.cpp)
TARGET_LINK_LIBRARIES(train_object_detector dlib)

ADD_EXECUTABLE(train_shape_predictor train_shape_predictor_ex.cpp)
TARGET_LINK_LIBRARIES(train_shape_predictor dlib)
