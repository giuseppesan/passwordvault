
#include "main.hpp"

int main()
{
    Router router;
    // close application by typing "q" or "quit"
    while (true)
    {
        router.handle_input();
    }
    return 0;
}