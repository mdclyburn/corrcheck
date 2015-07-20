#include "Database.h"

Database::Database()
{
}

Database::~Database()
{
    for(auto it = file_checksums.begin(); it != file_checksums.end(); it++)
	delete[] it->second;
}

unsigned int Database::load(const std::string& path)
{
    return DATABASE_SUCCESS;
}

unsigned int Database::write(const std::string& dir)
{
    return DATABASE_SUCCESS;
}

void Database::insert(const File* file)
{
    return;
}

void Database::remove(const std::string& file_name)
{
    return;
}

const unsigned char* get(const std::string& file_name)
{
    return nullptr;
}
