#include <stdio.h>

#include "bt_client.h"
#include "bt_server.h"
#include "hardware/structs/rosc.h"

typedef struct {
  bool turn;
  bool turn_complete;
  uint8_t last_move;
  uint8_t move;
} player_t;

#ifdef PLAYER_0
player_t player = {.turn = true,
                   .turn_complete = false,
                   .last_move = UINT8_MAX,
                   .move = UINT8_MAX};
#endif
#ifdef PLAYER_1
player_t player = {.turn = false,
                   .turn_complete = false,
                   .last_move = UINT8_MAX,
                   .move = UINT8_MAX};
#endif

uint8_t get_random_uint8_t(void) {
  uint8_t ret;
  for (size_t i = 0; i < 8; i++) {
    ret |= (rosc_hw->randombit) << i;
  }
  return ret;
}

static void server_heartbeat_handler(struct btstack_timer_source* ts) {
  // static uint32_t counter = 0;
  // counter++;

  if (player.turn) {
    player.move = get_random_uint8_t();
    player.turn_complete = true;
  }

  // Update the temp every 10s
  if (player.turn_complete) {
    // poll_temp(7);
    if (le_notification_enabled) {
      att_server_request_can_send_now_event(con_handle);
    }
  }

  // // Invert the led
  // static int led_on = true;
  // led_on = !led_on;
  // cyw43_arch_gpio_put(CYW43_WL_GPeIO_LED_PIN, led_on);

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
  server_hci_event_callback_registration.callback = &packet_handler;
  hci_add_event_handler(&server_hci_event_callback_registration);

  // register for ATT event
  att_server_register_packet_handler(packet_handler);

  // set one-shot btstack timer
  server_heartbeat.process = &server_heartbeat_handler;
  btstack_run_loop_set_timer(&server_heartbeat, HEARTBEAT_PERIOD_MS);
  btstack_run_loop_add_timer(&server_heartbeat);

  // Set up client
  gatt_client_init();

  client_hci_event_callback_registration.callback = &hci_event_handler;
  hci_add_event_handler(&client_hci_event_callback_registration);

  // set one-shot btstack timer
  client_heartbeat.process = &client_heartbeat_handler;
  btstack_run_loop_set_timer(&client_heartbeat, LED_SLOW_FLASH_DELAY_MS);
  btstack_run_loop_add_timer(&client_heartbeat);

  // turn on bluetooth!
  hci_power_control(HCI_POWER_ON);

  while (true) {
    // sleep_ms(1000);
  }
}
