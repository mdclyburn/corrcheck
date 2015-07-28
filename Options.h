#ifndef OPTIONS_H
#define OPTIONS_H

#include <cassert>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <string>

struct Options
{
    Options();

    bool recursive;
    std::string directory;
};

void read_options(Options& opts, int argc, char** args);

#endif
