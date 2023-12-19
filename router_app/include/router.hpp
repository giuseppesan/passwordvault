#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Router
{
private:
    string curr_path;
    string input;
    string logged_in_user;

public:
    /**
     * @brief returns current path
     * @return current path string
     */
    string get_curr_path();

    /**
     * @brief main loop for CLI program - starts with auth
     */
    void handle_input();

    /**
     * @brief prints the options depending on the current path
     */
    void console_available_commands();

    /**
     * @brief 
     * @param obj 
     */
    void handle_user(crypto &obj);

    /**
     * @brief 
     * @param cobj 
     */
    void handle_credentials(crypto &cobj);
    Router();
    ~Router();
};

Router::~Router() {}

#endif // ROUTER_H