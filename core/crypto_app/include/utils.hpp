#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <vector>

#define PEPPER "SEpl9QTQ574d9R5R"

const std::string passwd_path = "../data/passwd";
const std::string credentials_path = "../data/logins";
const int found = 0; 
const int not_found = -2; 

namespace utils
{

/**
 * @brief convert bytes to hexstring
 *
 * @param str Bytes string
 * @param len length of string
 *
 * @return hex string
 */

std::string to_hex_sha(const unsigned char* str, int len)
{
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');

    for (int i = 0; i < len; ++i)
    {
        oss << std::setw(2) << static_cast<unsigned>(str[i]);
    }

    return oss.str();
}

/**
 * @brief convert bytes to hexstring
 * @param data vector Bytes
 * @return hex string
 */
std::string to_hex(const std::vector<uint8_t>& data)
{
    std::ostringstream oss;
    oss << std::hex << std::uppercase << std::setfill('0');

    for (const auto& byte : data)
    {
        oss << std::setw(2) << static_cast<unsigned>(byte);
    }

    return oss.str();
}
/**
 * @brief writes sting to given filename
 *
 * @param in String which is written
 * @param file Filename / Filepath
 *
 * @return 0 if successful
 */

int write_to_file(std::string in, std::string file)
{
    /*Add multiple entries*/

    std::ofstream my_file(file, std::ios::app);

    if (!my_file.is_open())
    {
        std::cerr << "Unable to open file: " << file << std::endl;
        return -1;
    }

    my_file << in << std::endl;

    return 0;
}


/**
 * @brief looks in defined file for a matching string
 * @param entry search string
 * @param path filepath
 * @return found 0 if successful, not found -2 if not successful
 */
int find_entry(const std::string &entry, std::string path)
{
    std::fstream my_file(path);
    std::string line = "";

    if (!my_file.is_open())
    {
        std::cerr << "Unable to open & read file\n";
        return -1;
    }

    while (std::getline(my_file, line))
    {
        if (line.substr(0, entry.size()) == entry)
        {
            return found;
        }
    }

    return not_found;
}

/**
 * @brief Reads from file and find a string
 *
 * @param out String witch file content
 * @param name given name
 * @param path filepath
 *
 * @return found 0 if successful, not found -2 if not successful
 */

int read_from_file_and_find(std::string &out, std::string name, std::string path)
{
   /*search for user in file*/
    std::ifstream my_file(path);

    if (!my_file.is_open())
    {
        std::cerr << "Unable to open & read file\n";
        return -1;
    }

    std::string line;

    while (getline(my_file, line))
    {
        if (line.compare(0, name.size(), name) == 0)
        {
            out = line;
            my_file.close();
            return found;
        }
    }
    return not_found;
}

/**
 * @brief 
 * @param input src string
 * @return input 
 */
int char2int(char input) {
  if (input >= '0' && input <= '9') {
      return input - '0';
  } else if (input >= 'A' && input <= 'F') {
      return input - 'A' + 10;
  } else if (input >= 'a' && input <= 'f') {
      return input - 'a' + 10;
  }
    // Handle invalid characters if needed
    std::cerr << "Error: Invalid hexadecimal character." << std::endl;
    return -1; // Or throw an exception
}


/**
 * @brief This function assumes src to be a zero terminated sanitized string with
 *  an even number of [0-9a-f] characters, and target to be sufficiently large
 * @param src zero terminated sanitized string
 * @param target char array bytes
 */
void hex2bin(const char* src, std::vector<uint8_t>& target) {
    while (*src && src[1]) {
        uint8_t byte = char2int(*src) * 16 + char2int(src[1]);
        target.push_back(byte);
        src += 2;
    }
}
}
#endif // UTILS_H