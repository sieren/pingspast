##  --------------------------------------------------------------------------------------
##   Create Project
##  --------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.2)

project (pingspast)


##  --------------------------------------------------------------------------------------
##   Setup
##  --------------------------------------------------------------------------------------

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fvisibility=hidden")



##  --------------------------------------------------------------------------------------
##   BOOST
##  --------------------------------------------------------------------------------------

# TODO: move to .../3rdparty/cmake/CMakeList.txt
set(BOOST_ROOT        ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/boost_1_58_0)
set(PORTAUDIO_ROOT    ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/portaudio-19)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/cmake")

SET(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
message(STATUS ${BOOST_ROOT})
#  --------------------------------------------------------------------------------------
##   Add subdirs
##  --------------------------------------------------------------------------------------

# add_subdirectory(src src-binary)
# add_subdirectory(PiCamLib       PiCamLib-binary)
add_subdirectory(src src-binary)




