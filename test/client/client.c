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

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define PORT 38000

void usage(char *pname);

int
main(int argc, char *argv[])
{
  char rbuf[1024];
  size_t rbuf_len = 1024;

  if (argc != 2) {
    usage(argv[0]);
  }

  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("socket");
    return EXIT_FAILURE;
  }

  struct sockaddr_in addrl;
  addrl.sin_family = AF_INET;
  addrl.sin_port = htons(PORT);
  if (!inet_aton("127.0.0.1", &addrl.sin_addr)) {
    perror("inet_aton");
    return EXIT_FAILURE;
  }

  if (connect(s,(struct sockaddr*)&addrl, sizeof (struct sockaddr_in))){
    perror("connect");
    return EXIT_FAILURE;
  }

  int out_fd = open(argv[1],
		    O_WRONLY|O_APPEND|O_CREAT|O_TRUNC,
		    S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if (out_fd == -1) {
    perror("open");
    shutdown(s, SHUT_RDWR);
    return EXIT_FAILURE;
  }

  int rr;
  int wr;
  do {
    rr = read(s, rbuf, rbuf_len);
    if (rr == -1) {
      perror("read");
      shutdown(s, SHUT_RDWR);
      close(out_fd);
      return EXIT_FAILURE;
    }
    if (rr != 0) {
      wr = write(out_fd, rbuf, rr);
      if (wr == -1) {
	perror("write");
	shutdown(s, SHUT_RDWR);
	close(out_fd);
	return EXIT_FAILURE;
      }
    }
  } while(rr);

  if (shutdown(s, SHUT_RDWR)) {
    perror("shutdown");
    close(out_fd);
    return EXIT_FAILURE;
  }

  if (close(out_fd)) {
    perror("close");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void
usage(char *pname)
{
  (void)fprintf(stderr, "usage: %s filename\n", pname);
  exit(EXIT_FAILURE);
}
