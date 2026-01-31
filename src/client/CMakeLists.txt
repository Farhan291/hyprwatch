add_executable(hyprwatch
  main.cpp
  ipc_client.cpp
)

install(TARGETS hyprwatch
  RUNTIME DESTINATION bin
)

