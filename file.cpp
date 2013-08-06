#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>

extern "C" {
#include "md5.h"
};

#include <string>
#include <map>
#include <queue>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

#include "log.h"

bool is_file(const std::string &path)
{
	struct stat st;
	if (stat(path.c_str(), &st) != 0) return false;

	if (S_ISREG(st.st_mode)) return true; // normal file
	
	return false;
}

bool is_directory(const std::string &path)
{
	struct stat st;
	if (stat(path.c_str(), &st) != 0) return false;

	if (S_ISDIR(st.st_mode)) return true; // directory
	
	return false;
}

bool exists(const char *path)
{
	if (is_file(path) == false && is_directory(path) == false) {
		return false;
	}
	return true;
}

bool exists(const std::vector<std::string> &paths)
{
	std::vector<std::string>::const_iterator i;
	for (i = paths.begin(); i != paths.end(); ++i) {
		if (exists(i->c_str()) == false) {
			log_e("path not found...path=%s", i->c_str());
			return false;
		}
	}
	return true;
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

std::string get_file_hash(const std::string &path)
{
	MD5_CTX ctx;
	unsigned char md5[16];
	unsigned char buf[1024];

	MD5_Init(&ctx);

	FILE *fp = fopen(path.c_str(), "rb");
	if (fp == 0) {
		log_e("cannot open...file=%s", path.c_str());
		return std::string();
	}

	while(true) {
		size_t size = fread(buf, 1, 1024, fp);
		if (size <= 0) break;
		MD5_Update(&ctx, buf, size);
	}

	fclose(fp);
	fp = NULL;

	MD5_Final(md5, &ctx);
	
	return md5str(md5);
}

bool directory_traversal(const std::string &path, std::deque<std::string> &queue)
{
	DIR *dir = opendir(path.c_str());
	if (dir == NULL) return false;

	struct dirent *d = NULL;
	while(true) {
		d = readdir(dir);
		if (d == NULL) break;

		std::string p(d->d_name);
		if (p == "." || p == "..") continue;

		if (d->d_type == DT_BLK)  continue;
		if (d->d_type == DT_CHR)  continue;
		if (d->d_type == DT_FIFO) continue;
		if (d->d_type == DT_LNK)  continue;
		if (d->d_type == DT_SOCK) continue;

		// append normal file & directory name
		queue.push_back(path + std::string("/") + p);
	}

	closedir(dir);
	dir = NULL;
	
	return true;
}

void print_paths(const std::vector<std::string> &paths)
{
	std::vector<std::string>::const_iterator i;
	for (i = paths.begin(); i != paths.end(); ++i) {
		std::cerr << *i << std::endl;
	}
}

void process(const std::vector<std::string> &paths, std::map<std::string, std::string> &md5map)
{
	if (paths.size() == 0) return;

	// copy vector -> deque
	std::deque<std::string> queue;
	std::vector<std::string>::const_iterator i;
	for (i = paths.begin(); i != paths.end(); ++i) {
		queue.push_back(*i);
	}

	// process queue
	while(queue.size() > 0) {
		std::string path = queue[0];
		queue.pop_front();

		if (is_file(path)) {
			std::string h = get_file_hash(path);
			if (h.size() != 0) {
				md5map[path] = h;
			}
		}
		else if (is_directory(path)) {
			directory_traversal(path, queue);
		}
	}
}

void print_md5map(const std::map<std::string, std::string> &md5map)
{
	std::map<std::string, std::string>::const_iterator i;
	for (i = md5map.begin(); i != md5map.end(); ++i) {
		std::string path   = i->first;
		std::string md5str = i->second;
		std::cout << md5str << "," << path << std::endl;
	}
}

bool save_md5map(const std::string &filename, const std::map<std::string, std::string> &md5map)
{
	std::ofstream ofs;
	ofs.open(filename.c_str(), std::ios::out);
	if (!ofs) {
		log_e("cannot open file...filename=%s", filename.c_str());
		return false;
	}

	std::map<std::string, std::string>::const_iterator i;
	for (i = md5map.begin(); i != md5map.end(); ++i) {
		std::string path   = i->first;
		std::string md5str = i->second;
		ofs << md5str << "," << path << std::endl;
	}

	ofs.close();

	return true;
}

std::string chomp(const std::string &str)
{
	std::stringstream buf;
	for (unsigned int i = 0; i < str.size(); ++i) {
		if (str[i] == 0x0a || str[i] == 0x0d) continue;
		buf << str[i];
	}
	return buf.str();
}

bool load_md5map(const std::string &filename, std::map<std::string, std::string> &md5map)
{
	md5map.clear();

	if (is_file(filename) == false) {
		return false;
	}

	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in);
	if (!ifs) {
		return false;
	}

	int line_num = 1;
	while(!ifs.eof()) {
		std::string l;
		std::getline(ifs, l);
		l = chomp(l);

		if (l.size() == 0) break;
		if (l.size() < 34) {
			log_e("load_md5map() : detect broken data...line=%d", line_num);
			return false;
		}

		std::string md5  = l.substr(0, 32);
		std::string path = l.substr(33, l.size()-33);

		md5map[path] = md5;

		line_num ++;
	}

	return true;
}

void check_new(const std::map<std::string, std::string> &old_md5map, const std::map<std::string, std::string> &new_md5map)
{
	std::map<std::string, std::string>::const_iterator i;
	for (i = new_md5map.begin(); i != new_md5map.end(); ++i) {
		std::string new_path = i->first;
		std::string new_md5  = i->second;

		if (old_md5map.find(new_path) == old_md5map.end()) {
			log_i("N,%s", new_path.c_str());
		}
	}
}

void check_del(const std::map<std::string, std::string> &old_md5map, const std::map<std::string, std::string> &new_md5map)
{
	std::map<std::string, std::string>::const_iterator i;
	for (i = old_md5map.begin(); i != old_md5map.end(); ++i) {
		std::string old_path = i->first;
		std::string old_md5  = i->second;

		if (new_md5map.find(old_path) == new_md5map.end()) {
			log_i("D,%s", old_path.c_str());
		}
	}
}

void check_modify(const std::map<std::string, std::string> &old_md5map, const std::map<std::string, std::string> &new_md5map)
{
	std::map<std::string, std::string>::const_iterator i;
	std::map<std::string, std::string>::const_iterator f;
	for (i = old_md5map.begin(); i != old_md5map.end(); ++i) {
		std::string old_path = i->first;
		std::string old_md5  = i->second;

		f = new_md5map.find(old_path);
		if (f != new_md5map.end()) {
			std::string new_path = f->first;
			std::string new_md5  = f->second;
			
			if (old_md5 != new_md5) {
				log_i("M,%s", old_path.c_str());
			}
		}
	}
}

void compare(const std::map<std::string, std::string> &old_md5map, const std::map<std::string, std::string> &new_md5map)
{
	check_new(old_md5map, new_md5map);
	check_del(old_md5map, new_md5map);
	check_modify(old_md5map, new_md5map);
}

