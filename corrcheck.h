#ifndef CORRCHECK_H
#define CORRCHECK_H

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <openssl/ssl.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "File.h"

#define SUCCESS 0
#define FAILURE 1

// use a buffer size of 10MB when working with hashes
#define BUFFER_SIZE 10485760

int create_database(const std::string& directory);
int verify_database();
int update_database();

int write_database(const std::string& directory, const std::vector<File>& file_list);

#endif

