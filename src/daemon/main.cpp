#include "client_socket.hpp"
#include "connect.hpp"
#include "parse.hpp"
#include "server_socket.hpp"
#include "time_track.hpp"
#include <iostream>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
int main() {
  struct Daemon_state state;
  std::string hpath = get_hpath();
  unixsocket hypr(hpath);

  std::string dpath = get_dpath();
  server_socket daemon(dpath);

  int hypr_fd = hypr.create_socket();
  if (hypr_fd < 0) {
    std::cerr << "Failed to connect to Hyprland\n";
    return 1;
  }

  int daemon_fd = daemon.create_socket();
  if (daemon_fd < 0) {
    std::cerr << "Failed to create IPC socket\n";
    return 1;
  }

  std::string buff;
  // https://www.man7.org/linux/man-pages/man2/poll.2.html
  // https://www.man7.org/linux/man-pages/man2/select.2.html
  pollfd fds[2];
  fds[0].fd = hypr_fd;
  fds[0].events = POLLIN;
  fds[0].revents = 0;
  fds[1].fd = daemon_fd;
  fds[1].events = POLLIN;
  fds[1].revents = 0;
  while (true) {
    int ret = poll(fds, 2, 0);
    if (ret < 0) {
      perror("poll");
      break;
    }
    // hyprland ipc connection data retrival(client)
    if (fds[0].revents & POLLIN) {
      char temp[1024];
      ssize_t n = read(hypr_fd, temp, sizeof(temp));
      if (n <= 0)
        break;
      buff.append(temp, n);
      while (true) {
        auto pos = buff.find('\n');
        if (pos == std::string::npos)
          break;
        std::string line = buff.substr(0, pos);
        buff.erase(0, pos + 1);
        auto split = line.find(">>");
        if (split == std::string::npos)
          continue;

        std::string event = line.substr(0, split);
        std::string payload = line.substr(split + 2);

        parse(event, payload, state);
      }
    }
    // hyprwatch daemon data writing (server)
    if (fds[1].revents & POLLIN) {
      int client = accept(daemon_fd, nullptr, nullptr);
      if (client >= 0) {
        flush_active_app(state);            // finalize time
        std::string out = serialize(state); // convert to text
        write(client, out.c_str(), out.size());
        close(client);
      }
    }
  }
}
