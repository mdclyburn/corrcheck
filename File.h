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
 
    /*
      'name'
      The name of the file. This does not include any kind of path
      to the file (neither relative nor full). Initialized to a blank
      string.

      'checksum'
      The file's checksum. Corrcheck uses the SHA-256 algorithm
      for hashes.

      'next'
      Enables linked-list structures. Initialized to 'NULL'.
     */
    std::string name;
    unsigned char checksum[SHA256_DIGEST_LENGTH];
    File* next;
};

void checksum_files(const std::string& directory, File* const file_list, bool show_output = false);
void delete_file_list(File* file_list);
File* get_file_list(const std::string& directory);

#endif

