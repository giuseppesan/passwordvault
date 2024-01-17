
#include "../include/hash.hpp"

hash::hash() = default;


hash::~hash() = default;

int hash::sha_256(std::string in, std::string &out)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const unsigned char *plain_password = reinterpret_cast<const unsigned char *>(in.c_str());
    unsigned char *hash_bytes_ptr;
    std::string hex_hash_string = "";

    /* Generate Hash*/
    hash_bytes_ptr = SHA256(plain_password, in.size(), hash);

    if (hash_bytes_ptr == nullptr)
    {
        std::cerr << " NULL pointer" << std::endl;
        return -1;
    }

    out = utils::to_hex_sha(hash_bytes_ptr, SHA256_DIGEST_LENGTH);

    return 0;
}

int hash::sha_512(std::string in, std::string &out)
{
    unsigned char hash[SHA512_DIGEST_LENGTH];
    const unsigned char *plain_password = reinterpret_cast<const unsigned char *>(in.c_str());
    unsigned char *hash_bytes_ptr;
    std::string hex_hash_string = "";

    /* Generate Hash*/
    hash_bytes_ptr = SHA512(plain_password, in.size(), hash);

    if (hash_bytes_ptr == NULL)
    {
        std::cerr << " NULL pointer" << std::endl;
        return -1;
    }

    out = utils::to_hex_sha(hash_bytes_ptr, SHA512_DIGEST_LENGTH);

    return 0;
}

std::string hash::generateHMAC(const std::string& data, const unsigned char * key) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    const EVP_MD *digest = EVP_sha3_512();

    HMAC(digest, key, 32, reinterpret_cast<const unsigned char*>(data.c_str()), data.length(), hash, &hashLen);

    return utils::to_hex_sha(hash, hashLen);
}

bool hash::verifyIntegrity(const std::string& data, const unsigned char * key, const std::string& storedMAC) {
    std::string calculatedMAC = generateHMAC(data, key);
    return (calculatedMAC == storedMAC);
}

int hash::salt_n_hash(std::string in, std::string salt, std::string &final_hash, size_t iterations)
{
    if (algorithm != ALGORITHM_SHA256 && algorithm != ALGORITHM_SHA512)
    {
        std::cerr << "Algorithm is not implemented" << std::endl;
        return -1;
    }

    std::string buffer_hash = in + salt + PEPPER;

    int ret = -1;

    for (size_t i = 0; i < iterations; i++)
    {
        if (algorithm == 1)
        {
            ret = sha_256(buffer_hash, final_hash);
        }
        else if (algorithm == 2)
        {
            ret = sha_512(buffer_hash, final_hash);
        }

        if (ret != 0)
        {
            return ret;
        }

        // Skip adding salt in the last iteration
        buffer_hash = final_hash + salt;
    }
    return 0;
}

int hash::check_login(std::string name, std::string pw)
{
    int ret = -1;
    std::string saved_hash, salt, final_hash;

    ret = utils::read_from_file_and_find(saved_hash, name, passwd_path);

    if (ret == not_found)
    {
        std::cerr << "Username not found\n";
        return ret;
    }
    else if (ret != 0)
    {
        return ret;
    }
    
    // Erase name from string
    saved_hash.erase(0, name.size() + 1);

    // Get Algorithm and erase from string
    algorithm = stoi(saved_hash.substr(0, 1));
    saved_hash.erase(0, 2);

    // Get Salt and erase from string
    salt = saved_hash.substr(0, 16);
    saved_hash.erase(0, 17);

    // Check algorithm and perform corresponding hash verification
#ifdef HASH_DEBUG
    std::cout << (algorithm == 1 ? "SHA256" : "SHA512") << " detected" << std::endl;
#endif
    ret = salt_n_hash(pw, salt, final_hash, ITERATIONS);

    if (ret != 0)
    {
        return ret;
    }

    ret = strcmp(saved_hash.c_str(), final_hash.c_str());
#ifdef HASH_DEBUG
    std::cout << saved_hash << "\n"
              << final_hash << std::endl;
#endif
    if (ret != 0)
    {
        std::cerr << "Password is not correct" << std::endl;
        return ret;
    }

    std::cout << "Password is correct" << std::endl;

    return 0;
}

void hash::generate_salt(std::string &out)
{
    // Create random 16 byte salt from charset
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    for (size_t i = 0; i < salt_size; ++i)
    {
        out.push_back(CHARACTERS[distribution(generator)]);
    }
}

int hash::register_user(const std::string &name, const std::string &password, int u_algorithm)
{
    std::string salt, final_hash;
    std::stringstream ss;

    generate_salt(salt);
    algorithm = u_algorithm;

    if (salt_n_hash(password, salt, final_hash, ITERATIONS) != 0)
    {
        return -1;
    }

    ss << name << ":" << algorithm << ":" << salt << ":" << final_hash;
    std::string out = ss.str();

    return utils::write_to_file(out, passwd_path.c_str());
}


