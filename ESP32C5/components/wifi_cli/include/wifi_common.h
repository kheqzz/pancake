#ifndef WIFI_COMMON_H
#define WIFI_COMMON_H

#include <stdint.h>
#include <stdbool.h>
#include "esp_wifi.h"
#include "esp_wifi_types.h"
#include "led_strip.h"

#ifdef __cplusplus
extern "C" {
#endif

// Version
#define JANOS_VERSION "0.5.2"

// Maximum limits
#define MAX_AP_CNT 64
#define MAX_SCAN_RESULTS 64
#define MAX_PROBES 200
#define MAX_CLIENTS_PER_AP 50
#define MAX_TARGET_BSSIDS 50
#define MAX_WHITELIST_ENTRIES 150
#define MAX_SNIFFER_APS 100
#define MAX_PROBE_REQUESTS 200
#define DNS_PORT 53
#define DNS_MAX_PACKET_SIZE 512
#define MAX_HTML_FILES 20
#define MAX_HTML_FILENAME 64

// GPIO pins
#define NEOPIXEL_GPIO 27
#define LED_COUNT 1
#define RMT_RES_HZ (10 * 1000 * 1000)

// GPS UART pins — GPIO13/14 are USB D-/D+ (flash + monitor port), GPIO23 is
// CTP_INT, and GPIO6 MUST NOT be used here: with a wire on GPIO6 the SD card
// stops responding (timeout at CMD59) while it mounts fine without it.
// GPIO15 is not usable at all on this board (immediate "Load access fault").
// Battery level comes from a MAX17048 over I2C (9/4), so no ADC pin is needed.
#define GPS_UART_NUM UART_NUM_1
#define GPS_TX_PIN 2
#define GPS_RX_PIN 3
#define GPS_BUF_SIZE 1024

// SD Card SPI pins — microSD slot is on the TFT module, so SCK/MOSI/MISO are the
// SAME wires as the LCD (LCD_CLK 10 / LCD_MOSI 8 / LCD_MISO 25). Only CS is extra.
#define SD_MISO_PIN 25
#define SD_MOSI_PIN 8
#define SD_CLK_PIN 10
#define SD_CS_PIN 24

// Application states
typedef enum {
    APP_STATE_IDLE = 0,
    APP_STATE_DEAUTH,
    APP_STATE_DEAUTH_EVIL_TWIN,
    APP_STATE_EVIL_TWIN_PASS_CHECK,
    APP_STATE_DRAGON_DRAIN,
    APP_STATE_SAE_OVERFLOW,
    APP_STATE_BLACKOUT,
    APP_STATE_KARMA,
    APP_STATE_PORTAL,
    APP_STATE_SNIFFER,
    APP_STATE_SNIFFER_DOG,
    APP_STATE_WARDRIVE
} app_state_t;

// Scan result structure
typedef struct {
    uint8_t bssid[6];
    char ssid[33];
    uint8_t channel;
    wifi_auth_mode_t authmode;
    int rssi;
    bool selected;
} scan_result_t;

// Probe request structure
typedef struct {
    uint8_t mac[6];
    char ssid[33];
    int rssi;
    uint32_t last_seen;
} probe_request_t;

// Sniffer client structure
typedef struct {
    uint8_t mac[6];
    int rssi;
    uint32_t last_seen;
} sniffer_client_t;

// Client info structure (alias for compatibility)
typedef sniffer_client_t client_info_t;

// Sniffer AP structure
typedef struct {
    uint8_t bssid[6];
    char ssid[33];
    uint8_t channel;
    wifi_auth_mode_t authmode;
    int rssi;
    sniffer_client_t clients[MAX_CLIENTS_PER_AP];
    int client_count;
    uint32_t last_seen;
} sniffer_ap_t;

// GPS data structure
typedef struct {
    float latitude;
    float longitude;
    float altitude;
    float accuracy;
    int satellites;
    bool valid;
} gps_data_t;

// Target BSSID structure
typedef struct {
    uint8_t bssid[6];
    char ssid[33];
    uint8_t channel;
    uint32_t last_seen;
    bool active;
} target_bssid_t;

// Global LED strip handle (shared across components)
extern led_strip_handle_t g_led_strip;

// Global application state (shared across components)
extern volatile app_state_t g_app_state;

// Global stop flag (shared across components)
extern volatile bool g_operation_stop_requested;

// Shared scan results (from wifi_scanner, used by attacks/sniffer)
extern wifi_ap_record_t g_shared_scan_results[MAX_SCAN_RESULTS];
extern uint16_t g_shared_scan_count;
extern int g_shared_selected_indices[MAX_SCAN_RESULTS];
extern int g_shared_selected_count;

// Helper macros
#include "esp_log.h"
#define MY_LOG_INFO(tag, fmt, ...) ESP_LOGI(tag, fmt, ##__VA_ARGS__)

// Common helper functions
const char* authmode_to_string(wifi_auth_mode_t mode);
void escape_csv_field(const char* input, char* output, size_t output_size);
bool is_multicast_mac(const uint8_t *mac);
bool is_broadcast_bssid(const uint8_t *bssid);
bool is_own_device_mac(const uint8_t *mac);

#ifdef __cplusplus
}
#endif

#endif // WIFI_COMMON_H

