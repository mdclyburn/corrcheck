#ifndef FILE_H
#define FILE_H

#include <openssl/ssl.h>
#include <string>

// use buffer size of 10MB when handling files
#define FILE_BUFFER_SIZE 10485760

// error codes
#define FILE_SUCCESS 0
#define FILE_FAILURE 1
#define FILE_OPEN_FAILED 2

struct File
{
    std::string name;
    unsigned char checksum[SHA256_DIGEST_LENGTH];
};

#endif

