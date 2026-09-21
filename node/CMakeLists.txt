idf_component_register(SRCS "telemetry.c" "wifi_connect.c" "udp_client.c" "mpu_sensor.c"
                       PRIV_REQUIRES spi_flash esp_wifi nvs_flash
                       INCLUDE_DIRS "")
