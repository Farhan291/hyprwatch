#include "connect.hpp"
#include "unix_socket.hpp"
#include <iostream>
#include <string>
int main() {
  std::string path = get_path();
  unixsocket s(path);
  int sd = s.create_socket();
  std::cout << sd << path << std::endl;
}
