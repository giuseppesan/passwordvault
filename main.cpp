
#include "main.hpp"

int main()
{   
    encrypt_decrypt_test();
    string path = "secure/encrypt";
    decrypt_from_file_test(path);
    hashing_test();
    
    bool cli = false;
    if (cli == true)
    { 
        Router router;
        while (true)
        {
            router.handle_input();
        }
    }
    return 0;
}