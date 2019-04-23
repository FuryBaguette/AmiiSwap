#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <filesystem>

namespace utils
{
    void CreateDirectory(std::string Path);
    void EnsureDirectories();
    void getFiles(const std::string &path, std::function<void(const std::string &)> cb);
    std::vector<std::string> get_directories(char *path);
    bool IsEmuiiboPresent();
    bool copyFile(const char *SRC, const char* DEST);
    inline bool fileExists (const std::string& name) {
	  struct stat buffer;
	  return (stat (name.c_str(), &buffer) == 0);
	}
}
