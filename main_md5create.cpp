#include "file.h"
#include "usage.h"
#include "log.hpp"

int main(int argc, char *argv[])
{
	if (argc < 3) usage();

	std::string db_file = argv[1];

	// target path
	std::vector<std::string> paths;
	for (int i = 2; i < argc; ++i) {
		paths.push_back(argv[i]);
	}
	if (exists(paths) == false) {
		return 1;
	}

	// calc md5hash
	std::map<std::string, std::string> md5map;
	process(paths, md5map);

	// save 
	if (save_md5map(db_file, md5map) == false) {
		log_e("save_md5map() failed...filename=%s", db_file.c_str());
		return 1;
	}
	
	return 0;
}
