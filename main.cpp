
#include "main.hpp"

int main()
{
    int ret = -1;
    bool registration = false;
    string name = "Alice";
    string pw = "Secret";
    
    Router router;

    // close application by typing "q" or "quit"
    while (true)
    {
        router.handle_input();
    }
    return 0;
}