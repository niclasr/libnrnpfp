cmake_minimum_required(VERSION 3.5)

project(nrnpfp VERSION 0.0.0 LANGUAGES C)

option(BUILD_TESTS "build tests" ON)

#configure

include(cmake/configure.cmake)

#targets
add_library(nrnpfp SHARED lib/sendfile.c)
target_compile_definitions(nrnpfp PRIVATE HAVE_HEADER_CONFIG_H)
if(NEED_SENDFILE_LIB)
target_link_libraries(nrnpfp sendfile)
target_link_libraries(nrnpfp-static sendfile)
endif()

if(BUILD_TESTS)
add_library(nrnpfp-static STATIC lib/sendfile.c)
target_compile_definitions(nrnpfp-static PRIVATE HAVE_HEADER_CONFIG_H)
target_include_directories(nrnpfp-static INTERFACE
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>)
add_executable(server test/server/server.c)
target_link_libraries(server nrnpfp-static)
add_executable(client test/client/client.c)
endif()

