#include "../include/router.hpp"

Router::Router()
{
    curr_path = "auth";
    message = "";
}

void Router::handle_input()
{
    cout<<"~~~ current path: "<<curr_path<<"\n\n";
    if (message != "")
    {
        cout<<message<<"\n\n";
    }

    getline(cin, input);
    system("clear");
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
        // Router functions
        if (input == "l" || input == "login")
        {
            cout<<"Handle login \n";
        }
        else if (input == "r" || input == "register")
        {
            cout<<"Handle register \n";
        }
        else 
        {
            cout<<"command '"<<input<<"' not available \nUse 'h' or 'help' to get a list of all available commands";
        }
        
    }
};

string Router::get_curr_path()
{
    return curr_path;
};

void Router::console_available_commands()
{
    if (curr_path == "auth")
    {
        cout<<"Available commands are:\n";
        cout<<"'r' or 'register' - to create an account\n";
        cout<<"'l' or 'login' - to log into your existing account\n";
        cout<<"'q' or 'quit' - to create an account\n\n";
        cout<<"'h' or 'help' - to get a list of available commands\n\n";
    }
    else if (curr_path == "pw_manager")
    {
        cout<<"Available commands are:\n";
        cout<<"'get pw list' - to get a list of all passwords\n";
        cout<<"'create pw' - to create a new password\n";
        cout<<"'get pw {password_name}' - to get your password\n";
        cout<<"'change pw {password_name}' - to change your password\n";
        cout<<"'delete pw {password_name}' - to delete your password\n\n";
        cout<<"'q' or 'quit' - to create an account\n";
        cout<<"'h' or 'help' - to get a list of available commands\n\n";
    }
}
