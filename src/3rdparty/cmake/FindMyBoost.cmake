if(TARGET boost)
  return()
endif()

SET(BOOST_INCLUDE_DIR ${BOOST_ROOT}/)

if(WIN32)
  file(GLOB BOOST_LIB  ${BOOST_ROOT}/lib/win/*)
elseif(APPLE)
   file(GLOB BOOST_LIB  ${BOOST_ROOT}/lib/osx/* )
elseif(UNIX)
   file(GLOB BOOST_LIB  ${BOOST_ROOT}/lib/unix/*)
endif()

if(NOT BOOST_LIB)
  message(SEND_ERROR "Cannot find BOOST library at path: " ${BOOST_ROOT})
endif()

set(Boost_USE_STATIC_LIBS        ON)
   set(Boost_USE_MULTITHREADED      ON)
   set(Boost_USE_STATIC_RUNTIME    OFF)

add_library(boost STATIC IMPORTED GLOBAL)
set_target_properties(boost     PROPERTIES
  IMPORTED_LOCATION_DEBUG       ${BOOST_LIB}
  IMPORTED_LOCATION             ${BOOST_LIB}
  INTERFACE_INCLUDE_DIRECTORIES ${BOOST_INCLUDE_DIR}
)

