#include "../include/router.hpp"

Router::Router()
{
    curr_path = "auth";
    message = "";
    console_available_commands();
}

void Router::handle_input()
{
    crypto cobj;
    int ret = -1;

    cout << "~~~ current path: " << curr_path << "\n\n";
    if (message != "")
    {
        cout << message << "\n\n";
    }

    cin >> input;

    if (input == "q" || input == "quit")
    {
        exit(0);
    }
    else if (input == "h" || input == "help")
    {
        console_available_commands();
    }
    else
    {
        handle_user(cobj);
    }
};

void Router::handle_user(crypto &cobj)
{
    int ret = -1;
    string u, p;
    int alg;
    if (input == "l" || input == "login")
    {
        cout << "Handle login \n";
        cout << "Username: \n";
        cin >> u;
        cobj.set_user(u);
        cout << "Password: \n";
        cin >> p;
        cobj.set_password(p);
        ret = cobj.check_login(cobj.get_user(), cobj.get_password());

        if (ret == 0)
        {
            cout << "Login successful\n";
            cout << "Logged in as " << cobj.get_user() << endl;
            handle_credentials(cobj);
        }
        else
        {
            cout << "Login failed\n";
        }
    }
    else if (input == "r" || input == "register")
    {
        cout << "Handle register \n";
        cout << "Username: \n";
        cin >> u;
        cobj.set_user(u);
        ret = cobj.check_user(cobj.get_user());
        if (ret == 0)
        {
            cout << "Password: \n";
            cin >> p;
            cobj.set_password(p);
            cout << "Algorithms [1]SHA256 [2]SHA512\n";
            cin >> alg;
            ret = cobj.register_user(cobj.get_user(), cobj.get_password(), alg);
        }
    }
    else
    {
        cout << "command '" << input << "' not available \nUse 'h' or 'help' to get a list of all available commands";
    }
}

void Router::handle_credentials(crypto &cobj)
{
    cout << "Handle Credentials \n";
    curr_path = "pw_manager";
    
    console_available_commands();
    cin >> input;
    
    if (input == "c" || input == "credential")
    {
        cobj.add_new_entry("Google", "my_user@gmail.com", "my_password");
    }
    else
    {
        cout << "command '" << input << "' not available \nUse 'h' or 'help' to get a list of all available commands";
    }
}

string Router::get_curr_path()
{
    return curr_path;
};

void Router::console_available_commands()
{
    if (curr_path == "auth")
    {
        cout << "Available commands are:\n";
        cout << "'r' or 'register' - to create an account\n";
        cout << "'l' or 'login' - to log into your existing account\n";
        cout << "'q' or 'quit' - to create an account\n\n";
        cout << "'h' or 'help' - to get a list of available commands\n\n";
    }
    else if (curr_path == "pw_manager")
    {
        cout << "Available commands are:\n";
        cout << "'get pw list' - to get a list of all passwords\n";
        cout << "'c' or 'credentials' - to manage your passwords/credentials\n";
        cout << "'create pw' - to create a new password\n";
        cout << "'get pw {password_name}' - to get your password\n";
        cout << "'change pw {password_name}' - to change your password\n";
        cout << "'delete pw {password_name}' - to delete your password\n\n";
        cout << "'q' or 'quit' - to create an account\n";
        cout << "'h' or 'help' - to get a list of available commands\n\n";
    }
}
