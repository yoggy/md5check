#include "file.h"
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include "log.h"

void usage_md5watch()
{
	std::cerr 
		<< "md5watch is realtime new/modify/delete watching tool." << std::endl
		<< std::endl
		<< "usage:" << std::endl
		<< "    $ md5watch [file1] [file2] ... [dir1] [dir2] ..." << std::endl
		<< std::endl
		<< "example:" << std::endl
		<< "      $ md5watch /var /tmp /usr/local" << std::endl
		<< "      2013/07/30 19:50:04 [D] start watching..." << std::endl
		<< "      2013/07/30 19:50:26 [D] watching..." << std::endl
		<< "      2013/07/30 19:50:57 [I] N,/var/.a" << std::endl
		<< "      2013/07/30 19:51:12 [I] M,/var/tmp/text.txt" << std::endl
		<< "      2013/07/30 19:53:35 [I] D,/tmp/qwerty" << std::endl
		<< "         .         " << std::endl
		<< "         .         " << std::endl
		<< "         .         " << std::endl
		<< std::endl
		<< "      N : create new file" << std::endl
		<< "      M : modified file" << std::endl
		<< "      D : deleted file" << std::endl
		<< std::endl;

	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc < 2) usage_md5watch();

	nice(10);

	std::string db_file = argv[1];
	std::map<std::string, std::string> old_md5map, new_md5map;

	// target path
	std::vector<std::string> paths;
	for (int i = 1; i < argc; ++i) {
		paths.push_back(argv[i]);
	}
	if (exists(paths) == false) {
		return 1;
	}

	log_d("start watching...");

	// calc first md5hash
	process(paths, old_md5map);

	// watch loop...
	while(true) {
		log_d("watching...");

		new_md5map.clear();
		process(paths, new_md5map);
		compare(old_md5map, new_md5map);
		old_md5map.clear();
		old_md5map = new_md5map;

		sleep(10);
	}

	return 0;
}
