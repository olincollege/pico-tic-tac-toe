#pragma once

#include "btstack.h"
#include "btstack_config.h"
#include "pico/btstack_cyw43.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "player_0_gatt.h"
#include "player_1_gatt.h"

// Set gatt based on player
#ifdef PLAYER_0
#include "player_0_gatt.h"
#endif
#ifdef PLAYER_1
#include "player_1_gatt.h"
#endif

#define HEARTBEAT_PERIOD_MS 1000

// extern bool turn;
// extern bool turn_complete;
// extern uint8_t last_move;
// extern uint8_t move;

extern btstack_timer_source_t server_heartbeat;
extern btstack_packet_callback_registration_t
    server_hci_event_callback_registration;

#define APP_AD_FLAGS 0x06
static uint8_t adv_data[] = {
    // Flags general discoverable
    0x02,
    BLUETOOTH_DATA_TYPE_FLAGS,
    APP_AD_FLAGS,
    // Name
    0x17,
    BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME,
    'P',
    'i',
    'c',
    'o',
    ' ',
    '0',
    '0',
    ':',
    '0',
    '0',
    ':',
    '0',
    '0',
    ':',
    '0',
    '0',
    ':',
    '0',
    '0',
    ':',
    '0',
    '0',
    0x03,
    BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS,
    0x1a,
    0x18,
};
static const uint8_t adv_data_len = sizeof(adv_data);

extern int le_notification_enabled;
extern hci_con_handle_t con_handle;

void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t* packet,
                    uint16_t size);

uint16_t att_read_callback(hci_con_handle_t connection_handle,
                           uint16_t att_handle, uint16_t offset,
                           uint8_t* buffer, uint16_t buffer_size);

int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle,
                       uint16_t transaction_mode, uint16_t offset,
                       uint8_t* buffer, uint16_t buffer_size);

void poll_temp(uint8_t move);

// static void heartbeat_handler(struct btstack_timer_source* ts);
