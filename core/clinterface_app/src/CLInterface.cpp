#include "../include/CLInterface.hpp"


CLInterface::CLInterface() : curr_path("auth"), logged_in_user("")
{
    handle_help();
}

void CLInterface::main_thread()
{
    hash cobj;

    // Display user information
    if (!logged_in_user.empty())
    {
        std::cout << "\n\nLogged in as " << logged_in_user << "\n";
    }
    std::cout << "\n\n~~~ current path: " << curr_path << "\n\n";

    // Get user input
    getline(std::cin, input);

    // Process user input
    sanitize_input(input);

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
            handle_startup(cobj);
        }
        else if (curr_path == "pw_manager")
        {
            handle_credentials(cobj);
        }
    }
}

void CLInterface::sanitize_input(std::string &input)
{
    for (char &ch : input)
    {
        if (!isprint(static_cast<unsigned char>(ch)))
        {
            // Replace non-printable characters with a space
            ch = ' ';
        }
    }

    // Trim leading and trailing whitespaces
    input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
    input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
}

void CLInterface::handle_startup(hash &cobj)
{
    if (input == "l" || input == "login")
    {
        handle_login(cobj);
    }
    else if (input == "r" || input == "register")
    {
        handle_register(cobj);
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

void CLInterface::handle_login(hash &cobj)
{
    std::cout << "Handle login \n";
    std::string u, p;
    std::cout << "Username: \n";
    getline(std::cin, u);
    cobj.set_user(u);

    std::cout << "Password: \n";
    getline(std::cin, p);
    cobj.set_password(p);

    int ret = cobj.check_login(cobj.get_user(), cobj.get_password());

    if (ret == 0)
    {
        logged_in_user = "Logged in as " + cobj.get_user() + "\n";
        curr_path = "pw_manager";
        std::cout << logged_in_user;
    }
    else
    {
        std::cerr << "Login failed\n";
    }
}

void CLInterface::handle_register(hash &cobj)
{
    std::cout << "Handle register \n";
    std::string u, p;
    std::cout << "New Username: \n";
    getline(std::cin, u);
    cobj.set_user(u);

    while (cobj.find_user(cobj.get_user(), 1) == -1)
    {
        std::cout << "Choose a new username:\n";
        getline(std::cin, u);
        cobj.set_user(u);
    }

    std::cout << "Password: \n";
    getline(std::cin, p);
    cobj.set_password(p);

    int alg;
    std::cout << "Algorithms [1]SHA256 [2]SHA512\n";
    std::cin >> alg;

    cobj.register_user(cobj.get_user(), cobj.get_password(), alg);
}

void CLInterface::handle_logout()
{
    curr_path = "auth";
    logged_in_user.clear();
}

void CLInterface::handle_credentials(hash &cobj)
{
    if (input == "c" || input == "credential")
    {
        handle_credential_entry(cobj);
    }
    else if (input == "get credential" || input == "gc")
    {
        handle_get_credential(cobj);
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

void CLInterface::handle_credential_entry(hash &cobj)
{
    std::string user, password, tag;

    std::cout << "Username: \n";
    std::cin >> user;
    std::cout << "Password: \n";
    std::cin >> password;
    std::cout << "Tag: \n";
    std::cin >> tag;

    cobj.add_new_entry(tag, user, password);
}

void CLInterface::handle_get_credential(hash &cobj)
{
    std::string entry;
    std::cout << "Tag: \n";
    std::cin >> entry;
    encryption obj;
    if (cobj.check_entry(entry) == 0)
    {
        std::string out;
        if (utils::read_from_file_and_find(out, entry, logins_path) == 0)
        {
            std::cout << "Credentials: " << std::endl;
            obj.decrypt_credentials(out);
        }
    }
}

void CLInterface::handle_help()
{
    if (curr_path == "auth")
    {
        std::cout << "Available commands are:\n";
        std::cout << "'r' or 'register' - to create an account\n";
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
