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
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sendfileportable.h>

#define PORT 38000

void usage(void);

int
main(int argc, char *argv[])
{

  setprogname(argv[0]);
  if (argc != 2) {
    usage();
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

  if (bind(s,(struct sockaddr*)&addrl, sizeof (struct sockaddr_in))) {
    perror("bind");
    return EXIT_FAILURE;
  }

  if (listen(s,1)) {
    perror("listen");
    return EXIT_FAILURE;
  }

  int in_fd = open(argv[1], O_RDONLY);
  if (in_fd == -1) {
    perror("open");
    close(s);
    return EXIT_FAILURE;
  }

  off_t in_fd_size = lseek(in_fd, 0, SEEK_END);
  if (in_fd_size == -1) {
    perror("lseek");
    close(s);
    close(in_fd);
    return EXIT_FAILURE;
  }

  struct sockaddr_in addrc;
  socklen_t addrc_len = sizeof (struct sockaddr_in);
  int conn = accept(s, (struct sockaddr *)&addrc, &addrc_len);
  if (conn == -1) {
    perror("accept");
    close(s);
    close(in_fd);
    return EXIT_FAILURE;
  }
  if (close(s)) {
    perror("close listen socket");
    shutdown(conn, SHUT_RDWR);
    close(conn);
    close(in_fd);
    return EXIT_FAILURE;
  }

  size_t sbytes;
  if(sendfileportable(conn, in_fd, 0, in_fd_size, &sbytes)) {
    perror("sendfileportable");
    shutdown(conn, SHUT_RDWR);
    close(conn);
    close(s);
    return EXIT_FAILURE;
  }
  printf("%lu bytes was sent to the client\n", sbytes);

  if (shutdown(conn, SHUT_RDWR)) {
    perror("shutdown conn socket");
    close(conn);
    close(in_fd);
    return EXIT_FAILURE;
  }

  if (close(conn)) {
    perror("close connection socket");
    close(in_fd);
    return EXIT_FAILURE;
  }

  if (close(in_fd)) {
    perror("close file");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void
usage()
{
  (void)fprintf(stderr, "usage: %s filename\n", getprogname());
  exit(EXIT_FAILURE);
}
