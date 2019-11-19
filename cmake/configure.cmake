
include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckPrototypeDefinition)

check_library_exists(sendfile sendfile /usr/lib NEED_SENDFILE_LIB)
if (NEED_SENDFILE_LIB)
list(APPEND CMAKE_REQUIRED_LIBRARIES sendfile)
endif()

check_include_file(sys/param.h HAVE_HEADER_SYS_PARAM_H)
check_include_file(sys/sendfile.h HAVE_HEADER_SYS_SENDFILE_H)

check_prototype_definition(sendfile "ssize_t sendfile(int out_fd,int in_fd,off_t *offset,size_t count)" "0" "sys/sendfile.h" HAVE_PROTOTYPE_SENDFILE_ORG)
check_prototype_definition(sendfile "int sendfile(int fd,int s,off_t offset,size_t nbytes,struct sf_hdtr *hdtr,off_t *sbytes,int flags)" "0" "sys/types;sys/socket.h;sys/uio.h" HAVE_PROTOTYPE_SENDFILE_FBSD)

configure_file(cmake/config.h.in ${CMAKE_BINARY_DIR}/config.h)

include_directories(${CMAKE_BINARY_DIR})
