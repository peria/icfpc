peria_add_library(core communicator.cc game.cc protocol.cc punter.cc time.cc)

# ---------------------------------------------------------------------
function(peria_core_test target)
  peria_test(${target} ${ARGN})
  target_link_libraries(${target}_test peria_core)
endfunction()

peria_core_test(communicator communicator_test.cc)
peria_core_test(protocol protocol_test.cc)
