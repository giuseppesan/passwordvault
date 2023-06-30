#ifndef ROUTER_H
#define ROUTER_H

#include <iostream>
#include <string>

using namespace std;

class Router
{
    private:
        string curr_path;
        string input;
        string message;
    public:
        string get_curr_path();
        void handle_input();
        void console_available_commands();
        Router();
        ~Router();
};

Router::~Router() {}

#endif // ROUTER_H