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
    checksum_files(directory, file_list);
    
    return write_database(directory, file_list);
}

int verify_database(const std::string& directory)
{
    // ensure that the .corrcheckdb file exists here
    FILE* corrcheckdb = fopen(std::string(directory + "/.corrcheckdb").c_str(), "r");
    if(corrcheckdb == nullptr)
    {
	std::cout << "Could not open or find the .corrcheckdb file." << std::endl;
	return FAILURE;
    }

    // structure: <SHA256 checksum> <file name (null-terminated)>
    std::cout << "Reading database..." << std::endl;
    std::map<std::string, unsigned char*> database_files;
    while(!feof(corrcheckdb))
    {
	std::string name;
	unsigned char* hash = new unsigned char[SHA256_DIGEST_LENGTH];
	assert(hash);

	// read hash
	const unsigned int bytes_read = fread(hash, sizeof(unsigned char), SHA256_DIGEST_LENGTH, corrcheckdb);
	if(bytes_read != SHA256_DIGEST_LENGTH && !feof(corrcheckdb))
	{
	    std::cout << "Could not read checksum from database." << std::endl;
	    return FAILURE;
	}
	else if(feof(corrcheckdb))
	    break;

	// read file name
	char read_char;
	do
	{
	    if(fread(&read_char, sizeof(char), 1, corrcheckdb) != 1)
	    {
		std::cout << "Failed while reading file name from database." << std::endl;
		return FAILURE;
	    }
	    if(read_char != '\0') name += read_char;
	}
	while(read_char != '\0');
	
	database_files[name] = hash;
    }

    File* file_list = get_file_list(directory);
    if(file_list == nullptr)
	std::cout << "No files to check." << std::endl;
    else
    {
	// run checks on files that are in the database and exist
	checksum_files(directory, file_list);
	unsigned int new_files = 0;
	unsigned int changed_files = 0;
	File* current = file_list;
	while(current)
	{
	    if(database_files.find(current->name) == database_files.end())
	    {
		std::cout << "NEW:     " << current->name << std::endl;
		new_files++;
	    }

	    unsigned int i = 0;
	    while(current->checksum[i] == database_files[current->name][i] && i < SHA256_DIGEST_LENGTH) i++;
	    if(i != SHA256_DIGEST_LENGTH)
	    {
		std::cout << "CHANGED: " << current->name << std::endl;
		changed_files++;
	    }

	    current = current->next;
	}
    }

    return SUCCESS;
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

void checksum_files(const std::string& directory, File* file_list)
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

	fclose(file);
    }

    delete[] buffer;
 
    return;
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
