#include "CLInterface.hpp"

CLInterface::CLInterface() : curr_path("auth"), logged_in_user("")
{
    handle_help();
}

CLInterface::~CLInterface() {}

void CLInterface::main_thread()
{
    hash hash_obj;

    // Display user information
    if (!logged_in_user.empty())
    {
        std::cout << "\n\nLogged in as " << logged_in_user << "\n";
    }
    std::cout << "\n\n~~~ current path: " << curr_path << "\n\n";

    // Get user input
    getline(std::cin, input);

    // Process user input
    utils::sanitize_input(input);

    // Process user input
    if (input == "q" || input == "quit")
    {
        exit(0);
    }
    else if (input == "h" || input == "help")
    {
        handle_help();
    }
    else
    {
        // Delegate to specific handlers based on the current path
        if (curr_path == "auth")
        {
            handle_startup(hash_obj);
        }
        else if (curr_path == "pw_manager")
        {
            handle_credentials();
        }
    }
}

void CLInterface::handle_startup(hash &hash_obj)
{
    if (input == "l" || input == "login")
    {
        handle_login(hash_obj);
    }
    else if ((utils::is_file_empty(passwd_path) == true) && (input == "r" || input == "register"))
    {
        handle_register(hash_obj);
    }
    else if (input == "logout" || input == "o")
    {
        handle_logout();
    }
    else
    {
        std::cout << "Command '" << input << "' not available \nUse 'h' or 'help' to get a list of all available commands";
    }
}

void CLInterface::handle_login(hash &hash_obj)
{
    std::cout << "Handle login \n";
    std::string u, p;
    std::cout << "Username: \n";
    getline(std::cin, u);
    utils::sanitize_input(u);
    hash_obj.set_user(u);

    std::cout << "Password: \n";
    getline(std::cin, p);
    utils::sanitize_input(p);
    hash_obj.set_password(p);

    int ret = hash_obj.check_login(hash_obj.get_user(), hash_obj.get_password());

    if (ret == 0)
    {
        logged_in_user = "Logged in as " + hash_obj.get_user() + "\n";
        curr_path = "pw_manager";
        std::cout << logged_in_user;
    }
    else
    {
        std::cerr << "Login failed\n";
    }
}

void CLInterface::handle_register(hash &hash_obj)
{
    std::cout << "Handle register \n";
    std::string u, p;
    std::cout << "New Username: \n";
    getline(std::cin, u);
    utils::sanitize_input(u);
    hash_obj.set_user(u);

    while (utils::find_entry(hash_obj.get_user(), passwd_path) == found)
    {
        std::cout << "Username is taken. Choose a new username:\n";
        getline(std::cin, u);
        utils::sanitize_input(u);
        hash_obj.set_user(u);
    }

    std::cout << "Password: \n";
    getline(std::cin, p);
    utils::sanitize_input(p);
    hash_obj.set_password(p);
/*
    int alg;
    std::cout << "Algorithms [1]SHA256 [2]SHA512\n";
    std::cin >> alg;
*/
    hash_obj.register_user(hash_obj.get_user(), hash_obj.get_password());
}

void CLInterface::handle_logout()
{
    curr_path = "auth";
    logged_in_user.clear();
}

void CLInterface::handle_credentials()
{
    if (input == "c" || input == "credential")
    {
        handle_credential_entry();
    }
    else if (input == "get credential" || input == "gc")
    {
        handle_get_credential();
    }
    else if (input == "logout" || input == "o")
    {
        handle_logout();
    }
    else
    {
        std::cout << "Command '" << input << "' not available \nUse 'h' or 'help' to get a list of all available commands";
    }
}

void CLInterface::handle_credential_entry()
{
    std::string user, password, tag;

    std::cout << "Username: \n";
    std::cin >> user;
    utils::sanitize_input(user);
    std::cout << "Password: \n";
    std::cin >> password;
    utils::sanitize_input(password);
    std::cout << "Tag: \n";
    std::cin >> tag;
    utils::sanitize_input(tag);
   
    while (utils::find_entry(user, credentials_path) == found)
    {
        std::cout << "Tag is taken. Choose a new Tag:\n";
        getline(std::cin, user);
    }

    encryption enc_obj;
    enc_obj.add_new_entry(tag, user, password);
}

void CLInterface::handle_get_credential()
{
    std::string entry;
    std::cout << "Tag: \n";
    std::cin >> entry;
    utils::sanitize_input(entry);
    encryption obj;
    std::string out;
    
    if (utils::read_from_file_and_find(out, entry, credentials_path) == found)
    {
        std::cout << "Credentials: " << std::endl;
        obj.decrypt_credentials(out);
    }
    else
    {
        std::cerr << "Credentials not found " << std::endl;
    }
}

void CLInterface::handle_help()
{
    if (curr_path == "auth")
    {
        std::cout << "Available commands are:\n";
        if (utils::is_file_empty(passwd_path) == true)
        {
            std::cout << "'r' or 'register' - to create an account\n";
        }
        std::cout << "'l' or 'login' - to log into your existing account\n";
        std::cout << "'q' or 'quit' - to exit\n\n";
        std::cout << "'h' or 'help' - to get a list of available commands\n\n";
    }
    else if (curr_path == "pw_manager")
    {
        std::cout << "Available commands are:\n";
        std::cout << "'gc' or 'get credential' - to get a specific login\n";
        std::cout << "'c' or 'credentials' - to add your credentials\n";
        std::cout << "'o' or 'logout' - to logout\n";
        // std::cout << "'create pw' - to create a new random password\n";
        // std::cout << "'change pw' - to change your password\n";
        // std::cout << "'delete credential' - to delete your password\n\n";
        std::cout << "'q' or 'quit' - to exit\n";
        std::cout << "'h' or 'help' - to get a list of available commands\n\n";
    }
}
