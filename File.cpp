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
