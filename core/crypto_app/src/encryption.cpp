#include "encryption.hpp"

encryption::encryption()
{
}

encryption::~encryption()
{
}

int encryption::get_iv(unsigned char *iv, const std::vector<uint8_t> &cipher_block)
{
    for (int i = 0; i < IV_SIZE; i++)
    {
        iv[i] = cipher_block[i];
    }
    return 0;
}

int encryption::handle_encryption(const std::string plain_text, std::vector<uint8_t> &cipher_block)
{
    encryption encryption_obj;
    std::vector<uint8_t> plaintext_buff(AES_BLOCK_SIZE, 0);
    std::vector<uint8_t> cipher_payload(AES_BLOCK_SIZE, 0);

    int ciphertext_len = 0;
    int ret = -1;
    std::string hex_string = ""; // final readable string
    std::random_device rd;
    std::mt19937 generator(rd());

    if (plain_text.empty())
    {
        std::cerr << "plain_text is empty\n";
        return -1;
    }

    if (plain_text.size() > AES_BLOCK_SIZE / 2)
    {
        std::cerr << "plaintext size too big" << std::endl;
        return -1;
    }

    for (size_t i = 0; i < IV_SIZE; i++)
    {
        iv[i] = static_cast<uint8_t>(generator() % 256);
    }
#ifdef ENCRYPTION_DEBUG
    std::cout << "Input is: " << plain_text << std::endl;
#endif
    try
    {
        plaintext_buff.assign(plain_text.begin(), plain_text.end());
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during plaintext_buff assignment: " << e.what() << std::endl;
        return -1;
    }
#ifdef ENCRYPTION_DEBUG
    std::cout << "Input size = " << plain_text.size() << std::endl;
#endif
    unsigned char pbkdf2[32] = {0};
    utils::readKeyFromFile(encryption_key_path, pbkdf2);
    /*Encryption*/
    ret = encryption_obj.encrypt(plaintext_buff, AES_BLOCK_SIZE, pbkdf2, iv,
                                 cipher_payload, ciphertext_len);

    if (ret != true)
    {
        std::cerr << "Encrypt failed" << std::endl;
        return -1;
    }

    /*Adds Header to encrypted payload
    16 Bytes IV + 256 Bytes Payload*/
    ret = encryption_obj.prepare_ciphertext(iv, cipher_payload, cipher_block);

    if (ret != 0)
    {
        std::cerr << "Preparing cipher_payload failed" << std::endl;
        return -1;
    }

    hex_string = utils::to_hex(cipher_block);

#ifdef ENCRYPTION_DEBUG
    std::cout << "Ciphertext is:\n";
    std::cout << hex_string << std::endl;
#endif
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
    if (cipher_block.size() == IV_SIZE + AES_BLOCK_SIZE)
    {
        size_t destIndex = 0;
        for (size_t i = IV_SIZE; i < AES_BLOCK_SIZE; ++i)
        {
            cipher_payload[destIndex++] = cipher_block[i];
        }
    }
    else
    {
        std::cerr << "Error: Malformed cipher block.\n";
        return -1;
    }

    /*Reads IV from Header*/
    get_iv(iv, cipher_block);
    unsigned char pbkdf2[32] = {0};
    utils::readKeyFromFile(encryption_key_path, pbkdf2);
    /*Decryption*/
    ret = encryption_obj.decrypt(cipher_payload, AES_BLOCK_SIZE, pbkdf2, iv, decrypted_text, plaintext_len);

    if (ret != 1)
    {
        std::cerr << "Decrypt failed" << std::endl;
        return -1;
    }

    /* Show the decrypted text */
#ifdef ENCRYPTION_DEBUG
    std::cout << "\nDecrypted text is:" << std::endl;
    std::cout << decrypted_text.data() << "\n\n";
#endif
    try
    {
        decrypted_string.assign(decrypted_text.data(), decrypted_text.data() + plaintext_len);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during decrypted_string assignment: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

bool encryption::encrypt(const std::vector<uint8_t> &plain_text, int plaintext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &cipher_payload, int &ciphertext_len)
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

    if (EVP_EncryptUpdate(ctx.get(), cipher_payload.data(), &ciphertext_len, plain_text.data(), plaintext_len) != 1)
    {
        std::cerr << "Failed to encrypt data" << std::endl;
        return false;
    }

    return true;
}

bool encryption::decrypt(const std::vector<uint8_t> &cipher_payload, int ciphertext_len, const unsigned char *crypto_key, const unsigned char *iv, std::vector<uint8_t> &plain_text, int &plaintext_len)
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

    if (EVP_DecryptUpdate(ctx.get(), plain_text.data(), &plaintext_len, cipher_payload.data(), ciphertext_len) != 1)
    {
        std::cerr << "Failed to decrypt data" << std::endl;
        return false;
    }

    return true;
}

int encryption::prepare_ciphertext(const uint8_t *iv, const std::vector<uint8_t> &cipher_text,
                                   std::vector<uint8_t> &cipher_block)
{
    /*copy IV to bytes 0 to 15*/
    for (int i = 0; i < IV_SIZE; i++)
    {
        cipher_block.push_back(iv[i]);
    }

    /*copy cipher_payload*/
    for (uint32_t j = 0; j < AES_BLOCK_SIZE; j++)
    {
        cipher_block.push_back(cipher_text[j]);
    }

    return 0;
}

