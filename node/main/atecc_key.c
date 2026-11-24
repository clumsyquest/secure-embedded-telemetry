#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "atca_basic.h"

#include "atecc_key.h"


#define TAG "atecc608a"


void atecc(void *arg){

ATCA_STATUS status;
uint8_t random_number[32];

ATCAIfaceCfg cfg={

 .iface_type= ATCA_I2C_IFACE,
 .devtype= ATECC608A,
 .atcai2c.address= 0xC0,
 .atcai2c.bus= 1,
 .atcai2c.baud= 100000,
 .wake_delay= 1500,
 .rx_retries= 20};

status=atcab_init(&cfg);
if (status!=0){
        ESP_LOGI(TAG,"ATECC608A configuration failed");
	vTaskDelete(NULL);
	return;
}
ESP_LOGI(TAG,"ATECC608A configured");
while(1){


status=atcab_random(random_number);
//status= atcab_read_serial_number(random_number);
if (status!=0){
        ESP_LOGI(TAG,"error getting random number");
	vTaskDelete(NULL);
	return;
}
else {
       printf("random_number: ");
        for (int i=0; i<32;i++){
        if (i<31)
        printf("%.2x,", random_number[i]);
        else
        printf("%.2x\n", random_number[i]);

}
}
vTaskDelay(200 / portTICK_PERIOD_MS);

}
vTaskDelete(NULL);
}



void atecc_key(void){
xTaskCreate(atecc,"atecc_key",4096,NULL,3,NULL);}
