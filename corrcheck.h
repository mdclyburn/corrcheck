#ifndef CORRCHECK_H
#define CORRCHECK_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <map>
#include <openssl/ssl.h>
#include <string>
#include <sys/stat.h>
#include <vector>

#include "corrcheck_defines.h"
#include "File.h"

int create_database(const std::string& directory);
int update_database();
int verify_database(const std::string& directory);
int write_database(const std::string& directory, const File* file_list);

#endif

