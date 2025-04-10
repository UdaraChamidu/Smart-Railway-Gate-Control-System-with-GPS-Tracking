#include "firebase.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"

static const char *TAG = "FIREBASE";

/**
 * @brief Reads a boolean value from a Firebase path.
 * 
 * @param path Firebase database path (e.g., "servo/servo1")
 * @param out_value Pointer to store the retrieved boolean value
 * @return true if the value was successfully retrieved, false otherwise
 */
bool firebase_get_bool(const char *path, bool *out_value) {
    char url[256];
    snprintf(url, sizeof(url), "%s%s.json?auth=%s", FIREBASE_URL, path, FIREBASE_API_KEY);

    esp_http_client_config_t config = {
        .url = url,
        .method = HTTP_METHOD_GET
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err = esp_http_client_perform(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP GET failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    int content_length = esp_http_client_get_content_length(client);
    char *buffer = malloc(content_length + 1);
    if (!buffer) {
        ESP_LOGE(TAG, "Failed to allocate buffer");
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_read_response(client, buffer, content_length);
    buffer[content_length] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        ESP_LOGE(TAG, "Failed to parse JSON");
        free(buffer);
        esp_http_client_cleanup(client);
        return false;
    }

    if (cJSON_IsBool(json)) {
        *out_value = cJSON_IsTrue(json);
        cJSON_Delete(json);
        free(buffer);
        esp_http_client_cleanup(client);
        return true;
    }

    ESP_LOGW(TAG, "JSON value is not boolean");
    cJSON_Delete(json);
    free(buffer);
    esp_http_client_cleanup(client);
    return false;
}
