
#include "main.hpp"

int main()
{
    int ret = -1;
    bool registration = false;
    string name = "Alice";
    string pw = "Secret";

    crypto cobj;

    if (registration == true)
    {
        ret = cobj.crypto::check_user(name);

        if (ret != 0)
        {
            return -1;
        }

        cout << "Register User: " << name << " Password: " << pw << endl;
        ret = cobj.crypto::register_user(name, pw);

        if (ret != 0)
        {
            return -1;
        }
    }
    cout << "Login with User: " << name << " Password: " << pw << endl;
    ret = cobj.crypto::check_password(name, pw);

    if (ret != 0)
    {
        return -1;
    }

    return 0;
}