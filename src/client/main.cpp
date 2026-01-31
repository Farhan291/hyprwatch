#include "ipc_client.hpp"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

std::string get_dpath() {
  const char *xdg_dir = std::getenv("XDG_RUNTIME_DIR");
  if (!xdg_dir) {
    std::cerr << "Error : XDG_RUNTIME_DIR [DAEMON]" << "\n";
    exit(1);
  }
  std::string path = std::string(xdg_dir) + "/hyprwatch.sock";
  return path;
}

int main() {
  while (true) {
    unixsocket sock(get_dpath());
    int fd = sock.create_socket();
    if (fd < 0) {
      std::cerr << "Failed to connect to daemon\n";
      sleep(1);
      continue;
    }

    char buf[1024];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
      system("clear");
      write(1, buf, n);
    }

    sleep(1);
  }
}
