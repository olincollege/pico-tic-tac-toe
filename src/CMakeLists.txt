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

add_executable(tic_tac_toe_main_server
    # pico_main.c 
    bt_server.c
)
target_link_libraries(tic_tac_toe_main_server
  pico_stdlib
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  hardware_adc
# bt_server
    # pico_stdlib
    # pico_btstack_ble
    # pico_btstack_cyw43
    # pico_cyw43_arch_none
    # hardware_flash
    # hardware_exception
    # hardware_adc


    # pico_cyw43_arch_poll
    # pico_stdlib
    # pico_btstack_ble
    # pico_btstack_cyw43
    # pico_cyw43_driver
    # hardware_flash
    # hardware_exception
    # rp2_common
)

# For btstack_config.h
target_include_directories(tic_tac_toe_main_server PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Debug printing over usb
target_compile_definitions(tic_tac_toe_main_server PRIVATE
  # PICO_DEFAULT_UART=0
  # PICO_DEFAULT_UART_TX_PIN=0
  # PICO_DEFAULT_UART_RX_PIN=1
)

pico_enable_stdio_usb(tic_tac_toe_main_server 1)
pico_enable_stdio_uart(tic_tac_toe_main_server 0)

set(BTSTACK_ROOT ${PICO_SDK_PATH}/lib/btstack)
set(BTSTACK_3RD_PARTY_PATH ${BTSTACK_ROOT}/3rd-party)

# List of examples from btstack
include(${BTSTACK_ROOT}/example/CMakeLists.txt)

pico_btstack_make_gatt_header(tic_tac_toe_main_server PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.gatt")

pico_add_extra_outputs(tic_tac_toe_main_server)




add_executable(tic_tac_toe_main_client
    bt_client.c
    )
target_link_libraries(tic_tac_toe_main_client
    pico_stdlib
    pico_btstack_ble
    pico_btstack_cyw43
    pico_cyw43_arch_none
    hardware_adc
    )
target_include_directories(tic_tac_toe_main_client PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )

# Debug printing over usb
# Set pico as client
target_compile_definitions(tic_tac_toe_main_client PRIVATE
    RUNNING_AS_CLIENT=1
    # PICO_DEFAULT_UART=0
    # PICO_DEFAULT_UART_TX_PIN=0
    # PICO_DEFAULT_UART_RX_PIN=1
)

pico_enable_stdio_usb(tic_tac_toe_main_client 1)
pico_enable_stdio_uart(tic_tac_toe_main_client 0)

pico_add_extra_outputs(tic_tac_toe_main_client)

