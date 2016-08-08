add_library(peria_ai ai.cc)

peria_add_executable(punter punter_main.cc)
target_link_libraries(punter peria_core peria_ai)
