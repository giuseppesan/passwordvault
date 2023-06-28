
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class cipher_object
{
public:
    bool decrypt(const unsigned char *ciphertext, int ciphertext_len, const unsigned char *key, const unsigned char *iv, unsigned char *plaintext, int &plaintext_len);
    bool encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key, const unsigned char *iv, unsigned char *ciphertext, int &ciphertext_len);
    int prepare_ciphertext(uint8_t version, uint8_t *iv, uint8_t *plain_cipher_text, uint64_t pl_ciph_text_size, 
									 uint8_t *cipher_text_out, uint64_t ciph_text_out_size);
    void sha_256();
    int write_to_file (string in);
    int read_from_file(string &out);
    cipher_object();
    ~cipher_object();
};
cipher_object::cipher_object() {}

cipher_object::~cipher_object() {}

int aes_cbc(void);
string toHex(unsigned char *str, int len);
unsigned char iv[16] = {0};
unsigned char key[32] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
                         0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
                         0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33,
                         0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31};