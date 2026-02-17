#pragma once
#include <algorithm>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

// window data
struct app_data {
  std::string name;
  long long seconds;
  bool is_active;
};
// window data
struct window_data {
  std::string active_app;
  std::string active_title;
  std::vector<app_data> apps;
  long long total_seconds;
};

struct app_state {
  std::string latest_message = "Waiting for daemon";
  std::mutex mutex;
  bool should_exit = false;
};

window_data parser(const std::string &payload);
std::string formatDuration(long long seconds);
