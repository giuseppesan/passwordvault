
#include "main.hpp"

int main()
{   
    encrypt_decrypt_test();
    string path = "secure/encrypt";
    decrypt_from_file_test(path);
    hashing_test();
    
    bool cli = true;
    if (cli == true)
    { 
        CLInterface cli;
        while (true)
        {
            cli.main_thread();
        }
    }
    return 0;
}