#include "serialize.hpp"
#include <string>

std::string serialize(const Daemon_state &s) {
  std::string out;

  out += "ACTIVE " + s.active_window_class + "|" + s.active_window_title + "\n";

  for (const auto &[app, seconds] : s.window_time_seconds) {
    out += app + " " + std::to_string(seconds) + "\n";
  }

  return out;
}
