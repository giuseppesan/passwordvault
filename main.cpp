
#include "main.hpp"

int main()
{
    int ret = -1;
    bool registration = false;
    string name = "Bob";
    string pw = "Secret";

    if (registration == true)
    {
        ret = check_user(name);

        if (ret != 0)
        {
            return -1;
        }

        cout << "Register User: " << name << " Password: " << pw << endl;
        ret = register_user(name, pw);

        if (ret != 0)
        {
            return -1;
        }
    }
    cout << "Login with User: " << name << " Password: " << pw << endl;
    ret = check_password(name, pw);

    if (ret != 0)
    {
        return -1;
    }

    return 0;
}