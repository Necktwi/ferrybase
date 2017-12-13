cmake_minimum_required (VERSION 3.5)
include(GNUInstallDirs)
project (ferrybase)
IF (DEFINED _DEBUG)
ADD_DEFINITIONS(-D_DEBUG=${_DEBUG})
ENDIF()
set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)
set (Umbrella "ferryfair")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
SET(macOS ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
SET(Linux ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
SET(Windows ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
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
set (GCC_COVERAGE_COMPILE_FLAGS "-std=c++14")
set (GCC_COVERAGE_LINK_FLAGS "-fPIC -Wl,-unresolved-symbols=ignore-in-shared-libs")
IF (DEFINED _DEBUG)
set (GCC_COVERAGE_COMPILE_FLAGS "${GCC_COVERAGE_COMPILE_FLAGS} -g -O0")
set (GCC_COVERAGE_LINK_FLAGS "${GCC_COVERAGE_LINK_FLAGS} -g -O0")
ENDIF()
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
if(macOS OR Windows)
set(SOURCES FerryTimeStamp.cpp myconverters.cpp mystdlib.cpp)
set(HEADERS FerryTimeStamp.h myconverters.h mystdlib.h)
else(macOS OR Windows)
file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)
endif(macOS OR Windows)
include_directories(${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${Umbrella} /usr/include/libxml2)
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
if(macOS)
elseif(Windows)
else(macOS)
target_link_libraries(${PROJECT_NAME}Static xml2 pthread ssl crypto z rt)
target_link_libraries(${PROJECT_NAME} xml2 pthread ssl crypto z rt)
endif(macOS)
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
