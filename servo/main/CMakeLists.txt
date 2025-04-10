idf_component_register(SRCS "main.c" "firebase.c" "servo.c" "wifi_connect.c" "cJSON.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_http_client nvs_flash esp_wifi driver esp_event json
                    EMBED_TXTFILES "firebase_cert.pem")
