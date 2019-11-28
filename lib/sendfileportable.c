/*
 * Copyright 2019 Niclas Rosenvik
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <unistd.h>

#ifdef HAVE_HEADER_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_HEADER_SYS_PARAM_H
#include <sys/param.h>
#endif

#ifdef HAVE_HEADER_SYS_SENDFILE_H
#include <sys/sendfile.h>
#endif

#include "sendfileportable.h"

int do_sendfilep(int,int,off_t,size_t,off_t*);

int
sendfileportable(int s, int fd, off_t offset, size_t nbytes, off_t *sbytes)
{
  return do_sendfilep(s, fd, offset, nbytes, sbytes);
}

int
do_sendfilep(int out_fd, int in_fd, off_t offset, size_t count, off_t *sbytes)
{
#if defined(HAVE_PROTOTYPE_SENDFILE_ORG)
  ssize_t retval;
  off_t new_offset = offset;
  retval = sendfile(out_fd, in_fd, &new_offset, count);

  *sbytes = new_offset - offset;
  if (retval == -1){
    return -1;
  }
  return 0;
#endif

#if defined(HAVE_PROTOTYPE_SENDFILE_FBSD)
  if (count == 0) {
    *sbytes = 0;
    return 0;
  }
  return sendfile(in_fd, out_fd, offset, count, NULL, sbytes, 0);
#endif

#if defined(__NetBSD_Version__)
  void *start_addr;
  ssize_t ret_write;
  int addr_hint = 0;
  *sbytes = 0;

  start_addr = mmap((void*)&addr_hint, count, PROT_READ, MAP_FILE, in_fd, offset);
  if (start_addr == MAP_FAILED) {
    return -1;
  }

  ret_write = write(out_fd, start_addr, count);
  if (ret_write == -1) {
    return -1;
  }

  *sbytes = ret_write;
  if (ret_write < count) {
    errno = EAGAIN;
    return -1;
  }

  return 0;
#endif
}
