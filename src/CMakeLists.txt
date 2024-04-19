# If you have a program that's just a .c file and it has a main method, define
# an executable. If it has both a .c and .h file, make it a library. Also, link
# libraries appropriately here. Targets should be added in dependency order - if
# executable bar needs library foo, then you should do
#   add_library(foo foo.c foo.h)
#   add_executable(bar bar.c)
#   target_link_libraries(bar PRIVATE foo)
# or similar. The PRIVATE/INTERFACE/PUBLIC keyword will depend on whether the
# library is used only in function bodies (PRIVATE), only in function
# signatures/types (INTERFACE), or both (PUBLIC).

add_library(bt_server
    bt_server.c
)
target_link_libraries(bt_server
    pico_stdlib
)

add_executable(tic_tac_toe_main
    pico_main.c
)
target_link_libraries(tic_tac_toe_main
    bt_server
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
)

# For btstack_config.h
target_include_directories(tic_tac_toe_main PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)
target_compile_definitions(tic_tac_toe_main PRIVATE
  RUNNING_AS_CLIENT=1
)

pico_btstack_make_gatt_header(tic_tac_toe_main PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.gatt")

pico_add_extra_outputs(tic_tac_toe_main)
