#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <iostream>
#include "../src/utils.hpp"
using namespace std;

class encryption
{
private:
    /* data */
public:
    bool decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len);
    bool encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len);
    int prepare_ciphertext(uint8_t version, uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size, uint8_t *cipher_text_out, uint64_t ciph_text_out_size);
    int aes_cbc(void);
    encryption(/* args */);
    ~encryption();
};

encryption::encryption(/* args */)
{
}

encryption::~encryption()
{
}

unsigned char iv[16] = {0};
unsigned char key[32] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                         0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                         0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
                         0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31};

#endif //ENCRYPTION_H