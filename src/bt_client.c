#include "bt_client.h"

#if 0
#define DEBUG_LOG(...) printf(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

typedef struct {
  bool is_turn;
  bool turn_complete;
  uint8_t last_move;
  uint8_t move;
} player_t;

extern player_t player_temp;

btstack_packet_callback_registration_t client_hci_event_callback_registration;
gc_state_t state = TC_OFF;
bd_addr_t server_addr;
bd_addr_type_t server_addr_type;
hci_con_handle_t connection_handle;
gatt_client_service_t server_service;
gatt_client_characteristic_t server_characteristic;
bool listener_registered;
gatt_client_notification_t notification_listener;
btstack_timer_source_t client_heartbeat;

bool advertisement_report_contains_service(uint16_t service,
                                           uint8_t* advertisement_report) {
  // get advertisement from report event
  const uint8_t* adv_data =
      gap_event_advertising_report_get_data(advertisement_report);
  uint8_t adv_len =
      gap_event_advertising_report_get_data_length(advertisement_report);

  // iterate over advertisement data
  ad_context_t context;
  for (ad_iterator_init(&context, adv_len, adv_data);
       ad_iterator_has_more(&context); ad_iterator_next(&context)) {
    uint8_t data_type = ad_iterator_get_data_type(&context);
    uint8_t data_size = ad_iterator_get_data_len(&context);
    const uint8_t* data = ad_iterator_get_data(&context);
    switch (data_type) {
      case BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS:
        for (int i = 0; i < data_size; i += 2) {
          uint16_t type = little_endian_read_16(data, i);
          if (type == service) return true;
        }
      default:
        break;
    }
  }
  return false;
}

void handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                              uint8_t* packet, uint16_t size) {
  UNUSED(packet_type);
  UNUSED(channel);
  UNUSED(size);

  uint8_t att_status;
  switch (state) {
    case TC_W4_SERVICE_RESULT:
      switch (hci_event_packet_get_type(packet)) {
        case GATT_EVENT_SERVICE_QUERY_RESULT:
          // store service (we expect only one)
          DEBUG_LOG("Storing service\n");
          gatt_event_service_query_result_get_service(packet, &server_service);
          break;
        case GATT_EVENT_QUERY_COMPLETE:
          att_status = gatt_event_query_complete_get_att_status(packet);
          if (att_status != ATT_ERROR_SUCCESS) {
            printf("SERVICE_QUERY_RESULT, ATT Error 0x%02x.\n", att_status);
            gap_disconnect(connection_handle);
            break;
          }
          // service query complete, look for characteristic
          state = TC_W4_CHARACTERISTIC_RESULT;
          DEBUG_LOG("Search for env sensing characteristic.\n");
          gatt_client_discover_characteristics_for_service_by_uuid16(
              handle_gatt_client_event, connection_handle, &server_service,
              ORG_BLUETOOTH_CHARACTERISTIC_TEMPERATURE);
          break;
        default:
          break;
      }
      break;
    case TC_W4_CHARACTERISTIC_RESULT:
      switch (hci_event_packet_get_type(packet)) {
        case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
          DEBUG_LOG("Storing characteristic\n");
          gatt_event_characteristic_query_result_get_characteristic(
              packet, &server_characteristic);
          break;
        case GATT_EVENT_QUERY_COMPLETE:
          att_status = gatt_event_query_complete_get_att_status(packet);
          if (att_status != ATT_ERROR_SUCCESS) {
            printf("CHARACTERISTIC_QUERY_RESULT, ATT Error 0x%02x.\n",
                   att_status);
            gap_disconnect(connection_handle);
            break;
          }
          // register handler for notifications
          listener_registered = true;
          gatt_client_listen_for_characteristic_value_updates(
              &notification_listener, handle_gatt_client_event,
              connection_handle, &server_characteristic);
          // enable notifications
          DEBUG_LOG("Enable notify on characteristic.\n");
          state = TC_W4_ENABLE_NOTIFICATIONS_COMPLETE;
          gatt_client_write_client_characteristic_configuration(
              handle_gatt_client_event, connection_handle,
              &server_characteristic,
              GATT_CLIENT_CHARACTERISTICS_CONFIGURATION_NOTIFICATION);
          break;
        default:
          break;
      }
      break;
    case TC_W4_ENABLE_NOTIFICATIONS_COMPLETE:
      switch (hci_event_packet_get_type(packet)) {
        case GATT_EVENT_QUERY_COMPLETE:
          DEBUG_LOG("Notifications enabled, ATT status 0x%02x\n",
                    gatt_event_query_complete_get_att_status(packet));
          if (gatt_event_query_complete_get_att_status(packet) !=
              ATT_ERROR_SUCCESS)
            break;
          state = TC_W4_READY;
          break;
        default:
          break;
      }
      break;
    case TC_W4_READY:
      switch (hci_event_packet_get_type(packet)) {
        case GATT_EVENT_NOTIFICATION: {
          uint16_t value_length =
              gatt_event_notification_get_value_length(packet);
          const uint8_t* value = gatt_event_notification_get_value(packet);
          DEBUG_LOG("Indication value len %d\n", value_length);
          if (value_length == 1) {
            player_temp.move = little_endian_read_16(value, 0);
            player_temp.is_turn = true;
            printf("Receive %zu\n", player_temp.move);
          } else {
            printf("Unexpected length %d\n", value_length);
          }
          break;
        }
        default:
          printf("Unknown packet type 0x%02x\n",
                 hci_event_packet_get_type(packet));
          break;
      }
      break;
    default:
      printf("error\n");
      break;
  }
}

