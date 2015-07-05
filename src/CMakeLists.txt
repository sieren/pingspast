##  --------------------------------------------------------------------------------------
##   Create Executable
##  --------------------------------------------------------------------------------------


set(SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB FILES_WP RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC_ROOT}/*.cpp ${SRC_ROOT}/*.h)


if(APPLE)
  add_executable(pingspast MACOSX_BUNDLE ${FILES_WP})
elseif(WIN32)
  add_executable(pingspast WIN32 ${FILES_WP})
elseif(UNIX)
  add_executable(pingspast ${FILES_WP})
endif()

##  --------------------------------------------------------------------------------------
##   Dependencies
##  --------------------------------------------------------------------------------------

find_package(MyPortaudio REQUIRED)
find_package(MyBoost REQUIRED)

target_include_directories(pingspast PUBLIC .)
target_include_directories(pingspast PUBLIC  ${BOOST_INCLUDE_DIR})

target_link_libraries(pingspast PUBLIC portaudio)
target_link_libraries(pingspast PUBLIC ${BOOST_LIB})

if(APPLE)
  target_link_libraries(pingspast PUBLIC "-framework AudioToolbox" )
  target_link_libraries(pingspast PUBLIC "-framework AudioUnit" )
  target_link_libraries(pingspast PUBLIC "-framework CoreServices" )
  target_link_libraries(pingspast PUBLIC "-framework CoreFoundation" )
  target_link_libraries(pingspast PUBLIC "-framework CoreAudio" )
elseif(WIN32)
  target_link_libraries(pingspast  PRIVATE mfplat.lib mfreadwrite.lib mfuuid.lib Propsys.lib)
elseif(UNIX)
  target_link_libraries(pingspast PUBLIC "-lpthread -lasound -lrt") # -ljack
endif()

