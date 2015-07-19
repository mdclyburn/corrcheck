#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <cassert>
#include <dirent.h>
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <vector>

#include "File.h"

class Directory
{
public:
    Directory();
    Directory(const std::string& path);
    ~Directory();

    void set_path(const std::string& path);
    const std::string& get_path() const;

    const std::vector<File*>& get_files() const;

private:
    std::string path;
    std::vector<File*> files;

    void read_files();
};

#endif
