/**
 * @brief for testing functions
 * @file testing.cpp
 */
#include <cassert>
#include "../include/encryption.hpp"
#include "../include/crypto.hpp"
#define BUFFER 300

int encrypt_decrypt_test(void)
{
    int ret = -1;
    std::string plain = "Super Secret Message";
    std::string decrypted_string = "";
    uint8_t padded_cipher[BUFFER] = {0}; // passed between functions for testing
    encryption crypto_obj;

    ret = crypto_obj.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        std::cout << "Encryption failed" << std::endl;
        return -1;
    }

    ret = crypto_obj.handle_decryption(reinterpret_cast<const uint8_t *>(padded_cipher), decrypted_string);

    if (ret != 0)
    {
        std::cout << "Decryption failed" << std::endl;
        return -1;
    }

    if (strcmp(plain.c_str(), decrypted_string.c_str()) != 0)
    {
        std::cout << "Input and Output strings do not match" << std::endl;
        return -1;
    }

    return 0;
}

int decrypt_from_file_test(std::string file_name)
{
    std::ifstream my_file(file_name.c_str(), std::ios::binary);
    std::string plain_out = "";
    std::string read_buffer = "";
    char cipher_bytes[BUFFER];
    int ret = -1;

    encryption crypto_obj;
    std::string plain = "Secret";
    uint8_t padded_cipher[BUFFER] = {0};

    ret = crypto_obj.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        std::cout << "Encryption failed" << std::endl;
        return -1;
    }

    write_to_file(to_hex(padded_cipher, 300), file_name);

    if (my_file.is_open())
    {
        getline(my_file, read_buffer);
        my_file.close();
    }
    else
    {
        std::cout << "Unable to open & read file\n";
        return -1;
    }

    hex2bin(read_buffer.c_str(), cipher_bytes);

    ret = crypto_obj.handle_decryption(reinterpret_cast<const uint8_t *>(cipher_bytes), plain_out);

    if (ret != 0)
    {
        std::cout << "handle_decryption went wrong" << std::endl;
        return -1;
    }

    my_file.open(file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    my_file.close();
    return 0;
}
