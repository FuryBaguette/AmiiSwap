#include <MainApplication.hpp>

namespace utils
{
    void CreateDirectory(std::string Path)
    {
    	int res = mkdir(Path.c_str(), 777);
    }

    bool IsEmuiiboPresent()
    {
        Handle tmph = 0;
        Result rc = smRegisterService(&tmph, "nfp:emu", false, 1);
        if(R_FAILED(rc))
    		return true;
        smUnregisterService("nfp:emu");
        return false;
    }

    void EnsureDirectories()
    {
        CreateDirectory("sdmc:/emuiibo");
    }

    void getFiles(const std::string &path, std::function<void(const std::string &)> cb)
    {
    	if (auto dir = opendir(path.c_str())) {
    		while (auto f = readdir(dir)) {
    			if (!f->d_name || f->d_name[0] == '.') continue;
    			if (f->d_type == DT_DIR)
    				getFiles(path + f->d_name + "/", cb);

    			if (f->d_type == DT_REG) {
    				const char* ext = ui::getExtension(f->d_name);
    				if (strcasecmp(ext, ".bin")==0)
    					cb(path + f->d_name);
    			}
    		}
    		closedir(dir);
    	}
    }

    std::vector<std::string> get_directories(char *path)
    {
        std::vector<std::string> r;
        DIR *dir = opendir(path);

        struct dirent *entry = readdir(dir);

        while (entry != NULL)
        {
            if (entry->d_type == DT_DIR)
                r.push_back(std::string(path) + std::string("/") + std::string(entry->d_name));

            entry = readdir(dir);
        }
        closedir(dir);
        return r;
    }

    bool copyFile(const char *SRC, const char* DEST)
    {
        std::ifstream src(SRC, std::ios::binary);
        std::ofstream dest(DEST, std::ios::binary);
        dest << src.rdbuf();
        return src && dest;
    }
}
