#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <signal.h>

#include "log.hpp"
#include "file.h"

void usage_md5watch()
{
	std::cerr 
		<< "md5watch is realtime new/modify/delete watching tool." << std::endl
		<< std::endl
		<< "usage:" << std::endl
		<< "    $ md5watch [-i interval] [-n nice_val] [-p persistence_db_file] [file1] [file2] ... [dir1] [dir2] ..." << std::endl
		<< "    $ md5watch [-h]" << std::endl
		<< std::endl
		<< "example:" << std::endl
		<< "      $ md5watch /var /tmp /usr/local" << std::endl
		<< "      2013-07-30T19:50:04+09:00 [D] start watching..." << std::endl
		<< "      2013-07-30T19:50:26+09:00 [D] watching..." << std::endl
		<< "      2013-07-30T19:50:57+09:00 [I] N,/var/.a" << std::endl
		<< "      2013-07-30T19:51:12+09:00 [I] M,/var/tmp/text.txt" << std::endl
		<< "      2013-07-30T19:53:35+09:00 [I] D,/tmp/qwerty" << std::endl
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


std::string persistence_db_file = "";
std::map<std::string, std::string> old_md5map;

int main(int argc, char *argv[])
{
	int interval = 10;
	int nice_val = 19;

	if (argc < 2) usage_md5watch();

	while(true) {
		int c = getopt(argc, argv, "hp:i:n:");
		if (c == -1) break;

		switch(c) {
			case 'i':
				interval = atoi(optarg);
				if (interval < 0) interval = 10;
				break;
			case 'n':
				nice_val = atoi(optarg);
				if (nice_val < 0 || 20 < nice_val) nice_val = 19;
				break;
			case 'p':
				persistence_db_file = std::string(optarg);
				break;
			case 'h':
				usage_md5watch();
				break;
			default:
				usage_md5watch();
		}
	}
	if (optind + 1 > argc) {
		usage_md5watch();
	}

	// target path
	std::vector<std::string> paths;
	for (int i = optind; i < argc; ++i) {
		paths.push_back(argv[i]);
	}
	if (exists(paths) == false) {
		return 1;
	}

	log_d("start watching...");
	nice(nice_val);

	// restore old_md5map
	bool rv = false;
	if (persistence_db_file != "") {
		rv = load_md5map(persistence_db_file, old_md5map);
	}
	if (rv == false) {
		// calc first md5hash
		process(paths, old_md5map);
	}

	// watch loop...
	std::map<std::string, std::string> new_md5map;
	while(true) {
		log_d("watching...");

		new_md5map.clear();
		process(paths, new_md5map);

		compare(old_md5map, new_md5map);

		old_md5map.clear();
		old_md5map = new_md5map;

		// save persistence_db_file
		if (persistence_db_file != "") {
			signal(SIGINT, SIG_IGN);
			save_md5map(persistence_db_file, old_md5map);
			signal(SIGINT, SIG_DFL);
		}

		sleep(interval);
	}

	return 0;
}
