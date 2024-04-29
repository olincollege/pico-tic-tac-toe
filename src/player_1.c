#include <stdio.h>

#define PLAYER_1 1
#include "bt_client.h"
#include "bt_server.h"

bool turn = false;

static void server_heartbeat_handler(struct btstack_timer_source* ts) {
  // static uint32_t counter = 0;
  // counter++;

  // Update the temp every 10s
  if (turn) {
    poll_temp(19);
    if (le_notification_enabled) {
      att_server_request_can_send_now_event(con_handle);
    }
    turn = false;
  }

  // Invert the led
  static int led_on = true;
  led_on = !led_on;
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);

  // Restart timer
  btstack_run_loop_set_timer(ts, HEARTBEAT_PERIOD_MS);
  btstack_run_loop_add_timer(ts);
}

static void client_heartbeat_handler(struct btstack_timer_source* ts) {
  // Invert the led
  static bool quick_flash;
  static bool led_on = true;

  led_on = !led_on;
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
  if (listener_registered && led_on) {
    quick_flash = !quick_flash;
  } else if (!listener_registered) {
    quick_flash = false;
  }

  // Check if move is new, if so, now player 1's turn

  // Restart timer
  btstack_run_loop_set_timer(ts, (led_on || quick_flash)
                                     ? LED_QUICK_FLASH_DELAY_MS
                                     : LED_SLOW_FLASH_DELAY_MS);
  btstack_run_loop_add_timer(ts);
}

int main() {
  stdio_init_all();

  // initialize CYW43 driver architecture (will enable BT if/because
  // CYW43_ENABLE_BLUETOOTH == 1)
  if (cyw43_arch_init()) {
    printf("failed to initialise cyw43_arch\n");
    return -1;
  }

  l2cap_init();
  sm_init();
  sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);

  // setup empty ATT server - only needed if LE Peripheral does ATT queries on
  // its own, e.g. Android and iOS
  att_server_init(NULL, NULL, NULL);

  gatt_client_init();

  hci_event_callback_registration.callback = &hci_event_handler;
  hci_add_event_handler(&hci_event_callback_registration);

  // set one-shot btstack timer
  heartbeat.process = &client_heartbeat_handler;
  btstack_run_loop_set_timer(&heartbeat, LED_SLOW_FLASH_DELAY_MS);
  btstack_run_loop_add_timer(&heartbeat);

  // turn on!
  hci_power_control(HCI_POWER_ON);

  // btstack_run_loop_execute is only required when using the 'polling' method
  // (e.g. using pico_cyw43_arch_poll library). This example uses the
  // 'threadsafe background` method, where BT work is handled in a low priority
  // IRQ, so it is fine to call bt_stack_run_loop_execute() but equally you can
  // continue executing user code.

#if 1  // this is only necessary when using polling (which we aren't, but we're
       // showing it is still safe to call in this case)
  btstack_run_loop_execute();
#else
  // this core is free to do it's own stuff except when using 'polling' method
  // (in which case you should use btstacK_run_loop_ methods to add work to the
  // run loop.

  // this is a forever loop in place of where user code would go.
  // uint8_t new_temp = 118;
  // while (true) {
  //   gatt_client_write_value_of_characteristic(
  //       handle_gatt_client_event, connection_handle,
  //       ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING, 1, &new_temp);
  //   sleep_ms(1000);
  // }
#endif
  return 0;
}
