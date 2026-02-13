#include "parse.hpp"

std::string formatDuration(long long seconds) {
  long long hours = seconds / 3600;
  long long mins = (seconds % 3600) / 60;
  long long secs = seconds % 60;
  std::stringstream ss;
  if (hours > 0) {
    ss << hours << "h " << std::setw(2) << std::setfill('0') << mins << "m";
  } else if (mins > 0) {
    ss << mins << "m " << std::setw(2) << std::setfill('0') << secs << "s";
  } else {
    ss << secs << "s";
  }
  return ss.str();
}

window_data parser(const std::string &payload) {
  window_data state;
  state.total_seconds = 0;
  std::istringstream iss(payload);
  std::string line;

  while (getline(iss, line)) {
    // trim whitespace logic
    const std::string whitespace = " \t\n\r\f\v";
    size_t start = line.find_first_not_of(whitespace);
    if (start == std::string::npos)
      continue;
    size_t end = line.find_last_not_of(whitespace);
    line = line.substr(start, end - start + 1);

    if (line.empty())
      continue;

    if (line.substr(0, 7) == "ACTIVE ") {
      std::string rest_string = line.substr(7);
      size_t delim_pos = rest_string.find('|');
      if (delim_pos != std::string::npos) {
        state.active_app = rest_string.substr(0, delim_pos);
        state.active_title = rest_string.substr(delim_pos + 1);
      } else {
        state.active_app = rest_string;
        state.active_title = "";
      }
    } else {
      size_t delim_pos = line.find_last_of(' ');
      if (delim_pos == std::string::npos)
        continue;
      app_data app;
      app.name = line.substr(0, delim_pos);
      try {
        long long secs = std::stoll(line.substr(delim_pos + 1));
        app.seconds = secs;
        app.is_active = false;
        state.apps.push_back(app);
        state.total_seconds += secs;
      } catch (...) {
        continue;
      }
    }
  }
  for (auto &app : state.apps) {
    if (app.name == state.active_app) {
      app.is_active = true;
    }
  }

  std::sort(state.apps.begin(), state.apps.end(),
            [](const app_data &a, const app_data &b) {
              return a.seconds > b.seconds;
            });

  return state;
}
