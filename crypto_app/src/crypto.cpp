
#include "../include/crypto.hpp"

crypto::crypto()
{
}

crypto::~crypto()
{
}

int crypto::sha_256(string in, string &out)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    const unsigned char *plain_password = reinterpret_cast<const unsigned char *>(in.c_str());
    unsigned char *hash_bytes_ptr;
    string hex_hash_string = "";

    /* Generate Hash*/
    hash_bytes_ptr = SHA256(plain_password, in.size(), hash);

    if (hash_bytes_ptr == NULL)
    {
        cout << " NULL pointer" << endl;
        return -1;
    }

    out = to_hex2(hash_bytes_ptr, SHA256_DIGEST_LENGTH);

    return 0;
}

int crypto::sha_512(string in, string &out)
{
    unsigned char hash[SHA512_DIGEST_LENGTH];
    const unsigned char *plain_password = reinterpret_cast<const unsigned char *>(in.c_str());
    unsigned char *hash_bytes_ptr;
    string hex_hash_string = "";

    /* Generate Hash*/
    hash_bytes_ptr = SHA512(plain_password, in.size(), hash);

    if (hash_bytes_ptr == NULL)
    {
        cout << " NULL pointer" << endl;
        return -1;
    }

    out = to_hex2(hash_bytes_ptr, SHA512_DIGEST_LENGTH);

    return 0;
}

int crypto::salt_n_hash(string in, string salt, string &final_hash, size_t iterations)
{
    if (algorithm != 1 && algorithm != 2)
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

int crypto::check_login(string name, string pw)
{
    int ret = -1;
    std::string saved_hash, salt, final_hash;

    ret = read_from_file_and_find(saved_hash, name, "secure/passwd");

    if (ret != 0)
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
    std::cout << (algorithm == 1 ? "SHA256" : "SHA512") << " detected" << std::endl;

    ret = salt_n_hash(pw, salt, final_hash, ITERATIONS);

    if (ret != 0)
    {
        return ret;
    }

    ret = strcmp(saved_hash.c_str(), final_hash.c_str());
    std::cout << saved_hash << "\n"
              << final_hash << std::endl;

    if (ret != 0)
    {
        std::cout << "Password is not correct: " << ret << std::endl;
        return ret;
    }

    std::cout << "Password is correct" << std::endl;

    return 0;
}

void crypto::generate_salt(std::string &out)
{
    // Create random 16 byte salt from charset
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    // Use append directly instead of concatenating characters
    for (size_t i = 0; i < 16; ++i)
    {
        out.push_back(CHARACTERS[distribution(generator)]);
    }
}

int crypto::register_user(const std::string &name, const std::string &password, int u_algorithm)
{
    std::string salt, final_hash;

    generate_salt(salt);
    algorithm = u_algorithm;
    std::string out = name + ":" + std::to_string(algorithm) + ":" + salt + ":";

    if (salt_n_hash(password, salt, final_hash, ITERATIONS) != 0)
    {
        return -1;
    }

    out += final_hash;
    return write_to_file(out, passwd_path.c_str());
}

int crypto::check_user(const std::string &name)
{
    ifstream my_file(passwd_path.c_str());
    std::string line = "";

    if (!my_file.is_open())
    {
        std::cerr << "Unable to open & read file\n";
        return -1;
    }

    while (getline(my_file, line))
    {
        if (line.substr(0, name.size()) == name)
        {
            cout << "Username is taken\n";
            my_file.close();
            return -1;
        }
    }

    cout << "Username is available\n";
    return 0;
}

int crypto::check_entry(const std::string &entry)
{
    std::string path = "secure/logins";
    ifstream my_file(path.c_str());
    std::string line = "";

    if (my_file.fail())
    {
        std::cerr << "Unable to open & read file\n";
        return -1;
    }

    while (getline(my_file, line))
    {
        if (line.substr(0, entry.size()) == entry)
        {
            cout << "Found credentials\n";
            my_file.close();
            return 0;
        }
    }
    cout << "No credentials found\n";
    return -1;
}

int crypto::add_new_entry(const std::string &tag, const std::string &user, const std::string &password)
{
    std::string credentials = tag + ":" + user + ":" + password;

    if (write_to_file(credentials, logins_path.c_str()) != 0)
    {
        std::cerr << "Failed to add new entry to file\n";
        return -1;
    }

    return 0;
}
