#ifndef FILE_H
#define FILE_H

#include <openssl/ssl.h>
#include <string>

struct File
{
    File();
    File(const std::string& name);
    ~File();
 
    std::string name;
    unsigned char checksum[SHA256_DIGEST_LENGTH];
    File* next;
};

#endif

