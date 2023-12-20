/**
 * @brief for testing functions
 * @file testing.cpp
 */
#include <cassert>
#include "../include/encryption.hpp"
#define BUFFER 300

int encrypt_decrypt_test(void);
int decrypt_from_file_test(string file_name);

int encrypt_decrypt_test(void)
{
    int ret = -1;
    string plain = "Super Secret Message";
    string decrypted_string = "";
    uint8_t padded_cipher[BUFFER] = {0}; // passed between functions for testing
    encryption crypto_obj;

    ret = crypto_obj.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        cout << "Encryption failed" << endl;
        return -1;
    }

    ret = crypto_obj.handle_decryption(reinterpret_cast<const uint8_t *>(padded_cipher), decrypted_string);

    if (ret != 0)
    {
        cout << "Decryption failed" << endl;
        return -1;
    }

    if (strcmp(plain.c_str(), decrypted_string.c_str()) != 0)
    {
        cout << "Input and Output strings do not match" << endl;
        return -1;
    }

    return 0;
}

int decrypt_from_file_test(string file_name)
{
    ifstream my_file(file_name.c_str(), std::ios::binary);
    string plain_out = "";
    string read_buffer = "";
    char cipher_bytes[BUFFER];
    int ret = -1;

    encryption crypto_obj;
    string plain = "Secret";
    uint8_t padded_cipher[BUFFER] = {0};

    ret = crypto_obj.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        cout << "Encryption failed" << endl;
        return -1;
    }

    write_to_file(to_hex2(padded_cipher, 300), file_name);

    if (my_file.is_open())
    {
        getline(my_file, read_buffer);
        my_file.close();
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }

    hex2bin(read_buffer.c_str(), cipher_bytes);

    ret = crypto_obj.handle_decryption(reinterpret_cast<const uint8_t *>(cipher_bytes), plain_out);

    if (ret != 0)
    {
        cout << "handle_decryption went wrong" << endl;
        return -1;
    }

    my_file.open(file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
    my_file.close();
    return 0;
}

void hashing_test()
{
    crypto c_obj;
    int result = c_obj.check_login("giu", "giu");
    assert(result == 0 && "Login check failed");
    cout << endl;

    result= c_obj.check_login("giu2", "giu2");
    assert(result == 0 && "Login check failed");
    cout << endl;

    result = c_obj.check_login("giu", "giu2");
    assert(result != 0 && "Login passed but should fail");
    cout << endl;
    
    result= c_obj.check_login("giu2", "giu22");
    assert(result != 0 && "Login passed but should fail");
    cout << endl;

    result= c_obj.check_login("bebo2", "giu22");
    assert(result != 0 && "Login passed but should fail");
    cout << endl;
}