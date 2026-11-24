#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "wifi_connect.h"
#include "udp_client.h"
#include "encrypt.h"
#include "self_test.h"
#include "atecc_key.h"

#define TAG "telemetry"

void app_main(void)
{
   uint8_t msg[]= "message de test";
   test(msg,strlen((char*)msg));

   esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_connect_sta();
    ESP_LOGI(TAG,"atecc start");
    atecc_key();
    ESP_LOGI(TAG,"atecc end");
    udp_client_start();
}
