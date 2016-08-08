peria_add_library(server server.cc offline_server.cc client.cc)

# ---------------------------------------------------------------------
peria_add_executable(server server_main.cc)
target_link_libraries(server peria_server peria_core)
