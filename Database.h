#ifndef DATABASE_H
#define DATABASE_H

#include <cassert>
#include <fstream>
#include <map>
#include <string>

#include "Directory.h"

#define DATABASE_SUCCESS 0
#define DATABASE_FAILURE 1

class Database
{
public:
    Database();
    ~Database();

    unsigned int load(const std::string& path);
    unsigned int write(const std::string& dir);

    void insert(const File* file);
    void remove(const std::string& file_name);
    const unsigned char* get(const std::string& file_name);

private:
    std::map<std::string, unsigned char*> file_checksums;
};

#endif
