#include "time_track.hpp"

void on_focus_change(const std::string &new_class, const std::string &new_title,
                     Daemon_state &s) {
  time_t now = time(nullptr);

  if (s.last_switch_time == 0) {
    s.active_window_class = new_class;
    s.active_window_title = new_title;
    s.last_switch_time = now;
    return;
  }

  if (!s.active_window_class.empty()) {
    s.window_time_seconds[s.active_window_class] += (now - s.last_switch_time);
  }
  s.active_window_class = new_class;
  s.active_window_title = new_title;
  s.last_switch_time = now;
}

void flush_active_app(Daemon_state &s) {
  if (!s.active_window_class.empty()) {
    time_t now = time(nullptr);
    s.window_time_seconds[s.active_window_class] += (now - s.last_switch_time);
    s.last_switch_time = now;
  }
}
