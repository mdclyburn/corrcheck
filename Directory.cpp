#include "Directory.h"

Directory::Directory() : path("")
{
}

Directory::Directory(const std::string& path) : path(path)
{
    read_files();
}

Directory::~Directory()
{
    for(auto it = files.begin(); it != files.end(); it++)
	delete (*it);
}

void Directory::set_path(const std::string& path)
{
    this->path = path;
    read_files();
    return;
}

const std::string& Directory::get_path() const
{
    return path;
}

const std::vector<File*>& Directory::get_files() const
{
    return files;
}

void Directory::checksum_files()
{
    if(files.size() == 0)
    {
	std::cout << "No files to checksum." << std::endl;
	return;
    }

    SHA256_CTX sha;
    unsigned int last_length = 0;
    unsigned int checksummed_files = 0;
    unsigned char* buffer = new unsigned char[FILE_BUFFER_SIZE];
    std::string output_msg = "";
    for(auto it = files.begin(); it != files.end(); it++)
    {
	// helpful output
	output_msg = " (" + std::to_string(checksummed_files++) + " of " + std::to_string(files.size()) + ")" + " checksumming \'" + (*it)->name + '\'';
	if(last_length > output_msg.length())
	    while(last_length--) std::cout << ' ';
	std::cout << '\r';
	last_length = output_msg.length();
	std::cout << output_msg << '\r' << std::flush;

	SHA256_Init(&sha);
	std::ifstream file;
	file.open(path + '/' + (*it)->name, std::ios::binary);
	assert(file.is_open());

	while(!file.eof())
	{
	    // get up to FILE_BUFFER_SIZE bytes of file data
	    unsigned int bytes_read = 0;
	    while(bytes_read < FILE_BUFFER_SIZE)
	    {
		file.read(reinterpret_cast<char*>(&buffer[bytes_read]), sizeof(unsigned char));
		if(file.eof()) break;
		else bytes_read++;
	    }

	    SHA256_Update(&sha, buffer, bytes_read);
	}

	SHA256_Final((*it)->checksum, &sha);
    }

    // clear line and print final message
    output_msg = "Finished checksumming " + std::to_string(files.size()) + " files.";
    std::cout << output_msg;
    for(unsigned int i = 0; (output_msg.length() < last_length) && i < (last_length - output_msg.length()); i++)
	std::cout << ' ';
    std::cout << std::endl;

    delete[] buffer;

    return;
}

const std::vector<std::string>& Directory::get_directories() const
{
    return directories;
}

void Directory::read_files()
{
    // debug
    assert(path.length() > 0); // - non-zero length path

    DIR* dir = opendir(path.c_str());
    assert(dir != nullptr);
    struct stat st;
    struct dirent* entry = readdir(dir);
    while(entry != nullptr)
    {
	lstat(std::string(path + '/' + entry->d_name).c_str(), &st);
	if(S_ISREG(st.st_mode) && std::strcmp(entry->d_name, ".corrcheckdb") != 0)
	{
	    File* file = new File;
	    file->name = entry->d_name;
	    files.push_back(file);
	}
	else if(S_ISDIR(st.st_mode) && std::strcmp(entry->d_name, ".") != 0 && std::strcmp(entry->d_name, "..") != 0)
	    directories.push_back(std::string(entry->d_name));
	entry = readdir(dir);
    }
    closedir(dir);

    return;
}
