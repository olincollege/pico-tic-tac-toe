#include <stdio.h>

#include "bt_server.h"

static btstack_timer_source_t heartbeat;
static btstack_packet_callback_registration_t hci_event_callback_registration;

static void heartbeat_handler(struct btstack_timer_source* ts) {
  // static uint32_t counter = 0;
  // counter++;

  // Update the temp every 10s
  if (turn) {
    poll_temp(7);
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

int main(void) {
  sleep_ms(1000);  // Wait before init so serial dbg can connect first
  // Initalized io via uart
  stdio_init_all();

  // initialize CYW43 driver architecture (will enable BT if/because
  // CYW43_ENABLE_BLUETOOTH == 1)
  if (cyw43_arch_init()) {
    printf("failed to initialise cyw43_arch\n");
    return -1;
  }

  l2cap_init();
  sm_init();

  att_server_init(profile_data, att_read_callback, att_write_callback);

  // inform about BTstack state
  hci_event_callback_registration.callback = &packet_handler;
  hci_add_event_handler(&hci_event_callback_registration);

  // register for ATT event
  att_server_register_packet_handler(packet_handler);

  // set one-shot btstack timer
  heartbeat.process = &heartbeat_handler;
  btstack_run_loop_set_timer(&heartbeat, HEARTBEAT_PERIOD_MS);
  btstack_run_loop_add_timer(&heartbeat);

  // turn on bluetooth!
  hci_power_control(HCI_POWER_ON);

  while (true) {
    // sleep_ms(1000);
  }
}
