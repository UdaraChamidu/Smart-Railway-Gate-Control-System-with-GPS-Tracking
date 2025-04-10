#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "driver/gpio.h"

#include "servo.h"
#include "firebase.h"

extern const uint8_t firebase_cert_pem_start[] asm("_binary_firebase_cert_pem_start");
extern const uint8_t firebase_cert_pem_end[]   asm("_binary_firebase_cert_pem_end");

#define LED_PIN     12
#define BUZZER_PIN  14


#define FIREBASE_HOST "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/"

#define FIREBASE_AUTH "?auth=AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"
#define SERVO_PATH  "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/servo/servo1.json?auth=AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"
#define LED_PATH    "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/led/led1.json?auth=AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"
#define BUZZER_PATH "https://smart-railway-1d2fd-default-rtdb.firebaseio.com/buzzer/buzzer1.json?auth=AIzaSyD1MasgGuxrfqjEE4_Di8cjR4HuW2eFgwg"


static const char *TAG = "MAIN";

// Store previous states
static bool last_servo_state = false;
static bool last_led_state = false;
static bool last_buzzer_state = false;

static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
            break;
        default:
            ESP_LOGW(TAG, "Unhandled HTTP event: %d", evt->event_id);
            break;
    }
    return ESP_OK;
}

// Wi-Fi event handler
static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected, retrying...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "Got IP!");
    }
}

// Initialize Wi-Fi in STA mode
void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Galaxy A51 C187",
            .password = "ncrk7311",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Wi-Fi initialization complete.");
}

// Get boolean value from Firebase
bool get_firebase_bool(const char *path, bool *out_value) {
    char url[256];
    snprintf(url, sizeof(url), "%s%s", path, FIREBASE_AUTH);
    ESP_LOGI(TAG, "Constructed URL: %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .cert_pem = (const char *)firebase_cert_pem_start,
        .event_handler = http_event_handler,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_method(client, HTTP_METHOD_GET);

    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) {
        ESP_LOGE(TAG, "Invalid content length");
        esp_http_client_cleanup(client);
        return false;
    }

    char buffer[64] = {0};
    int read_len = esp_http_client_read(client, buffer, sizeof(buffer) - 1);
    if (read_len >= 0) {
        buffer[read_len] = '\0';
        ESP_LOGI(TAG, "Firebase Response: %s", buffer);

        if (strcmp(buffer, "true") == 0) {
            *out_value = true;
        } else if (strcmp(buffer, "false") == 0) {
            *out_value = false;
        } else {
            ESP_LOGE(TAG, "Unexpected response: %s", buffer);
            esp_http_client_cleanup(client);
            return false;
        }
    } else {
        ESP_LOGE(TAG, "Failed to read response");
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_cleanup(client);
    return true;
}

bool get_firebase_servo_status(bool *out_value) {
    esp_http_client_config_t config = {
        .url = SERVO_PATH,  // Firebase URL with the auth query parameter
        .method = HTTP_METHOD_GET,
        .cert_pem = (const char *)firebase_cert_pem_start,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "Failed to init HTTP client");
        return false;
    }

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    int content_length = esp_http_client_get_content_length(client);
    if (content_length <= 0) content_length = 512;  // fallback

    char *buffer = malloc(content_length + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "Failed to allocate buffer");
        esp_http_client_cleanup(client);
        return false;
    }

    int read_len = esp_http_client_read_response(client, buffer, content_length);
    buffer[read_len] = '\0';

    ESP_LOGI(TAG, "Firebase response: %s", buffer);

    // Parse the response
    if (strcmp(buffer, "true") == 0) {
        *out_value = true;
    } else if (strcmp(buffer, "false") == 0) {
        *out_value = false;
    } else {
        ESP_LOGE(TAG, "Invalid response from Firebase: %s", buffer);
        free(buffer);
        esp_http_client_cleanup(client);
        return false;
    }

    free(buffer);
    esp_http_client_cleanup(client);
    return true;
}

// Firebase monitor task
void monitor_firebase_task(void *pvParameters) {
    while (1) {
        bool servo_state = false, led_state = false, buzzer_state = false;

        ESP_LOGI(TAG, "Fetching Firebase states...");

        if (get_firebase_bool(LED_PATH, &led_state)) {
            ESP_LOGI(TAG, "LED state fetched: %d", led_state);
            if (led_state != last_led_state) {
                gpio_set_level(LED_PIN, led_state);
                ESP_LOGI(TAG, "LED state changed: %d", led_state);
                last_led_state = led_state;
                gpio_set_level(LED_PIN, led_state);
            }
        } else {
            ESP_LOGE(TAG, "Failed to fetch LED state");
        }

        if (get_firebase_bool(BUZZER_PATH, &buzzer_state)) {
            ESP_LOGI(TAG, "Buzzer state fetched: %d", buzzer_state);
            if (buzzer_state != last_buzzer_state) {
                gpio_set_level(BUZZER_PIN, buzzer_state);
                ESP_LOGI(TAG, "Buzzer state changed: %d", buzzer_state);
                last_buzzer_state = buzzer_state;
                gpio_set_level(BUZZER_PIN, buzzer_state);
            }
        } else {
            ESP_LOGE(TAG, "Failed to fetch Buzzer state");
        }

        if (get_firebase_servo_status(&servo_state)) {
            ESP_LOGI(TAG, "Servo state fetched: %d", servo_state);
            if (servo_state != last_servo_state) {
                if (servo_state) {
                    for (int angle = 0; angle <= 90; angle += 5) {
                        servo_set_angle(angle);
                        vTaskDelay(pdMS_TO_TICKS(100));
                    }
                } else {
                    for (int angle = 90; angle >= -90; angle -= 5) {
                        servo_set_angle(angle);
                        vTaskDelay(pdMS_TO_TICKS(100));
                    }
                }
                ESP_LOGI(TAG, "Servo state changed: %d", servo_state);
                last_servo_state = servo_state;
            }
        } else {
            ESP_LOGE(TAG, "Failed to fetch Servo state");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Main application
void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    // Initialize Wi-Fi
    wifi_init_sta();

    // GPIOs
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUZZER_PIN, GPIO_MODE_OUTPUT);

    gpio_set_level(LED_PIN, 0);
    gpio_set_level(BUZZER_PIN, 0);

    // Servo
    servo_init();
    servo_set_angle(0);

    // Start Firebase monitoring task
    xTaskCreate(&monitor_firebase_task, "monitor_firebase_task", 4096, NULL, 5, NULL);
}
