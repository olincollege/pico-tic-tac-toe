#pragma once

#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#if 0
#define DEBUG_LOG(...) printf(__VA_ARGS__)
#else
#define DEBUG_LOG(...)
#endif

#define LED_QUICK_FLASH_DELAY_MS 100
#define LED_SLOW_FLASH_DELAY_MS 1000

typedef enum {
  TC_OFF,
  TC_IDLE,
  TC_W4_SCAN_RESULT,
  TC_W4_CONNECT,
  TC_W4_SERVICE_RESULT,
  TC_W4_CHARACTERISTIC_RESULT,
  TC_W4_ENABLE_NOTIFICATIONS_COMPLETE,
  TC_W4_READY
} gc_state_t;

extern btstack_packet_callback_registration_t
    client_hci_event_callback_registration;
extern gc_state_t state;
extern bd_addr_t server_addr;
extern bd_addr_type_t server_addr_type;
extern hci_con_handle_t connection_handle;
extern gatt_client_service_t server_service;
extern gatt_client_characteristic_t server_characteristic;
extern bool listener_registered;
extern gatt_client_notification_t notification_listener;
extern btstack_timer_source_t client_heartbeat;

/**
 * Determine if an advertised Bluetooth connection contains a particular
 * service.
 *
 * @param service A Bluetooth service.
 * @param advertisement_report The report of a advertising Bluetooth device.
 * @return True if the advertisment contains a particular service, false
 * otherwise.
 */
bool advertisement_report_contains_service(uint16_t service,
                                           uint8_t* advertisement_report);

/**
 * Handle a Bluetooth characteristic change sent by a Bluetooth server.
 *
 * @param packet_type The packet type to receive.
 * @param channel UNUSED
 * @param packet A buffer to store the packet.
 * @param size UNUSED
 */
void handle_gatt_client_event(uint8_t packet_type, uint16_t channel,
                              uint8_t* packet, uint16_t size);

/**
 * Start the client and start scanning for a particular Bluetooth device.
 */
void client_start(void);

/**
 * Handle all all client-related events.
 *
 * @param packet_type The packet type to accept.
 * @param channel UNUSED
 * @param packet A buffer to accept a packet.
 * @param size UNUSED
 */
void hci_event_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                       uint16_t size);
