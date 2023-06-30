#include "../include/router.hpp"

Router::Router()
{
    curr_path = "auth";
    logged_in_user = "";
    console_available_commands();
}

void Router::handle_input()
{
    crypto cobj;
    int ret = -1;

    if (logged_in_user != "")
    {
        cout << "\n\nLogged in as " << logged_in_user << "\n";
    }
    cout << "\n\n~~~ current path: " << curr_path << "\n\n";

    getline(cin, input);
    // Comment this out instead of deleting it
    system("clear");

    // for debug purposes
    // cout <<input<<"\n\n";

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
        if (curr_path == "auth")
        {
            handle_user(cobj);
        } 
        else if (curr_path == "pw_manager")
        {
            handle_credentials(cobj);
        }
        
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
        getline(cin, u);
        cobj.set_user(u);
        cout << "Password: \n";
        getline(cin, p);
        cobj.set_password(p);
        ret = cobj.check_login(cobj.get_user(), cobj.get_password());

        if (ret == 0)
        {
            ostringstream oss;
            oss << "Logged in as " << cobj.get_user() << endl;
            // Once user is logged in, the curr_path switches from "auth" to "pw_manager", and therefore allowing him
            // to access the commands from "handle_credentials"
            curr_path = "pw_manager";
            logged_in_user = oss.str();
        }
        else
        {
            cout << "Login failed\n";
        }
    }
    else if (input == "r" || input == "register")
    {
        cout << "Handle register \n";
        cout << "New Username: \n";
        getline(cin, u);
        cobj.set_user(u);
        ret = cobj.check_user(cobj.get_user());

        while (ret == -1) //Username is already taken, re-prompt
        {
            getline(cin, u);
            cobj.set_user(u);
            ret = cobj.check_user(cobj.get_user());
        }

        if (ret == 0)
        {
            cout << "Password: \n";
            getline(cin, p);
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
