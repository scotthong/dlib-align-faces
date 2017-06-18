#
# This is the CMake makefile to build align_faces and detect_faces based on the Dlib.
# You can find the cmake utility and information at http://www.cmake.org
# You can find the Dlib information at http://dlib.net/
#

cmake_minimum_required(VERSION 2.8.4)

PROJECT(dlib-align-faces)

# For windows_x86_64 platform, use the pre-compiled libjpeg distributed with this project.
# For non windows platform, use libjpeg installed for the your platform. Please make sure that
# the minimum libjpeg version has to be greater than 80.

include(dlib/dlib/cmake)

if (WIN32 AND NOT CYGWIN)
    #
    # The pre-compiled libjpeg is downloaded using the following link.
    # http://windows.php.net/downloads/php-sdk/deps/vc15/x64/libjpeg-9b-vc14-x64.zip
    #
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    SET(LIBJPEG_LIB ${CMAKE_SOURCE_DIR}/libjpeg/windows_x86_64/lib/libjpeg_a.lib)
	SET(LIBJPEG_INC ${CMAKE_SOURCE_DIR}/libjpeg/windows_x86_64/include)
    include_directories(${LIBJPEG_INC})

elseif(APPLE)
    # Under mac_x86_64, use brew to install libjpeg
    # brew install libjpeg
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    SET(LIBJPEG_LIB "jpeg")

else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -static")
    SET(LIBJPEG_LIB "")

endif()

ADD_EXECUTABLE(align_faces align_faces.cpp)
TARGET_LINK_LIBRARIES(align_faces ${LIBJPEG_LIB} dlib)

# ADD_EXECUTABLE(train_object_detector train_object_detector.cpp)
# TARGET_LINK_LIBRARIES(train_object_detector ${LIBJPEG_LIB} dlib)

# ADD_EXECUTABLE(train_shape_predictor train_shape_predictor_ex.cpp)
# TARGET_LINK_LIBRARIES(train_shape_predictor ${LIBJPEG_LIB} dlib)
