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

    if (logged_in_user != "")
    {
        cout << "\n\nLogged in as " << logged_in_user << "\n";
    }
    cout << "\n\n~~~ current path: " << curr_path << "\n\n";

    getline(cin, input);

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

        while (ret == -1) // Username is already taken, re-prompt
        {
            cout << "Choose an new username:\n";
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
            //TODO:
            /*if (alg != "1" && alg != "2") 
            {
                cout << "Invalid choice. Please choose either 1 or 2 for the algorithm.\n";
            }*/
            ret = cobj.register_user(cobj.get_user(), cobj.get_password(), alg);
        }
    }
    else if (input == "logout" || input == "o")
    {
        curr_path = "auth";
        logged_in_user = "";
    }
    else
    {
        cout << "command '" << input << "' not available \nUse 'h' or 'help' to get a list of all available commands";
    }
}

void Router::handle_credentials(crypto &cobj)
{
    string user, password, tag;
    int ret = -1;

    if (input == "c" || input == "credential")
    {
        cout << "Username: \n";
        cin >> user;
        cout << "Password: \n";
        cin >> password;
        cout << "Tag: \n";
        cin >> tag;
        ret = cobj.add_new_entry(tag, user, password);
        if (ret != 0)
        {
            cout << "adding new entry failed\n";
        }
        cout << "credentials were added\n";
    }
    else if (input == "get credential" || input == "gc")
    {

        string entry = "";
        string out = "";
        cout << "Tag: \n";
        cin >> entry;
        ret = cobj.check_entry(entry);

        if (ret != 0)
        {
            cout << "Nothing found" << endl;
        }

        ret = read_from_file_and_find(out, entry, "secure/logins");

        if (ret != 0)
        {
            cout << "Nothing found" << endl;
        }

        cout << "Credentials: " << out << endl;
    }
    else if (input == "logout" || input == "o")
    {
        curr_path = "auth";
        logged_in_user = "";
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
        // cout << "'get pw list' - to get a list of all passwords\n";
        cout << "'gc' or 'get credential' - to get a specific login\n";
        cout << "'c' or 'credentials' - to manage your passwords/credentials\n";
        cout << "'o' or 'logout' - to logout\n";
        // cout << "'create pw' - to create a new password\n";
        // cout << "'get pw {password_name}' - to get your password\n";
        // cout << "'change pw {password_name}' - to change your password\n";
        // cout << "'delete pw {password_name}' - to delete your password\n\n";
        cout << "'q' or 'quit' - to create an account\n";
        cout << "'h' or 'help' - to get a list of available commands\n\n";
    }
}
