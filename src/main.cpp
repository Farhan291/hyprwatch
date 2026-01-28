#include "connect.hpp"
#include "parse.hpp"
#include "unix_socket.hpp"
#include <string>
#include <sys/types.h>
#include <unistd.h>
int main() {
  std::string path = get_path();
  unixsocket s(path);
  int sd = s.create_socket();
  if (sd < 0) {
    return 1;
  }

  std::string buff;
  while (true) {
    char temp[1024];
    ssize_t n = read(sd, temp, sizeof(temp) - 1);
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
      if (split == std::string::npos) {
        continue;
      }
      const std::string event = line.substr(0, split);
      const std::string payload = line.substr(split + 2);
      parse(event, payload);
    }
  }
}
