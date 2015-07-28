#include "corrcheck.h"

int create_database(const Options& opts)
{
    Directory dir;
    dir.set_path(opts.directory);
    dir.checksum_files(!opts.recursive);

    Database db;
    const std::vector<File*>& files = dir.get_files();
    for(auto it = files.begin(); it != files.end(); it++)
	db.insert(*it);
    db.write(opts.directory);

    if(opts.recursive == true)
    {
	const std::vector<std::string>& directories = dir.get_directories();
	for(auto it = directories.begin(); it != directories.end(); it++)
	{
	    Options new_opts = opts;
	    new_opts.directory += '/' + *it;
	    std::cout << "Continuing in " << new_opts.directory << std::endl;
	    if(create_database(new_opts) != CORRCHECK_SUCCESS)
		std::cout << "Failed in " << new_opts.directory << std::endl;
	}
    }

    return CORRCHECK_SUCCESS;
}

int verify_database(const Options& opts)
{
    bool needs_updating = false;

    Directory dir;
    dir.set_path(opts.directory);

    Database db;
    db.load(opts.directory);

    // find new files
    std::set<std::string> found_new_files;
    const std::vector<File*>& current_files = dir.get_files();
    for(auto it = current_files.begin(); it != current_files.end(); it++)
    {
	if(db.get((*it)->name) == nullptr)
	    found_new_files.insert((*it)->name);
    }

    if(found_new_files.size() > 0)
    {
	std::cout << "Found " << found_new_files.size() << " new file(s):" << std::endl;
	for(auto it = found_new_files.begin(); it != found_new_files.end(); it++)
	    std::cout << *it << std::endl;
	std::cout << "The database should be updated." << std::endl;
    }
    else
	std::cout << "No new files found." << std::endl;

    // verify checksums
    // check against files in directory; need to be checksummed first
    dir.checksum_files(!opts.recursive);
    std::set<std::string> found_changed_files;
    for(auto it = current_files.begin(); it != current_files.end(); it++)
    {
	// if it's a new file, no point in going further
	if(found_new_files.find((*it)->name) != found_new_files.end())
	    continue;

	const unsigned char* const current_sum = db.get((*it)->name);
	assert(current_sum != nullptr);
	unsigned int i = 0;
	for(; i < SHA256_DIGEST_LENGTH; i++)
	{
	    if(current_sum[i] != (*it)->checksum[i])
		break;
	}
	if(i < SHA256_DIGEST_LENGTH) // changed checksum
	    found_changed_files.insert((*it)->name);
    }

    if(found_changed_files.size() > 0)
    {
	std::cout << "Found " << found_changed_files.size() << " changed file(s):" << std::endl;
	for(auto it = found_changed_files.begin(); it != found_changed_files.end(); it++)
	    std::cout << *it << std::endl;
	std::cout << "The database needs to be updated or the file(s) have become corrupt." << std::endl;
    }
    else
	std::cout << "All files are consistent." << std::endl;

    // recurse if necessary
    if(opts.recursive == true)
    {
	const std::vector<std::string>& directories = dir.get_directories();
	for(auto it = directories.begin(); it != directories.end(); it++)
	{
	    Options new_opts = opts;
	    new_opts.directory += '/' + *it;
	    if(verify_database(new_opts) != CORRCHECK_SUCCESS)
		std::cout << "Failed in " << new_opts.directory << std::endl;
	}
    }

    return CORRCHECK_SUCCESS;
}

