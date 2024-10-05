/**
 * @brief for testing functions
 * @file testing.cpp
 */

#include "encryption.hpp"
#include "hash.hpp"

int encrypt_decrypt_test(std::string plain_text)
{
    int ret = -1;
    std::string decrypted_string = "";
    std::vector<uint8_t> cipher_block; // passed between functions for testing
    encryption crypto_obj;

    ret = crypto_obj.handle_encryption(plain_text, cipher_block);

    if (ret != 0)
    {
        std::cout << "Encryption failed" << std::endl;
        return -1;
    }

    ret = crypto_obj.handle_decryption(cipher_block, decrypted_string);

    if (ret != 0)
    {
        std::cout << "Decryption failed" << std::endl;
        return -1;
    }

    if (strcmp(plain_text.c_str(), decrypted_string.c_str()) != 0)
    {
        std::cout << "\nInput and Output strings do not match" << std::endl;
        return -1;
    }

    return 0;
}

int decrypt_from_file_test(std::string file_name, std::string plain_text)
{
    std::ifstream my_file(file_name.c_str(), std::ios::binary);
    std::string plain_out = "";
    std::string read_buffer = "";
    std::vector<uint8_t> cipher_bytes;
    int ret = -1;

    encryption crypto_obj;
    std::vector<uint8_t> padded_cipher;

    ret = crypto_obj.handle_encryption(plain_text, padded_cipher);

    if (ret != 0)
    {
        std::cout << "Encryption failed" << std::endl;
        return -1;
    }

    utils::write_to_file(utils::to_hex(padded_cipher), file_name);

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

    utils::hex2bin(read_buffer.c_str(), cipher_bytes);

    ret = crypto_obj.handle_decryption(cipher_bytes, plain_out);

    if (ret != 0)
    {
        std::cout << "handle_decryption went wrong" << std::endl;
        return -1;
    }

    if (strcmp(plain_text.c_str(), plain_out.c_str()) != 0)
    {
        std::cout << "Input and Output strings do not match" << std::endl;
        my_file.open(file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
        my_file.close();
        return -1;
    }

    my_file.open(file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    my_file.close();
    return 0;
}
