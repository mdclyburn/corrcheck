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
    while(file.peek() != EOF)
    {
	unsigned char* checksum = new unsigned char[SHA256_DIGEST_LENGTH];
	file.read(reinterpret_cast<char*>(checksum), SHA256_DIGEST_LENGTH);

	char ch;
	file >> ch;
	while(ch != '\0')
	{
	    file_name += ch;
	    file >> ch;
	}

	file_checksums[file_name] = checksum;

	file_name = "";
    }

    return DATABASE_SUCCESS;
}

unsigned int Database::write(const std::string& dir)
{
    std::ofstream file;
    file.open(dir + "/.corrcheckdb", std::ios::binary);
    if(!file.is_open()) return DATABASE_FAILURE;

    // write each entry <checksum> <null-terminated string>
    for(auto it = file_checksums.begin(); it != file_checksums.end(); it++)
    {
	const std::string& name = it->first;
	unsigned char* const checksum = it->second;

	file.write(reinterpret_cast<char*>(checksum), SHA256_DIGEST_LENGTH);
	for(unsigned int i = 0; i < name.length(); i++)
	    file << name[i];
	file << '\0';
    }

    file.close();

    return DATABASE_SUCCESS;
}

void Database::insert(const File* file)
{
    assert(file_checksums.find(file->name) == file_checksums.end());
    file_checksums[file->name] = new unsigned char[SHA256_DIGEST_LENGTH];
    unsigned char** const checksum = &file_checksums[file->name];
    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	(*checksum)[i] = file->checksum[i];

    return;
}

void Database::remove(const std::string& file_name)
{
    assert(file_checksums.find(file_name) == file_checksums.end());
    delete[] file_checksums[file_name];

    return;
}

const unsigned char* Database::get(const std::string& file_name)
{
    return file_checksums[file_name];
}
