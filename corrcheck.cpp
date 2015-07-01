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
    checksum_files(directory, file_list, true);
    const int result = write_database(directory, file_list);
    delete_file_list(file_list);
    
    return result;
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
    unsigned int new_files = 0, changed_files = 0;
    if(file_list == nullptr)
	std::cout << "No files to check." << std::endl;
    else
    {
	// run checks on files that are in the database and exist
	checksum_files(directory, file_list);
	File* current = file_list;
	while(current)
	{
	    if(database_files.find(current->name) == database_files.end())
	    {
		std::cout << "NEW:     " << current->name << std::endl;
		new_files++;
		current = current->next;
		continue;
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

    delete_file_list(file_list);

    if(changed_files > 0)
	std::cout << "File(s) were found that did not match the database checksum(s).\n";

    if(new_files > 0)
    {
	std::cout << "New file(s) were found that were not in the database.\n"
		  << "Consider updating/recreating the database." << std::endl;
    }

    if(changed_files + new_files == 0)
	std::cout << "All files are checksummed and consistent." << std::endl;

    return SUCCESS;
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
