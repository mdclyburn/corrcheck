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
    // debug
    assert(checksum == nullptr); // - please do not checksum twice
    assert(path.length() > 0); // - a path should be specified

    checksum = new unsigned char[SHA256_DIGEST_LENGTH];
    
    std::ifstream file;
    file.open(path, std::ios::binary);
    if(!file.is_open())
	return FILE_OPEN_FAILED;
    
    SHA256_CTX sha;
    SHA256_Init(&sha);
    
    unsigned char* buffer = new unsigned char[FILE_BUFFER_SIZE];
    while(!file.eof())
    {
	// get up to FILE_BUFFER_SIZE byts of file data
	unsigned int bytes_read = 0;
	while(bytes_read < FILE_BUFFER_SIZE)
	{
	    file.read(reinterpret_cast<char*>(&buffer[bytes_read]), sizeof(unsigned char));
	    if(file.eof()) break;
	    else bytes_read++;
	}
	std::cout << "read " << bytes_read << " bytes" << std::endl;

	SHA256_Update(&sha, buffer, bytes_read);
    }

    delete[] buffer;
    file.close();
    SHA256_Final(checksum, &sha);
    
    return FILE_SUCCESS;
}

const unsigned char* File::get_checksum() const
{
    return checksum;
}
