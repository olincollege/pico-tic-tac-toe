#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bt_client.h"
#include "bt_server.h"
#include "hardware/structs/rosc.h"
#include "keypad_wrapper.h"
#include "pico/stdlib.h"
#include "tic_tac_toe.h"

typedef struct {
  bool is_turn;
  bool turn_complete;
  bool incoming_turn;
  uint8_t last_move;
  uint8_t move;
} player_t;

#ifdef PLAYER_0
player_t _player = {.is_turn = true,
                    .turn_complete = false,
                    .incoming_turn = false,
                    .last_move = UINT8_MAX,
                    .move = UINT8_MAX};
#endif
#ifdef PLAYER_1
player_t _player = {.is_turn = false,
                    .turn_complete = false,
                    .incoming_turn = false,
                    .last_move = UINT8_MAX,
                    .move = UINT8_MAX};
#endif

static uint8_t get_random_uint8_t(void) {
  uint8_t ret;
  for (size_t i = 0; i < 8; i++) {
    ret |= (rosc_hw->randombit) << i;
  }
  return ret;
}

static void server_heartbeat_handler(struct btstack_timer_source* ts) {
  // Send a player's turn if it is complete
  if (_player.turn_complete) {
    if (le_notification_enabled) {
      att_server_request_can_send_now_event(con_handle);
    }
  }

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

  // Restart timer
  btstack_run_loop_set_timer(ts, (led_on || quick_flash)
                                     ? LED_QUICK_FLASH_DELAY_MS
                                     : LED_SLOW_FLASH_DELAY_MS);
  btstack_run_loop_add_timer(ts);
}

int main() {
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

  // game setup
  board* gameboard = malloc(sizeof(board));
  player* player1 = malloc(sizeof(player));
  player* player2 = malloc(sizeof(player));
  make_board(gameboard);
  make_player(player1, 1, 1);
  make_player(player2, 2, 0);
  int game_over = 0;  // 0 if not over, 1 if p1 wins, 2 in p2 wins, 3 if tie
  // keypad setup
  init_keypad();
  set_brightness(0.1f);  // this thing is *bright*
  set_board(gameboard->spaces);
  uint16_t last_button_states = 0;

  while (true) {
    if (game_over == 0) {
      // get button input
      button_coords button = get_button_press(&last_button_states);
      if (button.exit_code == 0) {
        if (is_space_valid(gameboard, button.x, button.y)) {
          if (player1->isturn) {
#ifdef PLAYER_0
            make_move(player1, gameboard, button.x, button.y);
            _player.turn_complete = true;
            _player.move = button.x + button.y * 4;
#endif
          } else if (player2->isturn) {
#ifdef PLAYER_1
            make_move(player2, gameboard, button.x, button.y);
            _player.turn_complete = true;
            _player.move = button.x + button.y * 4;
#endif
          }
          set_board(gameboard->spaces);
          // check for win
          if (player_win(player1, gameboard)) {
            game_over = 1;
          } else if (player_win(player2, gameboard)) {
            game_over = 2;
          } else if (is_full(gameboard)) {
            game_over = 3;
#ifdef PLAYER_0
          } else if (player1->isturn) {
            next_turn(player1, player2);
            _player.is_turn = false;
#endif
#ifdef PLAYER_1
          } else if (player2->isturn) {
            next_turn(player1, player2);
            _player.is_turn = false;
#endif
          }
        }
      }
      if ((_player.incoming_turn) && (_player.move < 16)) {
        // printf("Incoming dbg: %i\n", _player.incoming_turn);
        // printf("Incoming value: %i\n", _player.move);
#ifdef PLAYER_0
        make_move(player2, gameboard, _player.move % 4,
                  (_player.move - _player.move % 4) / 4);
#endif
#ifdef PLAYER_1
        make_move(player1, gameboard, _player.move % 4,
                  (_player.move - _player.move % 4) / 4);
#endif
        set_board(gameboard->spaces);
        // check for win
        if (player_win(player1, gameboard)) {
          game_over = 1;
        } else if (player_win(player2, gameboard)) {
          game_over = 2;
        } else if (is_full(gameboard)) {
          game_over = 3;
#ifdef PLAYER_0
        } else if (player2->isturn) {
          next_turn(player1, player2);
          _player.is_turn = true;
#endif
#ifdef PLAYER_1
        } else if (player1->isturn) {
          next_turn(player1, player2);
          _player.is_turn = true;
#endif
        }

        _player.incoming_turn = false;
      }
    } else {
      // game over
      // turn full board to winning color
      if (game_over == 1) {
        set_full_board(255, 0, 0);
      } else if (game_over == 2) {
        set_full_board(0, 0, 255);
      } else if (game_over == 3) {
        sleep_ms(500);  // see the last move
        set_full_board(0, 255, 0);
      }
      // pause for a sec
      sleep_ms(1000);
      // set board back to normal

      // reset game
      make_board(gameboard);
      set_board(gameboard->spaces);
      make_player(player1, 1, 1);
      make_player(player2, 2, 0);
      game_over = 0;
    }
  }
  return 0;
}
