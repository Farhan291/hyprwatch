add_executable(hyprwatch
  main.cpp
  ipc_client.cpp
  parse.cpp
  ui.cpp
)

install(TARGETS hyprwatch
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

find_package(Curses REQUIRED)

target_link_libraries(hyprwatch
  PRIVATE
    ${CURSES_LIBRARIES}
)
