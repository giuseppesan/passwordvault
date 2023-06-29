#include "crypto/src/crypto.cpp"
#include "router/src/router.cpp"

int main()
{
    /*run crypto algorithm*/
    // aes_cbc();
    /*run hash algorithm*/
    // sha_256();

    
    Router router;

    // close application by typing "q" or "quit"
    while (true)
    {
        router.handle_input();
    }
    return 0;
}