#include "parse.hpp"
#include <iostream>
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

void handle_active(const std::string &payload) {
  if (payload.empty() || payload == ",") {
    std::cout << "[FOCUS]  (Empty Workspace)\n";
    return;
  }
  auto split = payload.find(',');
  std::string window_class = payload.substr(0, split);
  std::string window_title = payload.substr(split + 1);
  std::cout << "[FOCUS] " << "[class] " << window_class << " [titles] "
            << window_title << "\n";
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

  std::cout << "[OPEN] " << "[WA]" << window_address << " [WS_N] "
            << windowspace_name << " [class] " << window_class << " [title] "
            << window_title << "\n";
}

void parse(const std::string &event, const std::string &payload) {
  HYPR_EVENT event_id = hash_event(event);
  switch (event_id) {
  case HYPR_EVENT::ACTIVE_WINDOW:
    handle_active(payload);
    break;
  case HYPR_EVENT::WORKSPACE:
    std::cout << "[WS SWITCH] " << payload << "\n";
    break;
  case HYPR_EVENT::OPEN_WINDOW:
    handle_open(payload);
    break;
  case HYPR_EVENT::CLOSE_WINDOW:
    std::cout << "[CLOSE] " << payload << "\n";
    break;
  case HYPR_EVENT::FULLSCREEN:
    std::cout << "[FULLSCREEN] " << payload << "\n";
    break;
  case HYPR_EVENT::IGNORE:
    break;
  }
}
