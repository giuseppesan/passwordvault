#ifndef CLINTERFACE_H
#define CLINTERFACE_H

#include <iostream>
#include <string>
#include <sstream>
#include "../../crypto_app/include/hash.hpp"
#include "../../crypto_app/include/utils.hpp"

class CLInterface
{
private:
    std::string curr_path;
    std::string input;
    std::string logged_in_user;

public:
    /**
     * @brief main loop for CLI program - starts with auth and handle user input
     */
    void main_thread();

    /**
     * @brief prints the options depending on the current path
     */
    void handle_help();

    /**
     * @brief switches between register, login, quit
     * @param obj
     */
    void handle_startup(hash &obj);

    /**
     * @brief check credentials
     * @param cobj
     */
    void handle_login(hash &cobj);

    /**
     * @brief register user
     * @param cobj
     */
    void handle_register(hash &cobj);

    /**
     * @brief logout user
     */
    void handle_logout();

    /**
     * @brief Validate and sanitize the input string
     * @param input
     */
    void sanitize_input(std::string &input);

    /**
     * @brief
     * @param cobj
     */
    void handle_credentials(hash &cobj);

    /**
     * @brief
     * @param cobj
     */
    void handle_credential_entry(hash &cobj);

    /**
     * @brief
     * @param cobj
     */
    void handle_get_credential(hash &cobj);

    CLInterface();
    ~CLInterface();
};

CLInterface::~CLInterface() {}

#endif // CLINTERFACE_H