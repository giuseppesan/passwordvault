#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string.h>
#include <fstream>
using namespace std;
#define PEPPER "SEpl9QTQ574d9R5R"
/**
 * @brief convert bytes to hexstring
 *
 * @param str Bytes string 
 * @param len length of string 
 *
 * @return string
 */

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

/**
 * @brief writes sting to give filename
 *
 * @param in String which is written
 * @param file Filename / Filepath
 *
 * @return 0 is successful
 */

inline int write_to_file(string in, string file)
{
    // TODO handle multiple entries
    ofstream my_file(file);

    if (my_file.is_open())
    {
        my_file << in;
        my_file.close();
    }
    else
    {
        cout << "Unable to open file\n";
        return -1;
    }

    return 0;
}

/**
 * @brief Reads from file
 *
 * @param out String witch filecontent
 *
 * @return 0 is successful
 */

inline int read_from_file(string &out)
{
    // TODO search for user in file
    ifstream my_file("sha");

    if (my_file.is_open())
    {
        getline(my_file, out);
        my_file.close();
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }

    return 0;
}

#endif // UTILS_H