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
#include "key.hpp"
using namespace std;

#define BUFFER 300
#define AES_BLOCK_SIZE 256

class encryption
{
private:
    bool decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len);
    bool encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len);
    int prepare_ciphertext(uint8_t version, uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size, uint8_t *cipher_text_out, uint64_t ciph_text_out_size);
    int get_iv(unsigned char *iv, const uint8_t *cipher_text);

public:
    int handle_encryption(string plain, uint8_t *padded_cipher, encryption &COB);
    int handle_decryption(const uint8_t *cipher, string &decrypted_string, encryption COB);

    encryption(/* args */);
    ~encryption();
};

unsigned char iv[16] = {0};

#endif // ENCRYPTION_H