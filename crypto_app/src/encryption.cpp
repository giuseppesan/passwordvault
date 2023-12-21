#include "../include/encryption.hpp"

encryption::encryption()
{
}

encryption::~encryption()
{
}

int encryption::get_iv(unsigned char *iv, const uint8_t *cipher_text)
{
    for (int i = 0; i < 16; i++)
    {
        iv[i] = cipher_text[i];
    }
    return 0;
}

int encryption::handle_encryption(string plain, uint8_t *padded_cipher)
{
    encryption COB;
    uint8_t plaintext_buff[AES_BLOCK_SIZE] = {0}; // plaintext
    uint8_t cipher_text[AES_BLOCK_SIZE] = {0};    // encrypted text

    int ciphertext_len = 0;
    int ret = -1;

    string hex_string = ""; // final readable string

    for (size_t i = 0; i <= 16; i++)
    {
        iv[i] = rand() % 256;
    }

    cout << "Input is: " << plain << endl;
    /*Cast to fit in encrypt function*/
    uint8_t *plain_text = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(plain.c_str()));

    if (plain.size() > AES_BLOCK_SIZE - 1)
    {
        std::cerr << "plaintext size too big" << endl;
        return -1;
    }

    cout << "Input size = " << plain.size() << endl;
    /*Write plaintext to buff for padding*/
    for (size_t i = 0; i < plain.size(); i++)
    {
        plaintext_buff[i] = plain_text[i];
    }

    // Padding to 256 Bytes
    if (plain.size() < AES_BLOCK_SIZE)
    {
        for (size_t i = plain.size(); i < (AES_BLOCK_SIZE - plain.size()); i++)
        {
            plaintext_buff[i + plain.size()] = 0;
        }
    }

    /*Encryption*/
    ret = COB.encrypt(plaintext_buff, AES_BLOCK_SIZE, crypto_key, iv,
                      cipher_text, ciphertext_len);

    if (ret != true)
    {
        std::cerr << "Encrypt failed" << endl;
        return -1;
    }

    /*Adds Header to encrypted payload
    16 Bytes IV + 4 Bytes Length + 256 Bytes Payload*/
    ret = COB.prepare_ciphertext(iv, cipher_text, ciphertext_len, padded_cipher, 276);

    if (ret != 0)
    {
        std::cerr << "Preparing cipher_text failed" << endl;
        return -1;
    }

    hex_string = to_hex2(padded_cipher, 276);
    cout << "Ciphertext is:\n";
    cout << hex_string << endl;

    return 0;
}
// TODO: problem when processing saved hexstring -> decrypt fails
int encryption::handle_decryption(const uint8_t *cipher, string &decrypted_string)
{
    encryption COB;
    int ret = -1;
    uint8_t decrypted_text[AES_BLOCK_SIZE] = {0}; // plaintext
    uint8_t cipher_text[AES_BLOCK_SIZE] = {0};    // encrypted payload buffer
    int plaintext_len = 0;

    /*Writes payload to buffer*/
    for (size_t i = 0; i < 256; i++)
    {
        cipher_text[i] = cipher[i + 20];
    }

    /*Reads IV from Header*/
    get_iv(iv, cipher);

    /*Decryption*/
    ret = COB.decrypt(cipher_text, AES_BLOCK_SIZE, crypto_key, iv, decrypted_text, plaintext_len);

    if (ret != true)
    {
        std::cerr << "Decrypt failed" << endl;
        return -1;
    }

    /* Show the decrypted text */
    std::cout << "\nDecrypted text is:" << endl;
    std::cout << decrypted_text << "\n" <<endl;
    decrypted_string.assign(decrypted_text, decrypted_text + plaintext_len);

    return 0;
}

bool encryption::encrypt(const unsigned char *plain_text, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, crypto_key, iv) != 1)
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

bool encryption::decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *plain_text, int &plaintext_len)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, crypto_key, iv) != 1)
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

int encryption::prepare_ciphertext(uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size,
                                   uint8_t *cipher_text_out, uint64_t ciph_text_out_size)
{
    /*copy IV to bytes 0 to 15*/
    for (int i = 0; i < 16; i++)
    {
        cipher_text_out[i] = iv[i];
    }
    /*add length of cipher_text from byte 16 to 19*/
    *(reinterpret_cast<uint32_t *>(cipher_text_out + 16)) = pl_ciph_text_size;

    if (pl_ciph_text_size > (ciph_text_out_size - 16 - 4))
    {
        std::cerr << "buffer is too big or cipher array is too small" << endl;
        return -1;
    }
    /*copy cipher_text*/
    for (uint32_t j = 0; j < ciph_text_out_size; j++)
    {
        cipher_text_out[j + 20] = plain_cipher_text[j];
    }

    return 0;
}
