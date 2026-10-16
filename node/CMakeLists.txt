idf_component_register(SRCS "telemetry.c" "wifi_connect.c" "udp_client.c" "mpu_sensor.c" "encrypt.c" "self_test.c"
                       PRIV_REQUIRES spi_flash esp_wifi nvs_flash mbedtls
                       INCLUDE_DIRS "")
