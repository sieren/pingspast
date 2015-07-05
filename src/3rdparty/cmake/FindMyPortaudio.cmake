if(TARGET portaudio)
  return()
endif()

SET(PORTAUDIO_INCLUDE_DIR ${PORTAUDIO_ROOT}/include)

if(WIN32)
  set(PORTAUDIO_LIB_DEBUG   ${PORTAUDIO_ROOT}/lib/win/portaudiod_${MSVC_ARCH_NAME}_${MSVC_LIB_NAME}.lib)
  set(PORTAUDIO_LIB_RELEASE ${PORTAUDIO_ROOT}/lib/win/portaudio_${MSVC_ARCH_NAME}_${MSVC_LIB_NAME}.lib)
elseif(APPLE)
  set(PORTAUDIO_LIB_DEBUG   ${PORTAUDIO_ROOT}/lib/osx/libportaudiod.a)
  set(PORTAUDIO_LIB_RELEASE ${PORTAUDIO_ROOT}/lib/osx/libportaudio.a)
elseif(UNIX)
  set(PORTAUDIO_LIB_DEBUG   ${PORTAUDIO_ROOT}/lib/unix/libportaudiod.a)
  set(PORTAUDIO_LIB_RELEASE ${PORTAUDIO_ROOT}/lib/unix/libportaudio.a)
endif()

if(NOT PORTAUDIO_LIB_DEBUG OR NOT PORTAUDIO_LIB_RELEASE)
  message(SEND_ERROR "Cannot find PORTAUDIO library at path: " ${PORTAUDIO_ROOT})
endif()

add_library(portaudio STATIC IMPORTED GLOBAL)
set_target_properties(portaudio PROPERTIES
  IMPORTED_LOCATION_DEBUG       ${PORTAUDIO_LIB_DEBUG}
  IMPORTED_LOCATION             ${PORTAUDIO_LIB_RELEASE}
  INTERFACE_INCLUDE_DIRECTORIES ${PORTAUDIO_INCLUDE_DIR}
)

