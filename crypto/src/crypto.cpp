
#include "../include/crypto.hpp"

int write_to_file(string in, string file)
{
    ofstream myfile(file);
    if (myfile.is_open())
    {
        myfile << in;
        myfile.close();
    }
    else
    {
        cout << "Unable to open file\n";
        return -1;
    }
    return 0;
}

int read_from_file(string &out)
{
    ifstream myfile("sha");
    if (myfile.is_open())
    {
        getline(myfile, out);
        myfile.close();
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }
    return 0;
}

int save_password(string out)
{
    int ret = -1;
    ret = write_to_file(out, "sha");

    if (ret != 0)
    {
        return ret;
    }
    return 0;
}

int sha_256(string in, string &out)
{
    int ret = -1;
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

int check_password(string name, string pw)
{
    int ret = -1;
    string saved_hash = "";
    string generated_hash = "";

    ret = sha_256(pw, generated_hash);

    ret = read_from_file(saved_hash);

    if (ret != 0)
    {
        return ret;
    }

    saved_hash.erase(0, name.size()+1);

    ret = strcmp(saved_hash.c_str(), generated_hash.c_str());
    cout << saved_hash << "\n"
         << generated_hash << endl;
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
    sha_256(password, out);
    out = name + ":" + out;
    save_password(out);
    return 0;
}
