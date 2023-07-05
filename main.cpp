
#include "main.hpp"

int main()
{   
    //encrypt_decrypt_test();
    //string path = "secure/crypt";
    //decrypt_from_file_test(path);
    Router router;
    //close application by typing "q" or "quit"
    while (true)
    {
        router.handle_input();
    }
    return 0;
}