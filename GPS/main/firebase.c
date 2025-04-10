#include "firebase.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"

// 👇 Embed Firebase SSL certificate (PEM file)
extern const uint8_t firebase_cert_pem_start[] asm("_binary_firebase_cert_pem_start");
extern const uint8_t firebase_cert_pem_end[]   asm("_binary_firebase_cert_pem_end");

static const char *TAG = "FIREBASE";

void update_gps_to_firebase(GPS_Coordinates coords) {
    // Create JSON with latitude and longitude
    cJSON *json_data = cJSON_CreateObject();
    cJSON_AddNumberToObject(json_data, "latitude", coords.latitude);
    cJSON_AddNumberToObject(json_data, "longitude", coords.longitude);
    char *json_string = cJSON_PrintUnformatted(json_data);

    // 🔁 Update existing "gps" node in Firebase
    char url[256];
    snprintf(url, sizeof(url), FIREBASE_URL "gps.json"); // 🔁 This is the path you're updating!

    // HTTP client config with embedded cert
    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_PUT,
        .cert_pem = (const char *)firebase_cert_pem_start,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_string, strlen(json_string));

    // Send the request
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "✅ Updated GPS data in existing 'gps' object.");
    } else {
        ESP_LOGE(TAG, "❌ Firebase update failed: %s", esp_err_to_name(err));
    }

    // Cleanup
    free(json_string);
    cJSON_Delete(json_data);
    esp_http_client_cleanup(client);
}

void firebase_init(void) {
    ESP_LOGI(TAG, "Firebase initialization completed.");
}
