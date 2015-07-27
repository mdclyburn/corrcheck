#include <cassert>
#include <iostream>
#include <dirent.h>
#include <openssl/sha.h>
#include <string>
using namespace std;

#include "corrcheck.h"

int main(int argc, char** args)
{
    // first argument will always be the action
    assert(argc >= 2);

    int result = 0;
    if(*args[1] == 'c') // create a new checksum database
    {
	// if a directory is not specified, then use the current directory
	if(argc == 2) result = create_database(".");
	else result = create_database(string(args[2]));
    }
    else if(*args[1] == 'v')
    {
    }
    else if(*args[1] == 'u')
    {
    }
    else
    {
	cout << "Unrecognized action: \'" << (char)(*args[1]) << "\'." << endl;
    }
 
    return result;
}
