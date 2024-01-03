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
#include <stdexcept>
#include "../src/utils.cpp"
#include "key.hpp"

const int AES_BLOCK_SIZE = 256;
const int IV_SIZE = 16;

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
    int handle_encryption(const std::string plain, std::vector<uint8_t> &padded_cipher);
    
    /**
     * @brief 
     * @param cipher 
     * @param decrypted_string 
     * @param in 
     * @return 0 if successful
     */
    int handle_decryption(const std::vector<uint8_t> & cipher, std::string &decrypted_string);

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
    bool decrypt(const std::vector<uint8_t> &cipher_text, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &plaintext, int &plaintext_len);
    

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
    bool encrypt(const std::vector<uint8_t> &plaintext, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &cipher_text, int &ciphertext_len);
    

    /**
     * @brief get IV(Initial-Vector) 16 Bytes from cipher text + add length of cipher_text from byte 16 to 19 + add cipher_text
     * @param iv 
     * @param plain_cipher_text 
     * @param pl_ciph_text_size 
     * @param cipher_text_out 
     * @param ciph_text_out_size 
     * @return 0 if successful
     */
    int prepare_ciphertext(const uint8_t *iv, const std::vector<uint8_t> &plain_cipher_text, std::vector<uint8_t> &cipher_block);
    
    /**
     * @brief copy bytes 0 to 15 to iv during decryption
     * @param iv Initial-Vector
     * @param cipher_text cipher text (header+payload)
     * @return 0 if successful
     */
    int get_iv(unsigned char *iv, const std::vector<uint8_t> &cipher_text);

};

unsigned char iv[16];

#endif // ENCRYPTION_H