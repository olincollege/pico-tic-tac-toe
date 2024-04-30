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

# gatt libs
add_library(player_0_gatt
    player_0_gatt.c
)
add_library(player_1_gatt
    player_1_gatt.c
)

# bt libs
add_library(bt_server
    bt_server.c
)
add_library(bt_client
    bt_client.c
)

# gatt header generation
# pico_btstack_make_gatt_header(bt_server PRIVATE "${CMAKE_CURRENT_LIST_DIR}/player_0_gatt.gatt")
# pico_btstack_make_gatt_header(bt_server PRIVATE "${CMAKE_CURRENT_LIST_DIR}/player_1_gatt.gatt")

# link deps to bt libs
target_link_libraries(bt_server
    pico_stdlib
    pico_btstack_ble
    pico_cyw43_arch_none
    pico_btstack_cyw43
    player_0_gatt
    player_1_gatt
)
target_link_libraries(bt_client
    pico_stdlib
    pico_btstack_cyw43
    pico_cyw43_arch_none
    pico_btstack_ble
)

# For btstack_config.h
target_include_directories(bt_server PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)
target_include_directories(bt_client PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Player 0
add_executable(player_0
    pico_tic_tac_toe.c
)

target_link_libraries(player_0
  pico_stdlib
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  bt_server
  player_0_gatt
  bt_client
)

# For btstack_config.h
target_include_directories(player_0 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)
# Set pico as player 0
target_compile_definitions(player_0 PRIVATE
    RUNNING_AS_CLIENT=1
    PLAYER_0
)

# Debug printing over usb
pico_enable_stdio_usb(player_0 1)
pico_enable_stdio_uart(player_0 0)

pico_add_extra_outputs(player_0)

# Player 1
add_executable(player_1
pico_tic_tac_toe.c
)

target_link_libraries(player_1
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    bt_client
    player_1_gatt
    bt_server
    )
target_include_directories(player_1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )


# Set pico as player 0
target_compile_definitions(player_1 PRIVATE
    RUNNING_AS_CLIENT=1
    PLAYER_1
)

# Debug printing over usb
pico_enable_stdio_usb(player_1 1)
pico_enable_stdio_uart(player_1 0)

pico_add_extra_outputs(player_1)
