#include "corrcheck.h"

int create_database(const std::string& directory)
{
    // get a listing of the files in the directory
    std::vector<File> file_list;
    DIR* dir = opendir(directory.c_str());
    if(dir == nullptr)
    {
	std::cout << "\'" << directory << "\'" << " is not readable." << std::endl;
	return FAILURE;
    }

    struct stat st;
    struct dirent* entry = readdir(dir);
    while(entry != nullptr)
    {
	// only add the file to the list if it is not a directory
	lstat(entry->d_name, &st);
	if(!S_ISDIR(st.st_mode))
	    file_list.push_back(File(std::string(entry->d_name)));
	entry = readdir(dir);
    }
    closedir(dir);

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
	for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
	    std::cout << std::hex << (unsigned int)hash[i];
	std::cout << std::endl;
	fclose(file);
    }
    delete[] buffer;

    return SUCCESS;
}
