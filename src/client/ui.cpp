// kimi k2.5 is used to generate the design and parts of ui code
// verfied by author::farhan

#include "ui.hpp"
#include <locale.h>
#include <ncurses.h>
#include <string>

void drawBox(int y, int x, int height, int width, int colorPair) {
  attron(COLOR_PAIR(colorPair));

  // add lines
  mvhline(y, x + 1, ACS_HLINE, width - 2);
  mvhline(y + height - 1, x + 1, ACS_HLINE, width - 2);
  mvvline(y + 1, x, ACS_VLINE, height - 2);
  mvvline(y + 1, x + width - 1, ACS_VLINE, height - 2);

  // add corners
  mvaddch(y, x, ACS_ULCORNER);
  mvaddch(y, x + width - 1, ACS_URCORNER);
  mvaddch(y + height - 1, x, ACS_LLCORNER);
  mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);

  attroff(COLOR_PAIR(colorPair));
}

void drawBar(int y, int x, int width, long long value, long long maxValue,
             int colorPair) {
  if (maxValue == 0)
    return;

  int barWidth = (value * width) / maxValue;
  if (barWidth > width)
    barWidth = width;

  attron(COLOR_PAIR(colorPair));
  for (int i = 0; i < barWidth; i++) {
    mvaddch(y, x + i, ACS_BLOCK);
  }
  attroff(COLOR_PAIR(colorPair));

  for (int i = barWidth; i < width; i++) {
    mvaddch(y, x + i, ' ');
  }
}

void drawHeader(int y, int x, int width, const std::string &activeApp,
                const std::string &activeTitle) {
  int height = activeTitle.empty() ? 5 : 6;
  drawBox(y, x, height, width, 1);

  attron(COLOR_PAIR(2) | A_BOLD);
  std::string title = " HYPRWATCH ";
  mvprintw(y + 1, x + (width - title.length()) / 2, "%s", title.c_str());
  attroff(COLOR_PAIR(2) | A_BOLD);

  if (!activeApp.empty()) {
    attron(COLOR_PAIR(2));
    std::string label = "Currently Active:";
    mvprintw(y + 2, x + 3, "%s", label.c_str());
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(6) | A_BOLD);
    std::string appDisplay = " " + activeApp + " ";
    mvprintw(y + 3, x + (width - appDisplay.length()) / 2, "%s",
             appDisplay.c_str());
    attroff(COLOR_PAIR(6) | A_BOLD);

    if (!activeTitle.empty()) {
      attron(COLOR_PAIR(3));
      // Truncate title if too long
      std::string titleDisplay = activeTitle;
      int maxTitleWidth = width - 6;
      if ((int)titleDisplay.length() > maxTitleWidth) {
        titleDisplay = titleDisplay.substr(0, maxTitleWidth - 3) + "...";
      }
      mvprintw(y + 4, x + (width - titleDisplay.length()) / 2, "%s",
               titleDisplay.c_str());
      attroff(COLOR_PAIR(3));
    }
  } else {
    attron(COLOR_PAIR(4));
    mvprintw(y + 3, x + (width - 20) / 2, " No Active Window ");
    attroff(COLOR_PAIR(4));
  }
}

void initColors() {
  start_color();
  use_default_colors();

  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_WHITE, -1);
  init_pair(3, COLOR_GREEN, -1);
  init_pair(4, COLOR_YELLOW, -1);
  init_pair(5, COLOR_CYAN, -1);
  init_pair(6, COLOR_BLACK, COLOR_GREEN);
  init_pair(7, COLOR_YELLOW, -1);
  init_pair(8, COLOR_BLUE, -1);
  init_pair(9, COLOR_MAGENTA, -1);
  init_pair(10, COLOR_RED, -1);
  init_pair(11, COLOR_GREEN, COLOR_BLACK);
  init_pair(12, COLOR_YELLOW, COLOR_BLACK);
  init_pair(13, COLOR_BLUE, COLOR_BLACK);
}

