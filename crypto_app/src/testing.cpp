/**
 * @brief for testing functions
 * @file testing.cpp
 */

#include "../include/encryption.hpp"
#define BUFFER 300

int encrypt_decrypt_test(void)
{
    int ret = -1;
    string plain = "Super Secret Message";
    string decrypted_string = "";
    uint8_t padded_cipher[BUFFER] = {0}; // passed between functions for testing
    encryption COB;

    ret = COB.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        cout << "Encryption failed" << endl;
        return -1;
    }

    ret = COB.handle_decryption(reinterpret_cast<const uint8_t *>(padded_cipher), decrypted_string);

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
    string plain_out = "";
    ifstream my_file(file_name.c_str());
    string buffer = "";
    int ret = -1;

    if (my_file.is_open())
    {
        getline(my_file, buffer);
        my_file.close();
        cout << buffer << endl;
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }
    encryption COB;
    string plain ="Secret";
    uint8_t padded_cipher[BUFFER] = {0};
    ret = COB.handle_encryption(plain, padded_cipher);

    if (ret != 0)
    {
        cout << "Encryption failed" << endl;
        return -1;
    }

    const char * padded_cipher2 = buffer.c_str();


    ret = COB.handle_decryption(reinterpret_cast<const uint8_t *>(padded_cipher2), plain_out);
    if (ret != 0)
    {
        cout << "handle_decryption went wrong" << endl;
        return -1;
    }

    // cout << plain_out << endl;
    return 0;
}