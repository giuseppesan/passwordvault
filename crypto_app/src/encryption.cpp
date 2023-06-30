#include "../include/encryption.hpp"

#define BUFFER 300
#define AES_BLOCK_SIZE 256

encryption::encryption(/* args */)
{
}

encryption::~encryption()
{
}

int aes_cbc(void)
{
    for (size_t i = 0; i <= 16; i++)
    {
        iv[i] = rand() % 256;
    }

    /* Message to be encrypted */
    string plain = "Super Secret Message";
    cout << "Input is: " << plain << endl;

    uint8_t *plain_text = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(plain.c_str()));
    uint8_t plaintext_buff[AES_BLOCK_SIZE] = {0};
    uint8_t cipher_text[AES_BLOCK_SIZE] = {0};
    uint8_t padded_frame[BUFFER] = {0};
    uint8_t decrypted_text[AES_BLOCK_SIZE] = {0};
    int plaintext_len = AES_BLOCK_SIZE;
    int ciphertext_len = 0;
    int ret = false;
    string hex_string = "";
    char *hex_array;
    string decrypted_string = "";

    if (plain.size() > AES_BLOCK_SIZE - 1)
    {
        cout << "plaintext size too big" << endl;
        return -1;
    }

    cout << "Input size = " << plain.size() << endl
         << endl;

    for (size_t i = 0; i < plain.size(); i++)
    {
        plaintext_buff[i] = plain_text[i];
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
    encryption COB;

    ret = COB.encrypt(plaintext_buff , plaintext_len, key, iv,
                      cipher_text, ciphertext_len);

    if (ret != true)
    {
        cout << "Encrypt failed" << endl;
        return -1;
    }

    ret = COB.prepare_ciphertext(1, iv, cipher_text, ciphertext_len, padded_frame, 277);

    if (ret != 0)
    {
        cout << "Preparing cipher_text failed" << endl;
        return -1;
    }

    std::cout << "Ciphertext is:\n";
    hex_string = to_hex(padded_frame, 277);

    cout << hex_string << endl;

    BIO_dump_fp(stdout, (const char *)padded_frame, 277);

    for (size_t i = 0; i < 256; i++)
    {
        cipher_text[i] = padded_frame[i + 21];
    }

    /*Decryption*/
    ret = COB.decrypt(cipher_text, ciphertext_len, key, iv,
                      decrypted_text, plaintext_len);

    if (ret != true)
    {
        cout << "Decrypt failed" << endl;
        return -1;
    }

    /* Show the decrypted text */
    std::cout << "\nDecrypted text is:" << endl;
    std::cout << decrypted_text << endl;

    decrypted_string.assign(decrypted_text, decrypted_text + plaintext_len);

    if (strcmp(plain.c_str(), decrypted_string.c_str()) != 0)
    {
        cout << "Input and Output strings do not match" << endl;
        return -1;
    }

    return 0;
}

bool encryption::encrypt(const unsigned char *plain_text, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len)
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

    if (EVP_EncryptUpdate(ctx, cipher_text, &ciphertext_len, plain_text, plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

bool encryption::decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plain_text, int &plaintext_len)
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

    if (EVP_DecryptUpdate(ctx, plain_text, &plaintext_len, cipher_text, ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }

    EVP_CIPHER_CTX_free(ctx);
    return true;
}

int encryption::prepare_ciphertext(uint8_t version, uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size,
                                      uint8_t *cipher_text_out, uint64_t ciph_text_out_size)
{
    /*copy version to byte 0*/
    cipher_text_out[0] = version;
    /*copy IV to bytes 1 to 16*/
    for (int i = 1; i < 17; i++)
    {
        cipher_text_out[i] = iv[i - 1];
    }
    /*add length of cipher_text from byte 17 to 20*/
    *(reinterpret_cast<uint32_t *>(cipher_text_out + 17)) = pl_ciph_text_size;

    if (pl_ciph_text_size > (ciph_text_out_size - 1 - 16 - 4))
    {
        cout << "buffer is too big or cipher array is too small" << endl;
        return -1;
    }
    /*copy cipher_text*/
    for (uint32_t j = 0; j < ciph_text_out_size; j++)
    {
        cipher_text_out[j + 21] = plain_cipher_text[j];
    }

    return 0;
}