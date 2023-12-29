#include "../include/encryption.hpp"

encryption::encryption()
{
}

encryption::~encryption()
{
}

int encryption::get_iv(unsigned char *iv, const std::vector<uint8_t> &cipher_block)
{
    for (int i = 0; i < iv_size; i++)
    {
        iv[i] = cipher_block[i];
    }
    return 0;
}

int encryption::handle_encryption(const std::string plain_text, std::vector<uint8_t> &cipher_block)
{
    encryption encryption_obj;
    std::vector<uint8_t> plaintext_buff(AES_BLOCK_SIZE, 0);
    std::vector<uint8_t> cipher_payload(AES_BLOCK_SIZE+16, 0);

    int ciphertext_len = 0;
    int ret = -1;
    std::string hex_string = ""; // final readable string
    std::random_device rd;
    std::mt19937 generator(rd());

    for (size_t i = 0; i < iv_size; i++)
    {
        iv[i] = static_cast<uint8_t>(generator() % 256);
    }

    std::cout << "Input is: " << plain_text << std::endl;

    plaintext_buff.assign(plain_text.begin(), plain_text.end());

    if (plain_text.size() > AES_BLOCK_SIZE - 1)
    {
        std::cerr << "plaintext size too big" << std::endl;
        return -1;
    }

    std::cout << "Input size = " << plain_text.size() << std::endl;
    /*Encryption*/
    ret = encryption_obj.encrypt(plaintext_buff, AES_BLOCK_SIZE, crypto_key, iv,
                                 cipher_payload, ciphertext_len);

    if (ret != true)
    {
        std::cerr << "Encrypt failed" << std::endl;
        return -1;
    }

    /*Adds Header to encrypted payload
    16 Bytes IV + 256 Bytes Payload*/
    ret = encryption_obj.prepare_ciphertext(iv, cipher_payload, ciphertext_len, cipher_block, total_cipher_len);

    if (ret != 0)
    {
        std::cerr << "Preparing cipher_payload failed" << std::endl;
        return -1;
    }
    hex_string = to_hex(cipher_block);
    std::cout << "Ciphertext is:\n";
    std::cout << hex_string << std::endl;

    return 0;
}

int encryption::handle_decryption(const std::vector<uint8_t> &cipher_block, std::string &decrypted_string)
{
    encryption encryption_obj;
    int ret = -1;
    std::vector<uint8_t> decrypted_text(AES_BLOCK_SIZE, 0);
    std::vector<uint8_t> cipher_payload(AES_BLOCK_SIZE, 0);
    int plaintext_len = 0;

    // Extract payload from cipher_block
    if (cipher_block.size() >= iv_size + payload_size)
    {
        std::copy(cipher_block.begin() + iv_size, cipher_block.begin() + iv_size + AES_BLOCK_SIZE, cipher_payload.begin());
    }
    else
    {
        // Handle the case where the cipher_block does not have enough data
        std::cerr << "Error: Insufficient data in the cipher.\n";
        return -1;
    }

    /*Reads IV from Header*/
    get_iv(iv, cipher_block);

    /*Decryption*/
    ret = encryption_obj.decrypt(cipher_payload, AES_BLOCK_SIZE, crypto_key, iv, decrypted_text, plaintext_len);

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

bool encryption::encrypt(const std::vector<uint8_t> &plain_text, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &cipher_payload, int &ciphertext_len)
{
    int final_len = 0;
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

    if (EVP_EncryptUpdate(ctx.get(), cipher_payload.data(), &ciphertext_len, plain_text.data(), plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        return false;
    }

    if (EVP_EncryptFinal_ex(ctx.get(), cipher_payload.data() + ciphertext_len, &final_len) != 1)
    {
        std::cerr << "Failed to finalize encrypt" << std::endl;
        return false;
    }

    ciphertext_len += final_len;

    return true;
}

bool encryption::decrypt(const std::vector<uint8_t> &cipher_payload, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &plain_text, int &plaintext_len)
{
    int final_len = 0;
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

    if (EVP_DecryptUpdate(ctx.get(), plain_text.data(), &plaintext_len, cipher_payload.data(), ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        return false;
    }

    /*
    int ret = EVP_DecryptFinal_ex(ctx.get(), plain_text.data() + plaintext_len, &final_len);
    if (ret != 1)
    {
        std::cerr << "Failed to finalize decrypt: " << std::endl;
        ERR_print_errors_fp(stderr);
        return false;
    }
    */
    plaintext_len += final_len;
    return true;
}

int encryption::prepare_ciphertext(const uint8_t *iv, const std::vector<uint8_t> &plain_cipher_text, uint64_t pl_ciph_text_size,
                                   std::vector<uint8_t> &cipher_block, uint64_t ciph_text_out_size)
{
    /*copy IV to bytes 0 to 15*/
    for (int i = 0; i < iv_size; i++)
    {
        cipher_block.push_back(iv[i]);
    }

    /*copy cipher_payload*/
    for (uint32_t j = 0; j < ciph_text_out_size; j++)
    {
        cipher_block.push_back(plain_cipher_text[j]);
    }

    return 0;
}
