#include "file.h"
#include "usage.h"

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
	save_md5map(db_file, md5map);
	
	return 0;
}
