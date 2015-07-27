#ifndef OPTIONS_H
#define OPTIONS_H

#include <cstdlib>
#include <string>

struct Options
{
    Options();

    bool recursive;
    std::string directory;
};

void read_options(Options& opts, char** args);

#endif
