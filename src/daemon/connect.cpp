#include <cstdlib>
#include <iostream>
#include <string>

std::string get_hpath() {
  const char *xdg_dir = std::getenv("XDG_RUNTIME_DIR");
  const char *his = std::getenv("HYPRLAND_INSTANCE_SIGNATURE");

  if (!xdg_dir || !his) {
    std::cerr
        << "Error: XDG_RUNTIME_DIR or HYPRLAND_INSTANCE_SIGNATURE not set !"
        << std::endl;
    exit(1);
  }
  std::string path =
      std::string(xdg_dir) + "/hypr/" + std::string(his) + "/.socket2.sock";

  return path;
}

std::string get_dpath() {
  const char *xdg_dir = std::getenv("XDG_RUNTIME_DIR");
  if (!xdg_dir) {
    std::cerr << "Error : XDG_RUNTIME_DIR [DAEMON]" << "\n";
    exit(1);
  }
  std::string path = std::string(xdg_dir) + "/hyprwatch.sock";
  return path;
}
