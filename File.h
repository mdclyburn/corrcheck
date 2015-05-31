#ifndef FILE_H
#define FILE_H

#include <string>

struct File
{
	std::string name;
	std::string checksum;

	File() : name(""), checksum("") {}
	File(const std::string& name) : name(name), checksum("") {}
	~File() {}
};

#endif

