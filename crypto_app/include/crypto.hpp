#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <random>
#include "../src/utils.hpp"

#define ITERATIONS 10
#define ALGORITHM_SHA256 1
#define ALGORITHM_SHA512 2

class crypto
{
public:
    crypto();
    ~crypto();

    /**
     * @brief reads hashed password from file and compares it to input
     *
     * @param name Username
     * @param pw Password
     *
     * @return 0 if successful
     */
    int check_login(string name, string pw);

    /**
     * @brief writes the userdata in the defined format to the file <USERNAME>:<ALG>:<SALT>:<HASH>
     *
     * @param name Username
     * @param password Password
     * @param u_algorithm hash algorithm
     * 
     * @return 0 if successful
     */
    int register_user(const std::string & name, const std::string & password, int u_algorithm);

    /**
     * @brief checks if username is taken
     *
     * @param name username
     *
     * @return 0 if successful
     */
    int check_user(const std::string& name);

    /**
     * @brief add new login credentials
     * 
     * @param tag tag which the user searches for e.g. google
     * @param user username
     * @param password user password
     * 
     * @return 0 if successful
     */
    int add_new_entry(const std::string& tag, const std::string& user, const std::string& password);
    
    /**
     * @brief check for existing login credentials
     * 
     * @param name name-tag which the user searches for e.g. google
     * 
     * @return 0 if successful
    */
    int check_entry(const std::string& entry);

    void set_user(string u) { user = u; }
    void set_password(string p) { password = p; }
    string get_user() { return user; }
    string get_password() { return password; }

    string passwd_path = "secure/passwd";
    string logins_path = "secure/logins";

    const size_t SALT_SIZE = 16;

private:
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 32 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_256(string in, string &out);
    /**
     * @brief Turn string into hash
     *
     * @param in Plain input string
     * @param out 64 Byte Hex Hash
     *
     * @return 0 if successful
     */
    int sha_512(string in, string &out);

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
    int salt_n_hash(string in, string salt, string &final_hash, size_t iterations);

    /**
     * @brief Generate random salt from set of characters
     *
     * @param out generated salt
     *
     */
    void generate_salt(std::string &out);

    string user = "";
    string password = "";
    int algorithm = 0;
};

#endif // CRYPTO_H