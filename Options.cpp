#include "Options.h"

Options::Options() : recursive(true), directory(".")
{
}

void read_options(Options& options, int argc, char** args)
{
    // an unspecified mode should not be handled here
    assert(argc >= 2);

    // parse switches
    unsigned int i = 2;
    for(; i < argc; i++)
    {
	// recursive run (not implemented)
	if(strcmp(args[i], "-r") == 0)
	    options.recursive = true;
	else break;
    }

    // user may have specified a directory
    if(i < argc)
    {
	// validate
	DIR* dir = opendir(args[argc - 1]);
	if(dir == nullptr)
	{
	    std::cout << "\'" << args[argc - 1] << "\' is not a valid directory." << std::endl;
	    exit(13);
	}
	options.directory = args[argc - 1];
    }
    
    return;
}

