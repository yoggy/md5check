#include "file.h"
#include "usage.h"
#include "log.hpp"

int main(int argc, char *argv[])
{
	if (argc < 3) usage();

	std::string db_file = argv[1];
	std::map<std::string, std::string> old_md5map, new_md5map;

	if (load_md5map(db_file, old_md5map) == false) {
		log_e("load_md5map() failed...filename=%s", db_file.c_str());
		return 1;
	}

	// target path
	std::vector<std::string> paths;
	for (int i = 2; i < argc; ++i) {
		paths.push_back(argv[i]);
	}

	// calc md5hash
	process(paths, new_md5map);

	// compare
	compare(old_md5map, new_md5map);
	
	return 0;
}