void client_start(void) {
  DEBUG_LOG("Start scanning!\n");
  state = TC_W4_SCAN_RESULT;
  gap_set_scan_parameters(0, 0x0030, 0x0030);
  gap_start_scan();
}

void hci_event_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                       uint16_t size) {
  UNUSED(size);
  UNUSED(channel);
  bd_addr_t local_addr;
  if (packet_type != HCI_EVENT_PACKET) return;

  uint8_t event_type = hci_event_packet_get_type(packet);
  switch (event_type) {
    case BTSTACK_EVENT_STATE:
      if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING) {
        gap_local_bd_addr(local_addr);
        printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
        client_start();
      } else {
        state = TC_OFF;
      }
      break;
    case GAP_EVENT_ADVERTISING_REPORT:
      if (state != TC_W4_SCAN_RESULT) return;
      // check name in advertisement
      if (!advertisement_report_contains_service(
              ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING, packet))
        return;
      // store address and type
      gap_event_advertising_report_get_address(packet, server_addr);
      server_addr_type = gap_event_advertising_report_get_address_type(packet);
      // stop scanning, and connect to the device
      state = TC_W4_CONNECT;
      gap_stop_scan();
      printf("Connecting to device with addr %s.\n",
             bd_addr_to_str(server_addr));
      gap_connect(server_addr, server_addr_type);
      break;
    case HCI_EVENT_LE_META:
      // wait for connection complete
      switch (hci_event_le_meta_get_subevent_code(packet)) {
        case HCI_SUBEVENT_LE_CONNECTION_COMPLETE:
          if (state != TC_W4_CONNECT) return;
          connection_handle =
              hci_subevent_le_connection_complete_get_connection_handle(packet);
          // initialize gatt client context with handle, and add it to the list
          // of active clients query primary services
          DEBUG_LOG("Search for env sensing service.\n");
          state = TC_W4_SERVICE_RESULT;
          gatt_client_discover_primary_services_by_uuid16(
              handle_gatt_client_event, connection_handle,
              ORG_BLUETOOTH_SERVICE_ENVIRONMENTAL_SENSING);
          break;
        default:
          break;
      }
      break;
    case HCI_EVENT_DISCONNECTION_COMPLETE:
      // unregister listener
      connection_handle = HCI_CON_HANDLE_INVALID;
      if (listener_registered) {
        listener_registered = false;
        gatt_client_stop_listening_for_characteristic_value_updates(
            &notification_listener);
      }
      printf("Disconnected %s\n", bd_addr_to_str(server_addr));
      if (state == TC_OFF) break;
      client_start();
      break;
    default:
      break;
  }
}
