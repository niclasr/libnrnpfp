
#ifndef _SENDFILEPORTABLE_H_
#define _SENDFILEPORTABLE_H_
#include <sys/types.h>

int sendfileportable(int s, int fd, off_t offset, size_t nbytes, off_t *sbytes);

#endif
