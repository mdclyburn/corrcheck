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
	lstat(entry->d_name, &st);
	if(S_ISREG(st.st_mode))
	    files.push_back(new File(path + "/" + std::string(entry->d_name)));
	else if(S_ISDIR(st.st_mode))
	    directories.push_back(path + "/" + std::string(entry->d_name));
	entry = readdir(dir);
    }
    closedir(dir);

    return;
}
