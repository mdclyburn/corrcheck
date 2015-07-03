#include "File.h"

File::File() : name(""), next(nullptr)
{
}

File::File(const std::string& name) : name(name), next(nullptr)
{
}

File::~File()
{
}

void checksum_files(const std::string& directory, File* const file_list, bool show_output)
{
    SHA256_CTX sha;
    unsigned char* buffer = new unsigned char[BUFFER_SIZE];
    for(File* current_file = file_list; current_file != nullptr; current_file = current_file->next)
    {
	SHA256_Init(&sha);
	FILE* file = fopen(std::string(directory + "/" + current_file->name).c_str(), "r");

	// hash files in BUFFER_SIZE chunks
	while(!feof(file))
	{
	    const unsigned int bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, file);
	    SHA256_Update(&sha, buffer, bytes_read);
	}

	// get the final hash
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_Final(hash, &sha);
	for(unsigned int b = 0; b < SHA256_DIGEST_LENGTH; b++)
	{
	    current_file->checksum[b] = hash[b];
	}

	// show files with their checksums if enabled
	if(show_output)
	{
	    std::cout << current_file->name << " -> ";
	    for(unsigned int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		std::cout << std::hex << (unsigned int) hash[i];
	    std::cout << std::endl;
	}

	fclose(file);
    }

    delete[] buffer;
 
    return;
}

void delete_file_list(File* file_list)
{
    File* current = file_list;
    while(current != nullptr)
    {
	File* to_kill = current;
	current = current->next;
	delete to_kill;
    }

    return;
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

