#include "corrcheck.h"

int create_database(const Options& opts)
{
    Directory dir;
    dir.set_path(opts.directory);
    dir.checksum_files();

    Database db;
    const std::vector<File*>& files = dir.get_files();
    for(auto it = files.begin(); it != files.end(); it++)
	db.insert(*it);
    db.write(opts.directory);

    return CORRCHECK_SUCCESS;
}

int verify_database(const Options& opts)
{
    return DATABASE_FAILURE;
}

