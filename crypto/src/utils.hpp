#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string.h>
using namespace std;

//string to_hex(unsigned char *str, int len);
inline string to_hex(unsigned char *str, int len)
{
    string hex_hash_string = "";
    char *buffer = new char[len * 2 + 1];
    char *p_buffer = buffer;
    for (int i = 0; i < len; ++i)
    {
        sprintf(p_buffer, "%02X", str[i]);
        p_buffer += 2;
    }

    hex_hash_string.assign(buffer, buffer + len);
    return hex_hash_string;
}

#endif //UTILS_H