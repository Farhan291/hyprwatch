#pragma once

#include <string>

class unixsocket {
public:
  explicit unixsocket(const std::string &path);

  unixsocket(const unixsocket &) = delete;
  unixsocket &operator=(const unixsocket &) = delete;

  int create_socket();

  ~unixsocket();

private:
  int sd;
  std::string path;
};
