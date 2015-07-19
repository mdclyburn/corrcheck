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
	// only add the file to the list if it is not a directory
	lstat(entry->d_name, &st);
	if(!S_ISDIR(st.st_mode))
	    files.push_back(new File(path + "/" + std::string(entry->d_name)));
	entry = readdir(dir);
    }
    closedir(dir);

    return;
}
