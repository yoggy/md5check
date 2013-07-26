#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 

extern "C" {
#include "md5.h"
};

#include <string>
#include <map>
#include <queue>
#include <iostream>
#include <sstream>
#include <iomanip>

std::map<std::string, std::string> md5map;
std::deque<std::string> queue;

void usage()
{
	std::cerr << "usage : hashcheck [file1] [file2] ... [dir1] [dir2]..." << std::endl;
	std::cerr << std::endl;
	exit(1);
}

bool is_file(const char *path)
{
	struct stat st;
	if (stat(path, &st) != 0) return false;

	if (S_ISREG(st.st_mode)) return true; // normal file
	
	return false;
}

bool is_directory(const char *path)
{
	struct stat st;
	if (stat(path, &st) != 0) return false;

	if (S_ISDIR(st.st_mode)) return true; // directory
	
	return false;
}

bool check_queue()
{
	std::deque<std::string>::iterator i;
	for (i = queue.begin(); i != queue.end(); ++i) {
		if (is_file(i->c_str()) == false && is_directory(i->c_str()) == false) {
			std::cerr << "path not found...path=" << i->c_str() << std::endl;
			return false;
		}
	}
	return true;
}

void print_queue()
{
	std::deque<std::string>::iterator i;
	for (i = queue.begin(); i != queue.end(); ++i) {
		std::cerr << *i << std::endl;
	}
}

std::string md5str(const unsigned char *md5)
{
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < 16; ++i) {
	    ss << std::setw(2) << static_cast<unsigned>(md5[i]);
	}
	return ss.str();
}

void process_file(const char *path)
{
	MD5_CTX ctx;
	unsigned char md5[16];
	unsigned char buf[1024];

	MD5_Init(&ctx);

	FILE *fp = fopen(path, "rb");
	if (fp == 0) {
		std::cout << path << "," << "cannot_open" << std::endl;
		return;
	}

	while(true) {
		size_t size = fread(buf, 1, 1024, fp);
		if (size <= 0) break;
		MD5_Update(&ctx, buf, size);
	}

	fclose(fp);
	fp = NULL;

	MD5_Final(md5, &ctx);
	
	md5map[path] = md5str(md5);
}

void process_directory(const char *path)
{
	
}

void process(const char *path)
{
	if (is_file(path)) process_file(path);
	if (is_directory(path)) process_directory(path);
}

void process_queue()
{
	while(queue.size() > 0) {
		std::string path = queue[0];
		queue.pop_front();
		process(path.c_str());
	}
}

void print_md5map()
{
	std::map<std::string, std::string>::iterator i;
	for (i = md5map.begin(); i != md5map.end(); ++i) {
		std::string path   = i->first;
		std::string md5str = i->second;
		std::cout << md5str << "  " << path << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1) usage();

	for (int i = 1; i < argc; ++i) {
		queue.push_back(argv[i]);
	}

	check_queue();
	process_queue();

	print_md5map();
	
	return 0;
}
