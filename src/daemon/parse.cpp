#include "parse.hpp"
#include "time_track.hpp"
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <unordered_map>

HYPR_EVENT hash_event(const std::string &name) {
  auto it = event_map.find(name);
  if (it != event_map.end()) {
    return it->second;
  }
  return HYPR_EVENT::IGNORE;
}

void handle_active(const std::string &payload, Daemon_state &s) {
  if (payload.empty() || payload == ",") {
    time_t now = time(nullptr);
    time_t delta = now - s.last_switch_time;
    if (!s.active_window_class.empty()) {
      s.window_time_seconds[s.active_window_class] += delta;
    }
    s.active_window_class.clear();
    s.last_switch_time = now;
    return;
  }

  auto split = payload.find(',');
  std::string window_class = payload.substr(0, split);
  on_focus_change(window_class, s);
}
void handle_open(const std::string &payload) {
  auto split1 = payload.find(',');
  auto split2 = payload.find(',', split1 + 1);
  auto split3 = payload.find(',', split2 + 1);
  std::string window_address = payload.substr(0, split1);
  std::string windowspace_name =
      payload.substr(split1 + 1, split2 - split1 - 1);
  std::string window_class = payload.substr(split2 + 1, split3 - split2 - 1);
  std::string window_title = payload.substr(split3 + 1);
  /*
    std::cout << "[OPEN] " << "[WA]" << window_address << " [WS_N] "
              << windowspace_name << " [class] " << window_class << " [title] "
              << window_title << "\n";*/
}

void parse(const std::string &event, const std::string &payload,
           Daemon_state &s) {
  HYPR_EVENT event_id = hash_event(event);
  switch (event_id) {
  case HYPR_EVENT::ACTIVE_WINDOW:
    handle_active(payload, s);
    break;

  case HYPR_EVENT::WORKSPACE:
    // some plan later with this info
    // std::cout << "[WS SWITCH] " << payload << "\n";
    break;
  case HYPR_EVENT::OPEN_WINDOW:
    // handle_open(payload);
    break;
  case HYPR_EVENT::CLOSE_WINDOW:
    // std::cout << "[CLOSE] " << payload << "\n";
    break;
  case HYPR_EVENT::FULLSCREEN:
    // std::cout << "[FULLSCREEN] " << payload << "\n";
    break;
  case HYPR_EVENT::IGNORE:
    break;
  }
}
