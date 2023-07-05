#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string.h>
#include <fstream>
#include <istream>
using namespace std;
#define PEPPER "SEpl9QTQ574d9R5R"

/**
 * @brief convert bytes to hexstring
 *
 * @param str Bytes string
 * @param len length of string
 *
 * @return hex string
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
 * @return 0 if successful
 */

inline int write_to_file(string in, string file)
{
    /*Add multiple entries*/

    ofstream my_file;
    my_file.open(file, ios::app);

    if (!my_file)
    {
        cout << "Unable to open file\n";
        return -1;
    }
    else
    {
        my_file << in;
        my_file << endl;
        my_file.close();
    }

    return 0;
}

/**
 * @brief Reads from file
 *
 * @param out String witch filecontent
 *
 * @return 0 if successful
 */

inline int read_from_file(string &out, string name, string path)
{
    /*search for user in file*/
    ifstream my_file(path.c_str());
    string check = "";

    if (my_file.is_open())
    {
        while (getline(my_file, out))
        {
            check = out.substr(0, name.size());

            if (strcmp(check.c_str(), name.c_str()) == 0)
            {
                my_file.close();
                return 0;
            }
        }
        cout << "Entry not found\n";
        return -1;
    }
    else
    {
        cout << "Unable to open & read file\n";
        return -1;
    }
}

#endif // UTILS_H