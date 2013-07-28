#include <iostream>

void usage()
{
	std::cerr 
		<< "usage:" << std::endl
		<< "    $ md5create db_file [file1] [file2] ... [dir1] [dir2] ..." << std::endl
		<< "    $ md5check  db_file [file1] [file2] ... [dir1] [dir2] ..." << std::endl
		<< std::endl
		<< "example:" << std::endl
		<< "    create md5 hash database." << std::endl
		<< "      $ md5create db_file.db /var /tmp /usr/local" << std::endl
		<< std::endl
		<< "    check dm5 hash database." << std::endl
		<< "      $ md5check db_file.db /var /tmp /usr/local" << std::endl
		<< "      N,/var/.a" << std::endl
		<< "      M./var/tmp/text.txt" << std::endl
		<< "      D,/tmp/qwerty" << std::endl
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
