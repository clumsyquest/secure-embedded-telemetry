#include <string.h>
#include <stdio.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "udp_client.h"
#include "mpu_sensor.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR


#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "UDP Protocol";
static char payload[200];


static void udp_client_task(void *pvParameters)
{
    int addr_family = 0;
    int ip_protocol = 0;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    mpu6050_temp_value_t temp;
    mpu6050_handle_t sensor= mpu6050_init();

    while (1) {

        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;

        int sock = socket(addr_family, SOCK_DGRAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            break;
        }

        // Set timeout
        struct timeval timeout;
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;
        setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof timeout);

        ESP_LOGI(TAG, "Socket created, sending to %s:%d", HOST_IP_ADDR, PORT);

        int len=0;


        while (1) {

            mpu6050_read(sensor, &acce, &gyro, &temp);

            len= snprintf(payload, sizeof(payload),
                     "{\"acce_x\":%.2f,\"acce_y\":%.2f,\"acce_z\":%.2f,"
                     "\"gyro_x\":%.2f,\"gyro_y\":%.2f,\"gyro_z\":%.2f,"
                     "\"temp\":%.2f}",
                     acce.acce_x, acce.acce_y, acce.acce_z,
                     gyro.gyro_x, gyro.gyro_y, gyro.gyro_z,
                     temp.temp);

            if (len>=sizeof(payload)) {
                ESP_LOGW(TAG, "payload troncated ! need %d octets", len);
            }

            int err = sendto(sock, payload, strlen(payload), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                break;
            }
            ESP_LOGI(TAG, "Message sent");

            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}


void udp_client_start(void)
{
    xTaskCreate(udp_client_task,"udp_client",4096,NULL,5,NULL);
}
