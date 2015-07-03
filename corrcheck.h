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

/*
  Create a .corrcheckdb of the files in a directory given by 'directory'.
  Returns SUCCESS (0) if no errors occur; returns FAILURE (1) otherwise.
 */
int create_database(const std::string& directory);

/*
  ---NOT IMPLEMENTED---
  Rewrite the .corrcheckdb of the files in a directory while notifying of
apparent changes since creation or last update.
 */
int update_database();

/*
  Check the .corrcheckdb of the files in a directory given by 'directory' and
notify of apparent changes since creation or last update. This function does not
modify the .corrcheckdb file.
 */
int verify_database(const std::string& directory);

/*
  Write a .corrcheckdb in the directory given by 'directory' based on the list of
files given as 'file_list', overwriting any that may already be present.
 */
int write_database(const std::string& directory, const File* file_list);

#endif

