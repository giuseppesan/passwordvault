
#include "main.hpp"

int main()
{   
    en_decrypt_test();
    hashing_test();
    
    bool cli = false;
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