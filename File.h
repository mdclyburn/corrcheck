#ifndef FILE_H
#define FILE_H

#include <dirent.h>
#include <iostream>
#include <openssl/ssl.h>
#include <string>
#include <sys/stat.h>

#include "corrcheck_defines.h"

struct File
{
    File();
    File(const std::string& name);
    ~File();
 
    std::string name;
    unsigned char checksum[SHA256_DIGEST_LENGTH];
    File* next;
};

File* get_file_list(const std::string& directory);
void delete_file_list(File* file_list);
void checksum_files(const std::string& directory, File* const file_list, bool show_files = false);

#endif

