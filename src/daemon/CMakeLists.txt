add_executable(hyprwatchd
client_socket.cpp
main.cpp
connect.cpp
parse.cpp
server_socket.cpp
time_track.cpp
)

install(TARGETS hyprwatchd RUNTIME DESTINATION bin )

