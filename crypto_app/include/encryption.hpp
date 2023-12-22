#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <iostream>
#include <memory>
#include <random>
#include "../src/utils.hpp"
#include "key.hpp"

#define BUFFER 300
#define AES_BLOCK_SIZE 256

const int total_cipher_len = 276;
const int payload_size = 256;
const int header_size = 20;
const int iv_size = 16;
const int payload_byte_size = 4;

class encryption
{

public:
    encryption();
    ~encryption();

    /**
     * @brief 
     * @param plain plain password 
     * @param padded_cipher 16 Bytes IV + 4 Bytes Length + 256 Bytes encrypted+padded Payload
     * @return 0 if successful
     */
    int handle_encryption(std::string plain, uint8_t *padded_cipher);
    
    /**
     * @brief 
     * @param cipher 
     * @param decrypted_string 
     * @param in 
     * @return 0 if successful
     */
    int handle_decryption(const uint8_t * cipher, std::string &decrypted_string);

private:

    /**
     * @brief Interface for EVP_CIPHER_CTX 
     *  EVP_CIPHER_CTX_new EVP_DecryptInit_ex EVP_DecryptUpdate EVP_CIPHER_CTX_free
     * @param cipher_text 
     * @param ciphertext_len 
     * @param crypto_key 
     * @param iv 
     * @param plaintext 
     * @param plaintext_len 
     * @return true if successful
     */
    bool decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len);
    

    /**
     * @brief Interface for EVP_CIPHER_CTX
     *  EVP_CIPHER_CTX_new EVP_EncryptInit_ex EVP_EncryptUpdate EVP_CIPHER_CTX_free
     * @param plaintext 
     * @param plaintext_len 
     * @param crypto_key 
     * @param iv 
     * @param cipher_text 
     * @param ciphertext_len 
     * @return true if successful
     */
    bool encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len);
    

    /**
     * @brief get IV(Initial-Vector) 16 Bytes from cipher text + add length of cipher_text from byte 16 to 19 + add cipher_text
     * @param iv 
     * @param plain_cipher_text 
     * @param pl_ciph_text_size 
     * @param cipher_text_out 
     * @param ciph_text_out_size 
     * @return 0 if successful
     */
    int prepare_ciphertext(uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size, uint8_t *cipher_text_out, uint64_t ciph_text_out_size);
    
    /**
     * @brief copy bytes 0 to 15 to iv during decryption
     * @param iv Initial-Vector
     * @param cipher_text cipher text (header+payload)
     * @return 0 if successful
     */
    int get_iv(unsigned char *iv, const uint8_t *cipher_text);

};

unsigned char iv[16];

#endif // ENCRYPTION_H