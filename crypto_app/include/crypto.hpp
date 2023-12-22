#ifndef CRYPTO_H
#define CRYPTO_H

#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <random>
#include "../src/utils.cpp"

const int ITERATIONS = 10;
const int ALGORITHM_SHA256 = 1;
const int ALGORITHM_SHA512 = 2;
const size_t salt_size = 16;

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
    int register_user(const std::string & name, const std::string & password, int u_algorithm);

    /**
     * @brief checks if username is taken
     *
     * @param name username
     * @param action 1 find for register - 2 find for delete
     *
     * @return 0 if successful
     */
    int find_user(const std::string& name, int action);

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

    void set_user(std::string u) { user = u; }
    void set_password(std::string p) { password = p; }
    std::string get_user() { return user; }
    std::string get_password() { return password; }

    std::string passwd_path = "../secure/passwd";
    std::string logins_path = "../secure/logins";


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

    std::string user = "";
    std::string password = "";
    int algorithm = 0;
};

#endif // CRYPTO_H