
#ifndef _LIBNRNPFP_H_
#define _LIBNRNPFP_H_
#include <sys/types.h>

int nrnpfp_sendfile(int s, int fd, off_t offset, size_t nbytes, off_t *sbytes);

#endif
