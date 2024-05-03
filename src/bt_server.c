#include "bt_server.h"

#include <stdio.h>

#include "btstack.h"
#include "btstack_config.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

typedef struct {
  bool is_turn;
  bool turn_complete;
  bool incoming_turn;
  uint8_t last_move;
  uint8_t move;
} player_t;

extern player_t _player;

hci_con_handle_t con_handle;
int le_notification_enabled;
btstack_timer_source_t server_heartbeat;
btstack_packet_callback_registration_t server_hci_event_callback_registration;

void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                    uint16_t size) {
  UNUSED(size);
  UNUSED(channel);
  bd_addr_t local_addr;
  if (packet_type != HCI_EVENT_PACKET) return;

  uint8_t event_type = hci_event_packet_get_type(packet);
  switch (event_type) {
    case BTSTACK_EVENT_STATE:
      if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
      gap_local_bd_addr(local_addr);
      printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));

      // setup advertisements
      uint16_t adv_int_min = 800;
      uint16_t adv_int_max = 800;
      uint8_t adv_type = 0;
      bd_addr_t null_addr;
      memset(null_addr, 0, 6);
      gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0,
                                    null_addr, 0x07, 0x00);
      assert(adv_data_len <= 31);  // ble limitation
      gap_advertisements_set_data(adv_data_len, (uint8_t*)adv_data);
      gap_advertisements_enable(1);

      break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
      le_notification_enabled = 0;
      break;
    case ATT_EVENT_CAN_SEND_NOW:
      printf("Send %zu\n", _player.move);
      _player.last_move = _player.move;
      _player.is_turn = false;
      _player.turn_complete = false;
      att_server_notify(
          con_handle,
          ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_VALUE_HANDLE,
          (uint8_t*)&_player.move, sizeof(_player.move));
      break;
    default:
      break;
  }
}

uint16_t att_read_callback(hci_con_handle_t connection_handle,
                           uint16_t att_handle, uint16_t offset,
                           uint8_t* buffer, uint16_t buffer_size) {
  UNUSED(connection_handle);

  if (att_handle ==
      ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_VALUE_HANDLE) {
    return att_read_callback_handle_blob((const uint8_t*)&_player.move,
                                         sizeof(_player.move), offset, buffer,
                                         buffer_size);
  }
  return 0;
}

int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle,
                       uint16_t transaction_mode, uint16_t offset,
                       uint8_t* buffer, uint16_t buffer_size) {
  UNUSED(transaction_mode);
  UNUSED(offset);
  UNUSED(buffer_size);

  if (att_handle !=
      ATT_CHARACTERISTIC_ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE_01_CLIENT_CONFIGURATION_HANDLE)
    return 0;
  le_notification_enabled =
      little_endian_read_16(buffer, 0) ==
      GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION;
  con_handle = connection_handle;
  if (le_notification_enabled) {
    att_server_request_can_send_now_event(con_handle);
  }
  return 0;
}