int encryption::decrypt_credentials(std::string &credentials)
{
    std::vector<uint8_t> cipher_block;
    std::vector<uint8_t> hmac_block;
    std::string password;
    std::string tag_and_username;
    std::string plain_password;
    std::string hmac;
    std::string hmac_plain;
    size_t firstColonPos = credentials.find(':');
    size_t secondColonPos = credentials.find(':', firstColonPos + 1);
    size_t thirdColonPos = credentials.find(':', secondColonPos + 1);
    std::string data;
    hash hash_obj;

    if (firstColonPos != std::string::npos && secondColonPos != std::string::npos)
    {
        tag_and_username = credentials.substr(0, secondColonPos + 1);
        password = credentials.substr(secondColonPos + 1, thirdColonPos - secondColonPos - 1);
        hmac = credentials.substr(thirdColonPos + 1);

        utils::hex2bin(hmac.c_str(), hmac_block);

        if (handle_decryption(hmac_block, hmac_plain) != 0)
        {
            return -1;
        }

        if (utils::return_user(data, passwd_path) != 0)
        {
            std::cerr << "Reading userdata went wrong\n";
            return -1;
        }

        if (hash_obj.verify_Integrity(data, hmac_key, hmac_plain) != true)
        {
            std::cerr << "Integrity check failed!\n";
            return -1;
        }

        utils::hex2bin(password.c_str(), cipher_block);

        if (handle_decryption(cipher_block, plain_password) != 0)
        {
            return -1;
        }

        std::cout << tag_and_username << plain_password << std::endl;
    }
    else
    {
        std::cerr << "Malformed string" << std::endl;
        return -2;
    }
    return 0;
}

int encryption::decrypt_credentials(std::string &credentials, std::string &username_out, std::string &password_out, std::string &tag_out)
{
    std::vector<uint8_t> cipher_block;
    std::vector<uint8_t> hmac_block;
    std::string password;
    std::string username;
    std::string tag;
    std::string plain_password;
    std::string hmac;
    std::string hmac_plain;
    size_t firstColonPos = credentials.find(':');
    size_t secondColonPos = credentials.find(':', firstColonPos + 1);
    size_t thirdColonPos = credentials.find(':', secondColonPos + 1);
    std::string data;
    hash hash_obj;

    if (firstColonPos != std::string::npos && secondColonPos != std::string::npos)
    {
        tag = credentials.substr(0, firstColonPos);
        username = credentials.substr(firstColonPos + 1, secondColonPos - firstColonPos - 1);
        password = credentials.substr(secondColonPos + 1, thirdColonPos - secondColonPos - 1);
        hmac = credentials.substr(thirdColonPos + 1);

        utils::hex2bin(hmac.c_str(), hmac_block);

        if (handle_decryption(hmac_block, hmac_plain) != 0)
        {
            return -1;
        }

        if (utils::return_user(data, passwd_path) != 0)
        {
            std::cerr << "Reading userdata went wrong\n";
            return -1;
        }

        if (hash_obj.verify_Integrity(data, hmac_key, hmac_plain) != true)
        {
            std::cerr << "Integrity check failed!\n";
            return -1;
        }

        utils::hex2bin(password.c_str(), cipher_block);

        if (handle_decryption(cipher_block, plain_password) != 0)
        {
            return -1;
        }

        username_out = username; 
        password_out = plain_password; 
        tag_out = tag;
    }
    else
    {
        std::cerr << "Malformed string" << std::endl;
        return -2;
    }
    return 0;
}

int encryption::encrypt_credentials(std::string &password)
{
    std::vector<uint8_t> cipher_block;
    encryption obj;

    if (obj.handle_encryption(password, cipher_block) != 0)
    {
        return -1;
    }

    password = utils::to_hex(cipher_block);
    return 0;
}

int encryption::add_new_entry(const std::string &tag, const std::string &user, const std::string &password)
{
    std::string encrypted_password = password;
    std::string data;
    std::string hmac_block;
    encryption encrypt_obj;
    hash hash_obj;
    std::vector<uint8_t> encrypt_block;

    encrypt_obj.encrypt_credentials(encrypted_password);

    if (utils::return_user(data, passwd_path) != 0)
    {
        std::cerr << "Reading userdata went wrong\n";
        return -1;
    }

    std::string hmac = hash_obj.generate_HMAC(data, hmac_key);

    encrypt_obj.handle_encryption(hmac, encrypt_block);
    hmac_block = utils::to_hex(encrypt_block);

    std::string credentials = tag + ":" + user + ":" + encrypted_password + ":" + hmac_block;

    if (utils::write_to_file(credentials, credentials_path.c_str()) != 0)
    {
        std::cerr << "Failed to add new entry to file\n";
        return -1;
    }
    std::cout << "credentials were added\n";

    return 0;
}

void encryption::pbkdf2(const std::string& password, const std::string& salt, int iterations, const std::string file_path) {
    // Convert password and salt to C-style strings
    const char* password_cstr = password.c_str();
    const char* salt_cstr = salt.c_str();
    unsigned char key[KEY_LENGTH];

    // Derive the key using PBKDF2
    PKCS5_PBKDF2_HMAC(password_cstr, -1, reinterpret_cast<const unsigned char*>(salt_cstr), -1,
                      iterations, EVP_sha256(), KEY_LENGTH, key);
    
    std::ofstream key_file(file_path, std::ios::binary);

    if (!key_file.is_open()) {
        std::cerr << "Error opening key file for writing" << std::endl;
        return;
    }

    key_file.write(reinterpret_cast<const char*>(key), KEY_LENGTH);
    key_file.close();
}
