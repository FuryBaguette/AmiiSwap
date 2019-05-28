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
    void get_amiibos_directories(char *path, std::vector<std::string> *r);
    std::string GetRomFsResource(std::string path);
    void Log(std::string Text);
    std::string UserInput(std::string Guide, std::string Initial);
    bool NoPathSort(const std::string& a, const std::string& b);
    bool GamesSort(const amiibo::AmiiboGame* a, const amiibo::AmiiboGame* b);
    bool AmiibosSort(const amiibo::AmiiboFile* a, const amiibo::AmiiboFile* b);
    char ClearForbidden(char toCheck);
    std::string bufferToString(char* buffer, int bufflen);
    char* bufferToCString(char *buff, int buffSize, char *str);
    inline bool folderExists(const std::string& dirName) {
        DIR *dir = opendir(dirName.c_str());
        if (dir) {
            closedir(dir);
            return (true);
        }
        return (false);
    }

    inline bool fileExists(const std::string& name) {
	  struct stat buffer;
	  return (stat (name.c_str(), &buffer) == 0);
	}
}
