cmake_minimum_required(VERSION 2.6)

project(zpdlib_test)

set(CMAKE_MACOSX_RPATH On)
set(BUILD_SHARED_LIBRARIES False)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../zpd/)
link_directories(${PROJECT_SOURCE_DIR}/../lib/${CMAKE_BUILD_TYPE})
add_executable(zpdlib_test test.c)
target_link_libraries(zpdlib_test zpd)
set_target_properties(zpdlib_test PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
