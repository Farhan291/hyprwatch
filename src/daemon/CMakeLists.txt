add_executable(hyprwatchd
client_socket.cpp
main.cpp
connect.cpp
parse.cpp
server_socket.cpp
time_track.cpp
serialize.cpp
)

install(TARGETS hyprwatchd RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# Configure systemd service file with install paths
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/hyprwatchd.service.in
  ${CMAKE_CURRENT_BINARY_DIR}/hyprwatchd.service
  @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/hyprwatchd.service
  DESTINATION lib/systemd/user
)

