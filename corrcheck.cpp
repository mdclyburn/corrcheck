#include "corrcheck.h"

int create_database(const std::string& directory)
{
    File* file_list = get_file_list(directory);
    if(file_list == nullptr)
    {
	std::cout << "No files to checksum." << std::endl;
	return FAILURE;
    }

    // checksum all given files
    SHA256_CTX sha;
    unsigned char* const buffer = new unsigned char[BUFFER_SIZE];
    for(File* current_file = file_list; current_file != nullptr; current_file = current_file->next)
    {
	SHA256_Init(&sha);
	FILE* file = fopen(std::string(directory + "/" + current_file->name).c_str(), "r");
	while(!feof(file))
	{
	    const unsigned int bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, file);
	    SHA256_Update(&sha, buffer, bytes_read);
	}
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_Final(hash, &sha);
	std::cout << current_file->name << " -> ";
	for(unsigned int b = 0; b < SHA256_DIGEST_LENGTH; b++)
	{
	    std::cout << std::hex << (unsigned int)hash[b];
	    current_file->checksum[b] = hash[b];
	}
	std::cout << std::endl;
	fclose(file);
    }
    delete[] buffer;

    return write_database(directory, file_list);
}

File* get_file_list(const std::string& directory)
{
    File* file_list = nullptr;
    File* last = file_list;
    DIR* dir = opendir(directory.c_str());
    if(dir == nullptr)
    {
	std::cout << "\'" << directory << "\' is not readable." << std::endl;
	return nullptr;
    }

    struct stat st;
    struct dirent* entry = readdir(dir);
    while(entry != nullptr)
    {
	// only add the file to the list if it is not a directory
	lstat(entry->d_name, &st);
	if(!S_ISDIR(st.st_mode) && std::string(entry->d_name).compare(".corrcheckdb") != 0)
	{
	    // null list
	    if(file_list == nullptr)
	    {
		file_list = new File(std::string(entry->d_name));
		last = file_list;
	    }
	    else
	    {
		last->next = new File(std::string(entry->d_name));
		last = last->next;
	    }
	}
	entry = readdir(dir);
    }
    closedir(dir);

    return file_list;
}

int write_database(const std::string& directory, const File* file_list)
{
    // write the file names and checksums out
    // format: <checksum> <null-terminated string of name>
    FILE* file = fopen(std::string(directory + "/.corrcheckdb").c_str(), "w");
    if(file == NULL) return FAILURE;
 
    while(file_list)
    {
	fwrite(file_list->checksum, sizeof(unsigned char), SHA256_DIGEST_LENGTH, file);
	fwrite(file_list->name.c_str(), sizeof(char), file_list->name.length() + 1, file);
	file_list = file_list->next;
    }
    fclose(file);

    return SUCCESS;
}
