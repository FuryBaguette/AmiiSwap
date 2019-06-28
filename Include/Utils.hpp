#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <filesystem>
#include <regex>
#include "json.hpp"

using json = nlohmann::json;

namespace utils
{
    void CreateDirectory(std::string Path);
    void EnsureDirectories();
    void getFiles(const std::string &path, std::function<void(const std::string &)> cb);
    std::vector<std::string> get_directories(char *path);
    bool IsEmuiiboPresent();
    bool copyFile(const char *SRC, const char* DEST);
    void get_amiibos_directories(char *path, std::vector<std::string> *r);
    void getImages(const std::string &path, std::vector<std::string> *r);
    std::string GetRomFsResource(std::string path);
    void Log(std::string Text);
    std::string UserInput(std::string Guide, std::string Initial);
    bool NoPathSort(const std::string& a, const std::string& b);
    bool GamesSort(const amiibo::Game* a, const amiibo::Game* b);
    bool AmiibosSort(const amiibo::Amiibo* a, const amiibo::Amiibo* b);
    bool StringSort(const std::string& a, const std::string& b);
    char ClearForbidden(char toCheck);
    std::string bufferToString(char* buffer, int bufflen);
    char* bufferToCString(char *buff, int buffSize, char *str);
    std::string getLastFromPath(std::string str);
    std::string getActiveAmiibo();
    std::vector<std::string> ReadFileLines(std::string Path, u32 LineOffset, u32 LineCount);
    u64 GetFileSize(std::string Path);
    u64 ReadFileBlock(std::string Path, u64 Offset, u64 Size, u8 *Out);
    bool isRandomUuid(std::string jsonPath);
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

    inline std::string trim_right_copy(const std::string& s, const std::string& delimiters = " \f\n\r\t\v\0" )
    {
        return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
    }

	inline std::string replace(std::string s, const std::string& f, const std::string& r)
	{
		size_t i = 0;
		i = s.find(f, i);
		if (i == std::string::npos) {}
		else
			s.replace(i, f.length(), r);
		return s;
	}
}
