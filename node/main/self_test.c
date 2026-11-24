#include <stdio.h>
#include <string.h>

#include "mbedtls/gcm.h"
#include "encrypt.h"

#include "esp_log.h"

static const char *TAG="TEST";


void test(uint8_t *payload, size_t len){

	static uint8_t ciphertext[256];
	static uint8_t tag[16];
	static uint8_t iv[12];
	static uint8_t decrypted[256];

	encrypt_init();
	gcm_encrypt(payload, len, ciphertext, tag, iv);
	int r= gcm_decrypt(ciphertext, len, decrypted, iv,tag);
	if (r==0 && memcmp(decrypted,payload,len)==0) ESP_LOGI(TAG, "test okay");
	else ESP_LOGI(TAG,"test echoué");

	ciphertext[0] ^= 0xFF;                               // on « corrompt » le 1er octet
	int r2 = gcm_decrypt(ciphertext, len, decrypted, iv, tag);
	if (r2 != 0) ESP_LOGI(TAG, "alteration detectee OK");
	else         ESP_LOGE(TAG, "PROBLEME: alteration non detectee");}

