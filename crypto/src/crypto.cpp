
#include "../include/crypto.hpp"

int save_password(string out)
{
    // TODO handle multiple entries & username has to be unique
    int ret = -1;
    ret = write_to_file(out, "sha");

    if (ret != 0)
    {
        return ret;
    }

    return ret;
}

int sha_256(string in, string &out)
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

    out = to_hex(hash_bytes_ptr, SHA512_DIGEST_LENGTH);

    return 0;
}

int salt_n_hash(string in, string salt, string &final_hash)
{
    int ret = -1;
    string first_hash = "";
    in = in + salt + PEPPER;
    ret = sha_256(in, first_hash);

    if (ret != 0)
    {
        return ret;
    }

    first_hash = first_hash + salt;
    ret = sha_256(first_hash, final_hash);

    if (ret != 0)
    {
        return ret;
    }

    return 0;
}

int check_password(string name, string pw)
{
    int ret = -1;
    string saved_hash = "";
    string salt = "";
    string final_hash = "";

    ret = read_from_file(saved_hash);

    if (ret != 0)
    {
        return ret;
    }

    saved_hash.erase(0, name.size() + 1);
    salt = saved_hash.substr(0, 16);
    saved_hash.erase(0, 17);

    ret = salt_n_hash(pw, salt, final_hash);

    if (ret != 0)
    {
        return ret;
    }

    ret = strcmp(saved_hash.c_str(), final_hash.c_str());
    cout << saved_hash << "\n"
         << final_hash << endl;

    if (ret != 0)
    {
        cout << "Password is not correct: " << ret << endl;
        return ret;
    }

    cout << "Password is correct" << endl;
    return 0;
}

int register_user(string name, string password)
{
    int ret = -1;
    string out = "";
    string salt = "";
    string final_hash = "";
    const string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    for (size_t i = 0; i < 16; ++i)
    {
        salt += CHARACTERS[distribution(generator)];
    }

    ret = salt_n_hash(password, salt, final_hash);

    if (ret != 0)
    {
        return ret;
    }

    out = name + ":" + salt + ":" + final_hash;
    ret = save_password(out);

    if (ret != 0)
    {
        return ret;
    }

    return 0;
}
