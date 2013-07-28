#ifndef ____FILE__H____
#define ____FILE__H____

#include <string>
#include <map>
#include <vector>

extern bool exists(const std::vector<std::string> &paths);

extern void process(const std::vector<std::string> &paths, std::map<std::string, std::string> &md5map);

extern void print_md5map(const std::map<std::string, std::string> &md5map);

extern bool save_md5map(const std::string &filename, const std::map<std::string, std::string> &md5map);
extern bool load_md5map(const std::string &filename, std::map<std::string, std::string> &md5map);

#endif // #define ____FILE__H____
