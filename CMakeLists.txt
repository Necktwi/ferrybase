cmake_minimum_required (VERSION 2.6)
project (base)
set (base_VERSION_MAJOR 1)
set (base_VERSION_MINOR 0)
set (Umbrella "ferryfair")
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.h.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
    )
configure_file(
    "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.pc.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc"
    )
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

set (GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -std=c++14")
set (GCC_COVERAGE_LINK_FLAGS "-lxml2 -lpthread -lssl -lcrypto -lz -lrt")
include_directories(${CMAKE_CURRENT_SOURCE_DIR} "/usr/include/libxml2")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
file(GLOB SOURCES *.cpp)
add_library(baseStatic STATIC ${SOURCES})
add_library(base SHARED ${SOURCES})
set_target_properties(baseStatic PROPERTIES OUTPUT_NAME base)
set_target_properties(base
    PROPERTIES
    VERSION ${base_VERSION_MAJOR}.${base_VERSION_MINOR}
    SOVERSION ${base_VERSION_MAJOR}
    )
set_target_properties(baseStatic
    PROPERTIES
    VERSION ${base_VERSION_MAJOR}.${base_VERSION_MINOR}
    SOVERSION ${base_VERSION_MAJOR}
    )
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
file(GLOB HEADERS *.h)
install(FILES ${HEADERS} DESTINATION include/${Umbrella}/${PROJECT_NAME})
install(FILES "${PROJECT_BINARY_DIR}/baseConfig.h"
    DESTINATION include/${Umbrella}/${PROJECT_NAME})
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc"
    DESTINATION lib/pkgconfig
    RENAME "lib${PROJECT_NAME}.pc")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc"
    DESTINATION share/pkgconfig
    RENAME "lib${PROJECT_NAME}.pc")
