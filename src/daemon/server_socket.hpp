#pragma once

#include <string>
class server_socket {
private:
  int dd;
  std::string path;

public:
  explicit server_socket(const std::string &path);

  server_socket(const server_socket &) = delete;
  server_socket &operator=(const server_socket &) = delete;

  int create_socket();
  ~server_socket();
};
