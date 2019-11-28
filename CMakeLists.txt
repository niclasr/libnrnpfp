cmake_minimum_required(VERSION 3.5)

project(sendfileportable VERSION 0.0.0 LANGUAGES C)

option(BUILD_TESTS "build tests" ON)

#configure

include(cmake/configure.cmake)

#targets
add_library(sendfileportable SHARED lib/sendfileportable.c)
target_compile_definitions(sendfileportable PRIVATE HAVE_HEADER_CONFIG_H)
if(NEED_SENDFILE_LIB)
target_link_libraries(sendfileportable sendfile)
target_link_libraries(sendfileportable-static sendfile)
endif()

if(BUILD_TESTS)
add_library(sendfileportable-static STATIC lib/sendfileportable.c)
target_compile_definitions(sendfileportable-static PRIVATE HAVE_HEADER_CONFIG_H)
target_include_directories(sendfileportable-static INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>)
add_executable(server test/server/server.c)
target_link_libraries(server sendfileportable-static)
add_executable(client test/client/client.c)
endif()

