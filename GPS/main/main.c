#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "neo6m.h"
#include "firebase.h"

extern const uint8_t firebase_cert_pem_start[] asm("_binary_firebase_cert_pem_start");
extern const uint8_t firebase_cert_pem_end[]   asm("_binary_firebase_cert_pem_end");

// Wi-Fi credentials
#define WIFI_SSID "Galaxy A51 C187"
#define WIFI_PASS "ncrk7311"

static const char *TAG = "GPS_MAIN";

// ------------------ Wi-Fi Event Handler ------------------
// Changed return type from `esp_err_t` to `void` to match expected signature
void event_handler(void *ctx, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (strcmp(event_base, WIFI_EVENT) == 0) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGW(TAG, "Disconnected from Wi-Fi. Reconnecting...");
                esp_wifi_connect();
                break;
            default:
                break;
        }
    } else if (strcmp(event_base, IP_EVENT) == 0) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                ESP_LOGI(TAG, "Wi-Fi connected, IP acquired");
                break;
            default:
                break;
        }
    }
}

// ------------------ Wi-Fi Initialization ------------------
void wifi_init_sta(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Register event handlers
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

// ------------------ Wi-Fi Task ------------------
void wifi_task(void *pvParameters) {
    ESP_LOGI(TAG, "Starting Wi-Fi task...");
    wifi_init_sta();
    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// ------------------ GPS & Firebase Task ------------------
void gps_task(void *pvParameters) {
    ESP_LOGI(TAG, "Starting GPS + Firebase task...");
    while (1) {
        GPS_Coordinates coords = get_gps_coordinates();

        if (coords.latitude != 0.0 && coords.longitude != 0.0) {
            ESP_LOGI(TAG, "Latitude: %.6f, Longitude: %.6f", coords.latitude, coords.longitude);
            update_gps_to_firebase(coords);
        } else {
            ESP_LOGW(TAG, "Invalid GPS fix, waiting...");
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS); // 10 seconds
    }
}

// ------------------ Main ------------------
void app_main(void) {
    gps_init();       // Init GPS UART
    firebase_init();  // Firebase init (you must define this in firebase.c)

    xTaskCreatePinnedToCore(wifi_task, "wifi_task", 4096, NULL, 10, NULL, 0);
    xTaskCreatePinnedToCore(gps_task, "gps_task", 4096, NULL, 10, NULL, 1);
}
