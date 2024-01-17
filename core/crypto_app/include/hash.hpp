#ifndef HASH_H
#define HASH_H

#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <random>
#include "../include/utils.hpp"
#include "encryption.hpp"

/**
 * @brief
 */
class hash
{
public:
    hash();
    ~hash();

    /**
     * @brief reads hashed password from file and compares it to input
     *
     * @param name Username
     * @param pw Password
     *
     * @return 0 if successful
     */
    int check_login(std::string name, std::string pw);

    /**
     * @brief writes the userdata in the defined format to the file <USERNAME>:<ALG>:<SALT>:<HASH>
     *
     * @param name Username
     * @param password Password
     * @param u_algorithm hash algorithm
     *
     * @return 0 if successful
     */
    int register_user(const std::string &name, const std::string &password, int u_algorithm);

    void set_user(std::string u) { user = u; }

    void set_password(std::string p) { password = p; }

    std::string get_user() { return user; }

    std::string get_password() { return password; }

    /**
     * @brief Creates a HMAC from the user entry in passwd
     * @param data user entry in passwd
     * @param key hmac key
     * @return the hash
     */
    std::string generate_HMAC(const std::string &data, const unsigned char *key);

    /**
     * @brief Checks if the user entry HMAC is == storedMAC
     * @param data user entry in passwd
     * @param key hmac key
     * @param storedMAC stored HMAC from encrypted credential entry
     * @return
     */
    bool verify_Integrity(const std::string &data, const unsigned char *key, const std::string &storedMAC);

private:
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 32 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_256(std::string in, std::string &out);
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 64 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_512(std::string in, std::string &out);

    /**
     * @brief creates the password hash
     * adds salts and pepper[fixed value] repeats it for number of iterations
     *
     * @param in password
     * @param salt random salt string
     * @param final_hash the hash string
     * @param iterations 10 times
     *
     * @return 0 if successful
     */
    int salt_n_hash(std::string in, std::string salt, std::string &final_hash, size_t iterations);

    /**
     * @brief Generate random salt from set of characters
     *
     * @param out generated salt
     *
     */
    void generate_salt(std::string &out);

    const int ITERATIONS = 10;
    const int ALGORITHM_SHA256 = 1;
    const int ALGORITHM_SHA512 = 2;
    const size_t salt_size = 16;

    std::string user = "";
    std::string password = "";
    int algorithm = 0;
};

#endif // HASH_H