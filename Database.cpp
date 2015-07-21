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
    std::ifstream file;
    file.open(path + "/.corrcheckdb", std::ios::binary);
    if(!file.is_open()) return DATABASE_FAILURE;

    // read each entry <checksum> <null-terminated string>
    std::string file_name = "";
    while(!file.eof())
    {
	unsigned char* checksum = new unsigned char[SHA256_DIGEST_LENGTH];
	for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	    file >> checksum[i];

	char ch;
	do
	{
	    file >> ch;
	    file_name += ch;
	}
	while(ch != '\0');

	file_checksums[file_name] = checksum;

	file_name = "";
    }

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
