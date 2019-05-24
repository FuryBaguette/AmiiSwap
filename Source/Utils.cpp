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

    void get_amiibos_directories(char *path, std::vector<std::string> *r)
    {
        DIR *dir = opendir(path);

        while (auto entry = readdir(dir))
        {
            if (strcasecmp(entry->d_name,"amiibo.bin")==0){
                r->push_back(std::string(path));
                continue;
            }
            if (entry->d_type == DT_DIR){
                std::string subfolder = std::string(path) + "/" + entry->d_name;
                get_amiibos_directories(subfolder.c_str(), r);
            }
        }
        closedir(dir);
        //return r;
    }

    std::string GetRomFsResource(std::string path)
    {
        if(path.at(0)=='/')
            return "romfs:" + path;
        else
            return "romfs:/" + path;            
    }

    void Log(std::string Text)
    {
        std::string logPath = "sdmc:/switch/AmiiSwap/log.txt";
        std::ofstream logOfs(logPath, std::ofstream::app);
        logOfs << Text << "\r" << "\n";
        if(logOfs.is_open())
            logOfs.close();
    }

    std::string UserInput(std::string Guide, std::string Initial)
    {
        std::string out = "";
        char tmpout[FS_MAX_PATH] = { 0 };
        SwkbdConfig kbd;
        Result rc = swkbdCreate(&kbd, 0);
        if(rc == 0)
        {

            swkbdConfigMakePresetDefault(&kbd);
            if(Guide != "") swkbdConfigSetGuideText(&kbd, Guide.c_str());
            if(Initial != "") swkbdConfigSetInitialText(&kbd, Initial.c_str());
            rc = swkbdShow(&kbd, tmpout, sizeof(tmpout));
            if(rc == 0) out = std::string(tmpout);
        }
        swkbdClose(&kbd);
        return out;
    }

    bool NoPathSort(const std::string& a, const std::string& b)
    {
        std::string astring = a;
        std::string bstring = b;
        size_t found = astring.find_last_of("/\\");;
        astring = astring.substr(found + 1);
        found = bstring.find_last_of("/\\");
        bstring = bstring.substr(found + 1);
        transform(astring.begin(), astring.end(), astring.begin(), ::tolower);
        transform(bstring.begin(), bstring.end(), bstring.begin(), ::tolower);
        return astring < bstring;
    }

    bool GamesSort(const amiibo::AmiiboGame* a, const amiibo::AmiiboGame* b)
    {
        std::string astring = a->GetName();
        std::string bstring = b->GetName();
        if(astring == "ALL") return true;
        if(bstring == "ALL") return false;
        transform(astring.begin(), astring.end(), astring.begin(), ::tolower);
        transform(bstring.begin(), bstring.end(), bstring.begin(), ::tolower);
        return astring < bstring;
    }

    bool AmiibosSort(const amiibo::AmiiboFile* a, const amiibo::AmiiboFile* b)
    {
        std::string astring = a->GetName();
        std::string bstring = b->GetName();
        transform(astring.begin(), astring.end(), astring.begin(), ::tolower);
        transform(bstring.begin(), bstring.end(), bstring.begin(), ::tolower);
        return astring < bstring;
    }

    const std::string forbiddenChars = "\\/\"\n\r\t[]";
    char ClearForbidden(char toCheck)
    {
        if(forbiddenChars.find(toCheck) != std::string::npos)
            return ' ';
        return toCheck;
    }
}
