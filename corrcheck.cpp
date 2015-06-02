#include "corrcheck.h"

int create_database(const std::string& directory)
{
    std::vector<File> file_list = get_file_list(directory);
    if(file_list.size() == 0)
    {
	std::cout << "No files to checksum." << std::endl;
	return FAILURE;
    }

    // checksum all given files
    SHA256_CTX sha;
    unsigned char* const buffer = new unsigned char[BUFFER_SIZE];
    for(unsigned int i = 0; i < file_list.size(); i++)
    {
	SHA256_Init(&sha);
	FILE* file = fopen(std::string(directory + "/" + file_list[i].name).c_str(), "r");
	while(!feof(file))
	{
	    const unsigned int bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, file);
	    SHA256_Update(&sha, buffer, bytes_read);
	}
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_Final(hash, &sha);
	std::cout << file_list[i].name << " -> ";
	for(unsigned int b = 0; b < SHA256_DIGEST_LENGTH; b++)
	{
	    std::cout << std::hex << (unsigned int)hash[b];
	    file_list[i].checksum[b] = hash[b];
	}
	std::cout << std::endl;
	fclose(file);
    }
    delete[] buffer;

    return write_database(directory, file_list);
}

std::vector<File> get_file_list(const std::string& directory)
{
    std::vector<File> file_list;
    DIR* dir = opendir(directory.c_str());
    if(dir == nullptr)
    {
	std::cout << "\'" << directory << "\' is not readable." << std::endl;
	return file_list;
    }

    struct stat st;
    struct dirent* entry = readdir(dir);
    while(entry != nullptr)
    {
	// only add the file to the list if it is not a directory
	lstat(entry->d_name, &st);
	if(!S_ISDIR(st.st_mode) && std::string(entry->d_name).compare(".corrcheckdb") != 0)
	    file_list.push_back(File(std::string(entry->d_name)));
	entry = readdir(dir);
    }
    closedir(dir);

    return file_list;
}

int write_database(const std::string& directory, const std::vector<File>& file_list)
{
    // write the file names and checksums out
    // format: <checksum> <null-terminated string of name>
    FILE* file = fopen(std::string(directory + "/.corrcheckdb").c_str(), "w");
    if(file == NULL) return FAILURE;

    for(unsigned int i = 0; i < file_list.size(); i++)
    {
	fwrite(file_list[i].checksum, sizeof(unsigned char), SHA256_DIGEST_LENGTH, file);
	fwrite(file_list[i].name.c_str(), sizeof(char), file_list[i].name.length() + 1, file);
    }
    fclose(file);

    return SUCCESS;
}
