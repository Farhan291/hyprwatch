#pragma once
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
inline const std::unordered_map<std::string, HYPR_EVENT> event_map = {
    {"workspace", HYPR_EVENT::WORKSPACE},
    {"activewindow", HYPR_EVENT::ACTIVE_WINDOW},
    {"openwindow", HYPR_EVENT::OPEN_WINDOW},
    {"closewindow", HYPR_EVENT::CLOSE_WINDOW},
    {"fullscreen", HYPR_EVENT::FULLSCREEN}};

void parse(const std::string &event, const std::string &payload);
