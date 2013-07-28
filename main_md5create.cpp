#include "file.h"
#include "usage.h"

int main(int argc, char *argv[])
{
	if (argc < 3) usage();

	std::string db_file = argv[1];

	std::vector<std::string> paths;
	for (int i = 2; i < argc; ++i) {
		paths.push_back(argv[i]);
	}

	std::map<std::string, std::string> md5map;

	process(paths, md5map);

	print_md5map(md5map);
	save_md5map(db_file, md5map);
	
	return 0;
}
