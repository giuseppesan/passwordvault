
#include "main.hpp"

int main()
{   
    //encrypt_decrypt_test();
    //string path = "secure/crypt";
    //decrypt_from_file_test(path);
    
    bool cli = true;
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