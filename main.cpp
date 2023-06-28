
#include "main.hpp"

int main()
{
    int ret = -1;
    string name = "Bob";
    string pw = "Secret";

    ret = register_user(name, pw);
    if (ret != 0)
    {
        return -1;
    }

    ret = check_password(name, pw);
    if (ret != 0)
    {
        return -1;
    }

    /*Encryption*/
    //aes_cbc();
    return 0;
}