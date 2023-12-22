#include "../include/encryption.hpp"

encryption::encryption()
{
}

encryption::~encryption()
{
}

int encryption::get_iv(unsigned char *iv, const uint8_t *cipher_text)
{
    for (int i = 0; i < iv_size; i++)
    {
        iv[i] = cipher_text[i];
    }
    return 0;
}

int encryption::handle_encryption(std::string plain, uint8_t *padded_cipher)
{
    encryption encryption_obj;
    std::vector<uint8_t> plaintext_buff(AES_BLOCK_SIZE, 0);
    std::vector<uint8_t> cipher_text(AES_BLOCK_SIZE, 0);
    int ciphertext_len = 0;
    int ret = -1;
    std::string hex_string = ""; // final readable string
    std::random_device rd;
    std::mt19937 generator(rd());

    for (size_t i = 0; i < iv_size; i++)
    {
        iv[i] = static_cast<uint8_t>(generator() % 256);
    }

    std::cout << "Input is: " << plain << std::endl;
    /*Cast to fit in encrypt function*/
    uint8_t *plain_text = const_cast<uint8_t *>(reinterpret_cast<const uint8_t *>(plain.c_str()));

    if (plain.size() > AES_BLOCK_SIZE - 1)
    {
        std::cerr << "plaintext size too big" << std::endl;
        return -1;
    }

    std::cout << "Input size = " << plain.size() << std::endl;
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
    ret = encryption_obj.encrypt(plaintext_buff.data(), AES_BLOCK_SIZE, crypto_key, iv,
                                 cipher_text.data(), ciphertext_len);

    if (ret != true)
    {
        std::cerr << "Encrypt failed" << std::endl;
        return -1;
    }

    /*Adds Header to encrypted payload
    16 Bytes IV + 4 Bytes Length + 256 Bytes Payload*/
    ret = encryption_obj.prepare_ciphertext(iv, cipher_text.data(), ciphertext_len, padded_cipher, total_cipher_len);

    if (ret != 0)
    {
        std::cerr << "Preparing cipher_text failed" << std::endl;
        return -1;
    }

    hex_string = to_hex(padded_cipher, 276);
    std::cout << "Ciphertext is:\n";
    std::cout << hex_string << std::endl;

    return 0;
}

int encryption::handle_decryption(const uint8_t *cipher, std::string &decrypted_string)
{
    encryption encryption_obj;
    int ret = -1;
    std::vector<uint8_t> decrypted_text(AES_BLOCK_SIZE, 0);
    std::vector<uint8_t> cipher_text(AES_BLOCK_SIZE, 0);
    int plaintext_len = 0;

    // Extract payload from cipher
    std::copy(cipher + header_size, cipher + header_size + payload_size, cipher_text.begin());

    /*Reads IV from Header*/
    get_iv(iv, cipher);

    /*Decryption*/
    ret = encryption_obj.decrypt(cipher_text.data(), AES_BLOCK_SIZE, crypto_key, iv, decrypted_text.data(), plaintext_len);

    if (ret != 1)
    {
        std::cerr << "Decrypt failed" << std::endl;
        return -1;
    }

    /* Show the decrypted text */
    std::cout << "\nDecrypted text is:" << std::endl;
    std::cout << decrypted_text.data() << "\n"
              << std::endl;

    decrypted_string.assign(decrypted_text.data(), decrypted_text.data() + plaintext_len);

    return 0;
}

bool encryption::encrypt(const unsigned char *plain_text, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *cipher_text, int &ciphertext_len)
{
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, crypto_key, iv) != 1)
    {
        std::cerr << "Failed to initialize encryption" << std::endl;
        return false;
    }

    if (EVP_EncryptUpdate(ctx.get(), cipher_text, &ciphertext_len, plain_text, plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        return false;
    }

    return true;
}

bool encryption::decrypt(const unsigned char *cipher_text, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, unsigned char *plain_text, int &plaintext_len)
{
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);
    if (!ctx)
    {
        std::cerr << "Failed to create EVP_CIPHER_CTX" << std::endl;
        return false;
    }

    if (EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, crypto_key, iv) != 1)
    {
        std::cerr << "Failed to initialize decryption" << std::endl;
        return false;
    }

    if (EVP_DecryptUpdate(ctx.get(), plain_text, &plaintext_len, cipher_text, ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        return false;
    }

    return true;
}

int encryption::prepare_ciphertext(uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size,
                                   uint8_t *cipher_text_out, uint64_t ciph_text_out_size)
{
    /*copy IV to bytes 0 to 15*/
    for (int i = 0; i < iv_size; i++)
    {
        cipher_text_out[i] = iv[i];
    }
    /*add length of cipher_text from byte 16 to 19*/
    *(reinterpret_cast<uint32_t *>(cipher_text_out + iv_size)) = pl_ciph_text_size;

    if (pl_ciph_text_size > (ciph_text_out_size - iv_size - payload_byte_size))
    {
        std::cerr << "buffer is too big or cipher array is too small" << std::endl;
        return -1;
    }
    /*copy cipher_text*/
    for (uint32_t j = 0; j < ciph_text_out_size; j++)
    {
        cipher_text_out[j + header_size] = plain_cipher_text[j];
    }

    return 0;
}
