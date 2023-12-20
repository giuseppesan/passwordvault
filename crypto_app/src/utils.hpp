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

string to_hex(unsigned char *str, int len)
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
 * @brief writes sting to given filename
 *
 * @param in String which is written
 * @param file Filename / Filepath
 *
 * @return 0 if successful
 */

int write_to_file(string in, string file)
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
 * @brief Reads from file and find a string
 *
 * @param out String witch filecontent
 * @param name given name
 * @param path filepath
 *
 * @return 0 if successful
 */

int read_from_file_and_find(string &out, string name, string path)
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

/**
 * @brief 
 * @param input src string
 * @return input 
 */
int char2int(char input)
{
  if(input >= '0' && input <= '9')
    return input - '0';
  if(input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if(input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument("Invalid input string");
}


/**
 * @brief This function assumes src to be a zero terminated sanitized string with
 *  an even number of [0-9a-f] characters, and target to be sufficiently large
 * @param src zero terminated sanitized string
 * @param target char array bytes
 */
void hex2bin(const char* src, char* target)
{
  while(*src && src[1])
  {
    *(target++) = char2int(*src)*16 + char2int(src[1]);
    src += 2;
  }
}

#endif // UTILS_H