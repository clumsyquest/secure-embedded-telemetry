#ifndef ENCRYPT_H
#define ENCRYPT_H

void encrypt_init(void);
void gcm_encrypt(uint8_t *payload, size_t len, uint8_t *ciphertext, uint8_t *tag, uint8_t *nonce);
void encrypt_free(void);
int gcm_decrypt(uint8_t *ciphertext, size_t len, uint8_t *decrypted, uint8_t *nonce, uint8_t *tag);

#endif






