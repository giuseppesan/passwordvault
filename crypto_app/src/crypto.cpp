
#include "../include/crypto.hpp"

crypto::crypto() {}
crypto::~crypto() {}

int crypto::save_password(string out)
{
    int ret = -1;
    ret = write_to_file(out, "passwd");

    if (ret != 0)
    {
        return ret;
    }

    return ret;
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

    out = to_hex(hash_bytes_ptr, SHA256_DIGEST_LENGTH);

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

    out = to_hex(hash_bytes_ptr, SHA512_DIGEST_LENGTH);

    return 0;
}

int crypto::salt_n_hash(string in, string salt, string &final_hash, size_t iterations)
{
    int ret = -1;
    string buffer_hash = "";

    in = in + salt + PEPPER;

    if (algorithm == 1)
    {
        ret = sha_256(in, buffer_hash);
    }
    else if (algorithm == 2)
    {
        ret = sha_512(in, buffer_hash);
    }
    else
    {
        cout << "Algorithm is not implemented" << endl;
        return -1;
    }

    if (ret != 0)
    {
        return ret;
    }

    /*Does the cycle 10 times*/
    for (size_t i = 0; i < iterations; i++)
    {
        buffer_hash += salt;
        if (algorithm == 1)
        {
            ret = sha_256(buffer_hash, final_hash);
        }

        if (algorithm == 2)
        {
            ret = sha_512(buffer_hash, final_hash);
        }

        if (ret != 0)
        {
            return ret;
        }
        buffer_hash = final_hash;
    }

    return 0;
}

int crypto::check_login(string name, string pw)
{
    int ret = -1;
    string saved_hash = "";
    string salt = "";
    string final_hash = "";

    ret = read_from_file(saved_hash, name);

    if (ret != 0)
    {
        return ret;
    }

    /*Erase name from string*/
    saved_hash.erase(0, name.size() + 1);

    /*Get Algorithm  and erase from string*/
    algorithm = stoi(saved_hash.substr(0, 1));

    saved_hash.erase(0, 2);
    /*Get Salt and erase from string*/
    salt = saved_hash.substr(0, 16);
    saved_hash.erase(0, 17);
    if (algorithm == 1)
    {
        cout << "SHA256 detected" << endl;

        ret = salt_n_hash(pw, salt, final_hash, ITERATIONS);

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
    }

    if (algorithm == 2)
    {
        cout << "SHA512 detected" << endl;

        ret = salt_n_hash(pw, salt, final_hash, ITERATIONS);

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
    }
    return 0;
}

void crypto::generate_salt(string &out)
{
    /*Create random 16 byte salt from charset*/
    const string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    random_device random_device;
    mt19937 generator(random_device());
    uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    for (size_t i = 0; i < 16; ++i)
    {
        out += CHARACTERS[distribution(generator)];
    }
}

int crypto::register_user(string name, string password, int u_algorithm)
{
    int ret = -1;
    string out = "";
    string salt = "";
    string final_hash = "";

    generate_salt(salt);
    algorithm = u_algorithm;
    ret = salt_n_hash(password, salt, final_hash, ITERATIONS);

    if (ret != 0)
    {
        return ret;
    }

    out = name + ":" + to_string(algorithm) + ":" + salt + ":" + final_hash;
    ret = save_password(out);

    if (ret != 0)
    {
        return ret;
    }

    return 0;
}

int crypto::check_user(string name)
{
    int ret = -1;
    ifstream my_file("passwd");
    string check = "";
    string buff = "";

    if (my_file.is_open())
    {
        while (getline(my_file, buff))
        {
            check = buff.substr(0, name.size());

            if (strcmp(check.c_str(), name.c_str()) == 0)
            {
                cout << "Username is taken\n";
                my_file.close();
                return -1;
            }
        }
        cout << "Username is available\n";
        return 0;
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }
}

int crypto::add_new_entry(string tag, string user, string password)
{
    int ret = -1;
    string credentials = tag + ":" + user + ":" + password;
    ret = write_to_file(credentials, "logins");
    
    if (ret != 0)
    {
        return -1;
    }

    return 0;
}
