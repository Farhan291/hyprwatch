#pragma once

#include "parse.hpp"
#include <string>

// Initialize ncurses colors
void initColors();

// Draw a box at specified position with given dimensions
void drawBox(int y, int x, int height, int width, int colorPair);

// Draw a progress bar
void drawBar(int y, int x, int width, long long value, long long maxValue,
             int colorPair);

// Draw header section showing active app and title
void drawHeader(int y, int x, int width, const std::string &activeApp,
                const std::string &activeTitle);

// Draw statistics section
void drawStats(int y, int x, int width, const window_data &state);

// Draw application usage list
void drawAppList(int y, int x, int height, int width, const window_data &state);

// Draw footer with help text
void drawFooter(int y, int x, int width);

// Main render function
void renderUI(const window_data &state);
