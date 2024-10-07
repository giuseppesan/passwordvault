#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <iomanip>
#include <vector>

#define PEPPER "SEpl9QTQ574d9R5R"
const std::string passwd_path = std::string(PROJECT_ROOT) + "/data/passwd";
const std::string credentials_path = std::string(PROJECT_ROOT) + "/data/logins";
const std::string encryption_key_path = std::string(PROJECT_ROOT) + "/data/encryption_key.bin";
const std::string hmac_key_path = std::string(PROJECT_ROOT) + "/data/hmac_key.bin";
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

    inline std::string to_hex_sha(const unsigned char *str, int len)
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
    inline std::string to_hex(const std::vector<uint8_t> &data)
    {
        std::ostringstream oss;
        oss << std::hex << std::uppercase << std::setfill('0');

        for (const auto &byte : data)
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

    /**
     * @brief Sanitizes the given input string by removing non-printable characters and trimming leading/trailing whitespaces.
     *
     * @param input String which is sanitized
     */
    inline void sanitize_input(std::string &input)
    {
        for (char &ch : input)
        {
            if (!isprint(static_cast<unsigned char>(ch)))
            {
                // Replace non-printable characters with a space
                ch = ' ';
            }
        }

        // Trim leading and trailing whitespaces
        input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
        input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
    }

    /**
     * @brief writes given string to a file
     *
     * @param in String which is written
     * @param file Filename / Filepath
     *
     * @return 0 if successful
     */
    inline int write_to_file(std::string in, std::string file)
    {
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
     * @brief reads the binary key from the file
     * @param file_path file path
     * @param key the private key
     * @return true or false
     */
    inline bool readKeyFromFile(std::string file_path, unsigned char *key)
    {
        std::ifstream key_file(file_path, std::ios::binary);

        if (!key_file.is_open())
        {
            std::cerr << "Error opening key file for reading" << std::endl;
            return false;
        }

        key_file.read(reinterpret_cast<char *>(key), 32);

        if (key_file.fail())
        {
            std::cerr << "Error reading key from file" << std::endl;
            key_file.close();
            return false;
        }

        key_file.close();
        return true;
    }

    /**
     * @brief looks in defined file for a matching string
     * @param entry search string
     * @param path filepath
     * @return found 0 if successful, not found -2 if not successful
     */
    inline int find_entry(const std::string &entry, const std::string file_path)
    {
        std::fstream my_file(file_path);
        std::string line;

        if (!my_file.is_open())
        {
            std::cerr << "Unable to open & read file\n";
            return -1;
        }
        size_t firstColonPos;
        while (std::getline(my_file, line))
        {
            firstColonPos = line.find(':');
            if (line.compare(0, firstColonPos, entry) == 0)
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

    inline int read_from_file_and_find(std::string &out, std::string name, const std::string file_path)
    {
        std::ifstream my_file(file_path);

        if (!my_file.is_open())
        {
            std::cerr << "Unable to open & read file\n";
            return -1;
        }

        std::string line;
        size_t firstColonPos;
        while (getline(my_file, line))
        {
            firstColonPos = line.find(':');
            if (line.compare(0, firstColonPos, name) == 0)
            {
                out = line;
                my_file.close();
                return found;
            }
        }
        return not_found;
    }

    inline int return_user(std::string &out, const std::string file_path)
    {
        std::ifstream my_file(file_path);
        std::string line;
        if (!my_file.is_open())
        {
            std::cerr << "Unable to open & read file\n";
            return -1;
        }
        getline(my_file, line);
        out = line;
        return 0;
    }

    inline bool is_file_empty(const std::string &file_path)
    {
        std::ifstream file(file_path);

        if (!file.is_open())
        {
            std::cerr << "Error opening file: " << file_path << std::endl;
            return false;
        }

        return file.peek() == std::ifstream::traits_type::eof();
    }

    /**
     * @brief Converts a single character to an integer. Supports 0-9, a-f, A-F.
     *
     * @param input The character to convert.
     * @return The converted integer or -1 if the character is invalid.
     */
    inline int char2int(char input)
    {
        if (input >= '0' && input <= '9')
        {
            return input - '0';
        }
        else if (input >= 'A' && input <= 'F')
        {
            return input - 'A' + 10;
        }
        else if (input >= 'a' && input <= 'f')
        {
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
    inline void hex2bin(const char *src, std::vector<uint8_t> &target)
    {
        while (*src && src[1])
        {
            uint8_t byte = char2int(*src) * 16 + char2int(src[1]);
            target.push_back(byte);
            src += 2;
        }
    }
}
#endif // UTILS_HPP