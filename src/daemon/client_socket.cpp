#include "client_socket.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

unixsocket::unixsocket(const std::string &path) : sd(-1), path(path) {}

int unixsocket::create_socket() {
  sd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sd < 0) {
    std::cerr << "socket()" << std::endl;
    return -1;
  }

  struct sockaddr_un addr = {0};
  addr.sun_family = AF_UNIX;
  std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

  if (connect(sd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    std::cerr << "Failed to connect to at: " << path << std::endl;
    close(sd);
    sd = -1;
    return -1;
  }
  return sd;
}

unixsocket::~unixsocket() {
  if (sd != -1) {
    close(sd);
  }
}
