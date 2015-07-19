#ifndef FILE_H
#define FILE_H

#include <cassert>
#include <fstream>
#include <iostream>
#include <openssl/ssl.h>
#include <string>

// use buffer size of 10MB when handling files
#define FILE_BUFFER_SIZE 10485760

// error codes
#define FILE_SUCCESS 0
#define FILE_FAILURE 1
#define FILE_OPEN_FAILED 2

class File
{
public:
    File();
    File(const std::string& path);
    ~File();

    void set_path(const std::string& path);
    const std::string& get_path() const;

    unsigned int calculate_checksum();
    const unsigned char* get_checksum() const;

private:

    // full path to the file
    std::string path;

    // file's checksum using SHA256
    unsigned char* checksum;
};

#endif

