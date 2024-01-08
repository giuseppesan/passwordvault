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
#include "utils.hpp"
#include "key.hpp"

const int AES_BLOCK_SIZE = 256;
const int IV_SIZE = 16;

class encryption
{

public:
    encryption();
    ~encryption();

    /**
     * @brief Handles the encryption process
     * @param plain plain password 
     * @param padded_cipher 16 Bytes IV + 256 Bytes encrypted+padded Payload
     * @return 0 if successful
     */
    int handle_encryption(const std::string plain, std::vector<uint8_t> &padded_cipher);
    
    /**
     * @brief Handles the decryption process
     * @param cipher encrypted bytes
     * @param decrypted_string decrypted password
     * @return 0 if successful
     */
    int handle_decryption(const std::vector<uint8_t> &cipher, std::string &decrypted_string);
    
    /**
     * @brief Encrypts the password and returns it as hex string
     * @param password plain password
     * @return encrypted hex string
     */
    int encrypt_credentials(std::string &password);
    
    /**
     * @brief Strips credential string and decrypts the password
     * @param credentials full credential string 
     * @return plain password
     */
    int decrypt_credentials(std::string &credentials);

    /**
     * @brief add new login credentials
     * 
     * @param tag tag which the user searches for e.g. google
     * @param user username
     * @param password user password
     * 
     * @return 0 if successful
     */
    int add_new_entry(const std::string& tag, const std::string& user, const std::string& password);
    
private:

    /**
     * @brief Interface for EVP_CIPHER_CTX 
     *  EVP_CIPHER_CTX_new EVP_DecryptInit_ex EVP_DecryptUpdate EVP_CIPHER_CTX_free
     * @param cipher_text the encrypted block with IV and password
     * @param ciphertext_len length of the cipher_text
     * @param crypto_key symmetric key used for en/decryption
     * @param iv Initial-Vector
     * @param plaintext the decrypted password
     * @param plaintext_len the length of decrypted password
     * @return true if successful
     */
    bool decrypt(const std::vector<uint8_t> &cipher_text, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &plaintext, int &plaintext_len);
    

    /**
     * @brief Interface for EVP_CIPHER_CTX
     *  EVP_CIPHER_CTX_new EVP_EncryptInit_ex EVP_EncryptUpdate EVP_CIPHER_CTX_free
     * @param plaintext plain password
     * @param plaintext_len length of the plaintext
     * @param crypto_key symmetric key used for en/decryption
     * @param iv Initial-Vector
     * @param cipher_text the encrypted password block
     * @param ciphertext_len length of the encrypted_text
     * @return true if successful
     */
    bool encrypt(const std::vector<uint8_t> &plaintext, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &cipher_text, int &ciphertext_len);
    

    /**
     * @brief get IV(Initial-Vector) 16 Bytes and add to cipher_text
     * @param iv Initial-Vector
     * @param cipher_text the encrypted text/password
     * @param cipher_block the complete cipher block = IV + cipher_text 
     * @return 0 if successful
     */
    int prepare_ciphertext(const uint8_t *iv, const std::vector<uint8_t> &cipher_text, std::vector<uint8_t> &cipher_block);
    
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