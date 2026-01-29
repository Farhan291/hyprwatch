#include "server_socket.hpp"
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

server_socket::server_socket(const std::string &path) : dd(-1), path(path) {};

int server_socket::create_socket() {
  dd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (dd < 0) {
    perror("socket()");
    return -1;
  }
  struct sockaddr_un addr = {0};
  addr.sun_family = AF_UNIX;
  std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

  unlink(addr.sun_path);
  if (bind(dd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
    close(dd);
    dd = -1;
    return -1;
  }
  if (listen(dd, 5) == -1) {
    perror("listen");
    close(dd);
    dd = -1;
    return -1;
  }
  return dd;
}

server_socket::~server_socket() {
  if (dd != -1) {
    close(dd);
  }
}
