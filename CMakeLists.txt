cmake_minimum_required (VERSION 3.5 FATAL_ERROR)
include(GNUInstallDirs)
project (ferrybase CXX)
set (CMAKE_USE_RELATIVE_PATHS 1)
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (Umbrella "ferryfair")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Mac OS X specific code
SET(macOS ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
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
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

set (GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -std=c++14")
set (GCC_COVERAGE_LINK_FLAGS "-lxml2 -lpthread -lssl -lcrypto -lz -lrt")
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "/usr/include/libxml2")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
if(macOS)
set(SOURCES FerryTimeStamp.cpp myconverters.cpp mystdlib.cpp)
set(HEADERS FerryTimeStamp.h myconverters.h mystdlib.h)
else(macOS)
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)
endif(macOS)
add_library(${PROJECT_NAME}Static STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME}Static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
    )
set_target_properties(${PROJECT_NAME}Static
    PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}
    SOVERSION ${VERSION_MAJOR}
    )
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS ${PROJECT_NAME}Static DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella}/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/config.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${Umbrella}/${PROJECT_NAME}
    RENAME ${PROJECT_NAME}Config.h)
install(FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    RENAME "${PROJECT_NAME}.pc")
install(FILES "${PROJECT_BINARY_DIR}/pkgconfig.pc"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
    RENAME "${PROJECT_NAME}.pc")
