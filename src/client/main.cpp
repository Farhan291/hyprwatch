#include "ipc_client.hpp"
#include "parse.hpp"
#include "ui.hpp"
#include <chrono>
#include <locale.h>
#include <ncurses.h>
#include <thread>
#include <unistd.h>

std::string get_dpath() {
  const char *xdg_dir = std::getenv("XDG_RUNTIME_DIR");
  if (!xdg_dir)
    return "/tmp/hyprwatch.sock";
  return std::string(xdg_dir) + "/hyprwatch.sock";
}

void socket_listener(app_state &state) {
  while (!state.should_exit) {
    unixsocket sock(get_dpath());
    int fd = sock.create_socket();

    if (fd < 0) {
      {
        std::lock_guard<std::mutex> lock(state.mutex);
        state.latest_message = "ACTIVE \nConnection failed";
      }
      sleep(1);
      continue;
    }

    char buf[4096];
    ssize_t n;
    while (!state.should_exit && (n = read(fd, buf, sizeof(buf) - 1)) > 0) {
      buf[n] = '\0';
      {
        std::lock_guard<std::mutex> lock(state.mutex);
        state.latest_message = std::string(buf);
      }
    }

    close(fd);
    sleep(1);
  }
}

int main() {
  app_state state;

  setlocale(LC_ALL, "");
  initscr();
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  curs_set(0);

  if (has_colors()) {
    initColors();
  }

  std::thread worker(socket_listener, std::ref(state));
  worker.detach();

  window_data currentState;

  while (!state.should_exit) {
    int ch = getch();
    if (ch == 'q' || ch == 'Q') {
      state.should_exit = true;
      break;
    }

    {
      std::lock_guard<std::mutex> lock(state.mutex);
      currentState = parser(state.latest_message);
    }

    renderUI(currentState);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  endwin();
  return 0;
}
