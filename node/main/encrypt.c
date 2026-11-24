#include <stdio.h>
#include <string.h>

#include "mbedtls/gcm.h"

#include "encrypt.h"



static uint8_t iv[12];
static uint64_t counter=0;
static uint8_t key[32];

mbedtls_gcm_context ctx;

void encrypt_init(void){
	mbedtls_gcm_init(&ctx);
	mbedtls_gcm_setkey(&ctx,MBEDTLS_CIPHER_ID_AES,key,sizeof(key)*8);
	}

void gcm_encrypt(uint8_t *payload, size_t len, uint8_t *ciphertext, uint8_t *tag, uint8_t *nonce){

	memset(iv,0,sizeof(iv));
	memcpy(iv,&counter,sizeof(counter));
	int ret= mbedtls_gcm_crypt_and_tag(&ctx,MBEDTLS_GCM_ENCRYPT,len,iv,12,NULL,0,payload,ciphertext,16,tag);
	if(ret!=0)
	      printf("chiffremant échoué: -0x%04x\n", -ret);
    	else {
	      memcpy(nonce,iv,sizeof(iv));
	      counter++;}
	}


int gcm_decrypt(uint8_t *ciphertext,size_t len, uint8_t *decrypted,uint8_t *nonce, uint8_t *tag){

	int ret= mbedtls_gcm_auth_decrypt(&ctx,len, nonce, 12, NULL, 0, tag, 16,ciphertext, decrypted);
	return ret; }
