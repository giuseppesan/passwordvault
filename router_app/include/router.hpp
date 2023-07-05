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
    string get_curr_path();
    void handle_input();
    void console_available_commands();
    void handle_user(crypto &obj);
    void handle_credentials(crypto &cobj);
    Router();
    ~Router();
};

Router::~Router() {}

#endif // ROUTER_H