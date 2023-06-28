
#include "../include/crypto.hpp"

#define BUFFER 300
#define AES_BLOCK_SIZE 256

int aes_cbc(void)
{
    for (size_t i = 0; i <= 16; i++)
    {
        iv[i] = rand() % 256;
    }

    /* Message to be encrypted */
    string plain = "Super Secret Message";
    cout << "Input is: " << plain << endl;

    uint8_t *plaintext = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(plain.c_str()));
    uint8_t plaintext_buff[AES_BLOCK_SIZE] = {0};
    uint8_t ciphertext[AES_BLOCK_SIZE] = {0};
    uint8_t padded_frame[BUFFER] = {0};
    uint8_t decryptedtext[AES_BLOCK_SIZE] = {0};
    int plaintext_len = AES_BLOCK_SIZE;
    int ciphertext_len = 0;
    int ret = false;
    string hexString = "";
    char *hexArray;
    string decryptedString = "";

    if (plain.size() > AES_BLOCK_SIZE - 1)
    {
        cout << "plaintext size too big" << endl;
        return -1;
    }

    cout << "Input size = " << plain.size() << endl
         << endl;

    for (size_t i = 0; i < plain.size(); i++)
    {
        plaintext_buff[i] = plaintext[i];
    }

    // Padding
    if (plain.size() < AES_BLOCK_SIZE)
    {
        for (size_t i = plain.size(); i < (AES_BLOCK_SIZE - plain.size()); i++)
        {
            plaintext_buff[i + plain.size()] = 0;
        }
    }

    /*Encryption*/
    cipher_object COB;

    ret = COB.encrypt(plaintext_buff, plaintext_len, key, iv,
                      ciphertext, ciphertext_len);

    if (ret != true)
    {
        cout << "Encrypt failed" << endl;
        return -1;
    }

    ret = COB.prepare_ciphertext(1, iv, ciphertext, ciphertext_len, padded_frame, 277);

    if (ret != 0)
    {
        cout << "Preparing ciphertext failed" << endl;
        return -1;
    }

    std::cout << "Ciphertext is:\n";
    hexString = toHex(padded_frame, 277);

    cout << hexString << endl;

    BIO_dump_fp(stdout, (const char *)padded_frame, 277);

    for (size_t i = 0; i < 256; i++)
    {
        ciphertext[i] = padded_frame[i + 21];
    }

    /*Decryption*/
    ret = COB.decrypt(ciphertext, ciphertext_len, key, iv,
                      decryptedtext, plaintext_len);

    if (ret != true)
    {
        cout << "Decrypt failed" << endl;
        return -1;
    }

    /* Show the decrypted text */
    std::cout << "\nDecrypted text is:" << endl;
    std::cout << decryptedtext << endl;

    decryptedString.assign(decryptedtext, decryptedtext + plaintext_len);

    if (strcmp(plain.c_str(), decryptedString.c_str()) != 0)
    {
        cout << "Input and Output strings do not match" << endl;
        return -1;
    }

    return 0;
}

bool cipher_object::encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext, int &ciphertext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        std::cerr << "Failed to initialize encryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_EncryptUpdate(ctx, ciphertext, &ciphertext_len, plaintext, plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool cipher_object::decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv) != 1)
    {
        std::cerr << "Failed to initialize decryption" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    if (EVP_DecryptUpdate(ctx, plaintext, &plaintext_len, ciphertext, ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

int cipher_object::prepare_ciphertext(uint8_t version, uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size,
                                      uint8_t *cipher_text_out, uint64_t ciph_text_out_size)
{
    /*copy version to byte 0*/
    cipher_text_out[0] = version;
    /*copy IV to bytes 1 to 16*/
    for (int i = 1; i < 17; i++)
    {
        cipher_text_out[i] = iv[i - 1];
    }
    /*add length of ciphertext from byte 17 to 20*/
    *(reinterpret_cast<uint32_t *>(cipher_text_out + 17)) = pl_ciph_text_size;

    if (pl_ciph_text_size > (ciph_text_out_size - 1 - 16 - 4))
    {
        cout << "buffer is too big or cipher array is too small" << endl;
        return -1;
    }
    /*copy ciphertext*/
    for (uint32_t j = 0; j < ciph_text_out_size; j++)
    {
        cipher_text_out[j + 21] = plain_cipher_text[j];
    }

    return 0;
}

string toHex(unsigned char *str, int len)
{
    string hex_hash_string = "";
    char *buffer = new char[len * 2 + 1];
    char *pbuffer = buffer;
    for (int i = 0; i < len; ++i)
    {
        sprintf(pbuffer, "%02X", str[i]);
        pbuffer += 2;
    }

    hex_hash_string.assign(buffer, buffer + len);
    return hex_hash_string;
}

int write_to_file(string in)
{
    ofstream myfile("sha");
    if (myfile.is_open())
    {
        myfile << in;
        myfile.close();
    }
    else
    {
        cout << "Unable to open file\n";
        return -1;
    }
    return 0;
}

int read_from_file(string &out)
{
    ifstream myfile("sha");
    if (myfile.is_open())
    {
        getline(myfile, out);
        myfile.close();
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }
    return 0;
}

int sha_256()
{
    int ret = -1;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const unsigned char plain_password[] = "Secret";
    unsigned char *hash_bytes_ptr;

    string hex_hash_string = "";

    string compare_hash_string = "";

    /* Generate Hash*/
    hash_bytes_ptr = SHA256(plain_password, 7, hash);

    if (hash_bytes_ptr == NULL)
    {
        cout << " NULL pointer" << endl;
        return -1;
    }

    hex_hash_string = toHex(hash_bytes_ptr, SHA256_DIGEST_LENGTH);

    ret = write_to_file(hex_hash_string);

    if (ret != 0)
    {
        return ret;
    }

    ret = read_from_file(compare_hash_string);

    if (ret != 0)
    {
        return ret;
    }

    ret = strcmp(compare_hash_string.c_str(), hex_hash_string.c_str());
    cout << compare_hash_string << "\n"
         << hex_hash_string << endl;
    if (ret != 0)
    {
        cout << "Hash ckeck was not Sucessfull: " << ret << endl;
        return ret;
    }

    cout << "Hash check was sucessfull" << endl;

    return 0;
}
