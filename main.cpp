#include <cassert>
#include <iostream>
#include <dirent.h>
#include <openssl/sha.h>
#include <string>
using namespace std;

#include "corrcheck.h"
#include "Options.h"

int main(int argc, char** args)
{
    // first argument will always be the action
    assert(argc >= 2);

    // set options specified
    Options opts;
    read_options(opts, args);
    
    int result = 0;
    if(*args[1] == 'c') // create a new checksum database
	create_database(opts);

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
