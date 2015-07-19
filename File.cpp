#include "File.h"

File::File() : path(""), checksum(nullptr)
{
}

File::File(const std::string& path) : path(path)
{
}

File::~File()
{
    if(checksum) delete[] checksum;
}

void File::set_path(const std::string& path)
{
    this->path = path;
    return;
}

const std::string& File::get_path() const
{
    return path;
}

unsigned int File::calculate_checksum()
{
    return FILE_SUCCESS;
}

const unsigned char* File::get_checksum() const
{
    return checksum;
}
