peria_add_library(core communicator.cc game.cc map.cc punter.cc time.cc)

# ---------------------------------------------------------------------
peria_test(communicator communicator_test.cc)
target_link_libraries(communicator_test peria_core)
