# list does not recognize sub-command FILTER for version 3.5
cmake_minimum_required (VERSION 3.5)
include(GNUInstallDirs)
project (ferrybase)
message ("Generating build procedure for ${PROJECT_NAME} ...")

message ("Extropolating build type ...")
string (REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" SRC_RLTV_BLD_DIR
   ${PROJECT_BINARY_DIR}
)
message ("SRC_RLTV_BLD_DIR: ${SRC_RLTV_BLD_DIR}")
string (REPLACE "/Build" "" BUILD_TARGET ${SRC_RLTV_BLD_DIR})
if (${SRC_RLTV_BLD_DIR})
   string (REPLACE "/build" "" BUILD_TARGET ${BUILD_TARGET})
endif ()
message ("BUILD_TARGET: ${BUILD_TARGET}")


if (DEFINED _DEBUG)
  ADD_DEFINITIONS(-D_DEBUG=${_DEBUG})
endif()
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (Umbrella "ferryfair")
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set (macOS ON)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set (Linux ON)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set (Windows ON)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
if (NOT LINKAGE)
  set(LINKAGE "STATIC")
  message("Building static by default")
else ()
  string(TOUPPER "${LINKAGE}" LINKAGE)
endif ()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
)
configure_file(
  "${PROJECT_SOURCE_DIR}/pkgconfig.pc.in"
   "${PROJECT_BINARY_DIR}/pkgconfig.pc"
)
set (GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
set (GCC_COVERAGE_LINK_FLAGS 
   "-fPIC -Wl,-unresolved-symbols=ignore-in-shared-libs")
if (DEFINED _DEBUG)
  set (GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")
  set (GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -g -O0")
endif ()
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} 
   ${GCC_COVERAGE_LINK_FLAGS}")
if(macOS OR Windows)
  set(SOURCES FerryTimeStamp.cpp myconverters.cpp mystdlib.cpp 
   ServerSocket.cpp Socket.cpp)
  set(HEADERS FerryTimeStamp.h myconverters.h mystdlib.h ServerSocket.h Socket.h
   SocketException.h)
else(macOS OR Windows)
   file(GLOB HEADERS [a-zA-Z0-9_]*.h)
   file(GLOB SOURCES [a-zA-Z0-9_]*.cpp)
endif()
list(APPEND HEADERS "config.h.in"  ${PROJECT_BINARY_DIR}/config.h)
include_directories(${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${Umbrella} /usr/include/libxml2
)
if (macOS)
   message("openssl include")
   include_directories(/usr/local/opt/openssl/include)
   link_directories(/usr/local/opt/openssl/lib)
endif ()

if (${LINKAGE} MATCHES "STATIC")
  add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
  set_target_properties(${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
  )
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
  )
  if (macOS)
      target_link_libraries(${PROJECT_NAME} ssl crypto)
  elseif (Windows)
  else ()
      target_link_libraries(${PROJECT_NAME} xml2 pthread ssl crypto z rt)
  endif ()
else ()
  add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
  set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
  )
   if (macOS)
      message("openssl include")
      include_directories(/usr/local/opt/openssl/include)
      link_directories(/usr/local/opt/openssl/lib)
   endif ()
   if (macOS)
      target_link_libraries(${PROJECT_NAME} ssl crypto)
   elseif (Windows)
  else (macOS)
    target_link_libraries(${PROJECT_NAME} xml2 pthread ssl crypto z rt)
  endif ()
  
endif ()
install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella}/${PROJECT_NAME}
)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella}/${PROJECT_NAME}
  RENAME ${PROJECT_NAME}Config.h
)
install (FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  RENAME "${PROJECT_NAME}.pc"
)
install (FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
  RENAME "${PROJECT_NAME}.pc"
)
