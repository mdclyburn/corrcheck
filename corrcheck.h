#ifndef CORRCHECK_H
#define CORRCHECK_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Directory.h"
#include "Database.h"
#include "Options.h"

#define CORRCHECK_SUCCESS 0;
#define CORRCHECK_FAILURE 1;

/*
  Create a .corrcheckdb of the files in a directory given by 'directory'.
  Returns SUCCESS (0) if no errors occur; returns FAILURE (1) otherwise.
 */
int create_database(const Options& opts);

/*
  ---NOT IMPLEMENTED---
  Rewrite the .corrcheckdb of the files in a directory while notifying of
apparent changes since creation or last update.
 */
int update_database(const Options& opts);

/*
  ---NOT IMPLEMENTED---
  Check the .corrcheckdb of the files in a directory given by 'directory' and
notify of apparent changes since creation or last update. This function does not
modify the .corrcheckdb file.
 */
int verify_database(const Options& opts);

#endif