void drawStats(int y, int x, int width, const window_data &state) {
  int height = 4;
  drawBox(y, x, height, width, 3);

  attron(COLOR_PAIR(5));
  mvprintw(y + 1, x + 2, " Statistics ");

  std::string totalApps = std::to_string(state.apps.size());
  mvprintw(y + 2, x + 2, " Total Apps: %s", totalApps.c_str());

  std::string totalTime = formatDuration(state.total_seconds);
  int timeX = width - totalTime.length() - 15;
  mvprintw(y + 2, timeX, " Total Time: %s", totalTime.c_str());

  attroff(COLOR_PAIR(5));
}

void drawAppList(int y, int x, int height, int width,
                 const window_data &state) {
  drawBox(y, x, height, width, 2);

  attron(COLOR_PAIR(5) | A_BOLD);
  mvprintw(y + 1, x + 2, " Application Usage ");
  attroff(COLOR_PAIR(5) | A_BOLD);

  if (state.apps.empty()) {
    attron(COLOR_PAIR(4));
    mvprintw(y + 3, x + (width - 20) / 2, "No data available");
    attroff(COLOR_PAIR(4));
    return;
  }

  int startY = y + 3;
  int maxApps = height - 5;

  long long maxTime = state.apps.empty() ? 0 : state.apps[0].seconds;

  attron(COLOR_PAIR(7));
  mvprintw(startY - 1, x + 2, "%-20s %10s  %s", "Application", "Time",
           "Usage Graph");
  mvhline(startY, x + 1, ACS_HLINE, width - 2);
  attroff(COLOR_PAIR(7));

  for (size_t i = 0; i < state.apps.size() && i < (size_t)maxApps; i++) {
    const auto &app = state.apps[i];
    int row = startY + 1 + i;

    if (app.is_active) {
      attron(COLOR_PAIR(6) | A_BOLD);
      mvprintw(row, x + 2, "%-20s", app.name.c_str());
      attroff(COLOR_PAIR(6) | A_BOLD);
    } else {
      attron(COLOR_PAIR(2));
      mvprintw(row, x + 2, "%-20s", app.name.c_str());
      attroff(COLOR_PAIR(2));
    }

    attron(COLOR_PAIR(3));
    mvprintw(row, x + 25, "%10s", formatDuration(app.seconds).c_str());
    attroff(COLOR_PAIR(3));

    int barX = x + 38;
    int barWidth = width - barX - x - 2;
    if (barWidth > 5) {
      int color;
      if (app.is_active) {
        color = 6; // Active app - green background
      } else if (i == 0) {
        color = 12; // Longest bar - yellow
      } else if (i == 1) {
        color = 11; // Second longest - green
      } else {
        color = 13; // Others - blue
      }
      drawBar(row, barX, barWidth, app.seconds, maxTime, color);
    }
  }
}

void drawFooter(int y, int x, int width) {
  attron(COLOR_PAIR(4));
  std::string help = " Press 'q' to quit ";
  mvprintw(y, x + (width - help.length()) / 2, "%s", help.c_str());
  attroff(COLOR_PAIR(4));
}

void renderUI(const window_data &state) {
  clear();

  int maxY, maxX;
  getmaxyx(stdscr, maxY, maxX);

  if (maxX < 60 || maxY < 15) {
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(maxY / 2, (maxX - 25) / 2, "Terminal too small!");
    mvprintw(maxY / 2 + 1, (maxX - 30) / 2, "Please resize to 60x15+");
    attroff(COLOR_PAIR(4) | A_BOLD);
    refresh();
    return;
  }

  int marginX = 2;
  int marginY = 1;
  int contentWidth = maxX - 2 * marginX;

  drawHeader(marginY, marginX, contentWidth, state.active_app,
             state.active_title);

  int statsY = marginY + (state.active_title.empty() ? 5 : 6);
  drawStats(statsY, marginX, contentWidth, state);

  int listY = statsY + 4;
  int listHeight = maxY - listY - marginY - 1;
  if (listHeight > 3) {
    drawAppList(listY, marginX, listHeight, contentWidth, state);
  }

  drawFooter(maxY - 1, marginX, contentWidth);

  refresh();
}
