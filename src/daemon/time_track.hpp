#pragma once

#include <iostream>
#include <unordered_map>
struct Daemon_state {
  std::string active_window_class;
  std::string active_window_title;
  time_t last_switch_time;
  std::unordered_map<std::string, long long> window_time_seconds;
};

void on_focus_change(const std::string &new_clas, const std::string &new_title,
                     Daemon_state &s);
void flush_active_app(Daemon_state &s);
std::string serialize(const Daemon_state &s);
