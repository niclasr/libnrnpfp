
include(CheckIncludeFile)
include(CheckLibraryExists)

check_library_exists(sendfile sendfile /usr/lib NEED_SENDFILE_LIB)
check_include_file(sys/param.h HAVE_HEADER_SYS_PARAM_H)
check_include_file(sys/sendfile.h HAVE_HEADER_SYS_SENDFILE_H)

configure_file(cmake/config.h.in ${CMAKE_BINARY_DIR}/config.h)

include_directories(${CMAKE_BINARY_DIR})
