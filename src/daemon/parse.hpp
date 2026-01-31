#pragma once
#include "time_track.hpp"
#include <string>
#include <unordered_map>

enum class HYPR_EVENT {
  WORKSPACE,
  ACTIVE_WINDOW,
  OPEN_WINDOW,
  CLOSE_WINDOW,
  FULLSCREEN,
  IGNORE
};
struct ActiveWindowState {
  std::string window_class;
  std::string window_title;
};

struct GlobalState {
  ActiveWindowState active;
  std::string current_workspace;
};

inline const std::unordered_map<std::string, HYPR_EVENT> event_map = {
    {"workspace", HYPR_EVENT::WORKSPACE},
    {"activewindow", HYPR_EVENT::ACTIVE_WINDOW},
    {"openwindow", HYPR_EVENT::OPEN_WINDOW},
    {"closewindow", HYPR_EVENT::CLOSE_WINDOW},
    {"fullscreen", HYPR_EVENT::FULLSCREEN}};

void handle_active(const std::string &payload, Daemon_state &s);

void parse(const std::string &event, const std::string &payload,
           Daemon_state &s);
