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
        CreateDirectory("sdmc:/switch/AmiiSwap/game_icons");
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

    void getImages(const std::string &path, std::vector<std::string> *r)
    {
    	if (auto dir = opendir(path.c_str())) {
    		while (auto f = readdir(dir)) {
    			if (!f->d_name || f->d_name[0] == '.') continue;
    			if (f->d_type == DT_REG) {
    				const char* ext = ui::getExtension(f->d_name);
    				if (strcasecmp(ext, ".png") == 0 || strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0)
    					r->insert(r->end(), path + "/" + f->d_name);
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
                r.insert(r.end(), std::string(path) + std::string("/") + std::string(entry->d_name));

            entry = readdir(dir);
        }
        closedir(dir);        
        r.shrink_to_fit();
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
                r->insert(r->end(), std::string(path));
                continue;
            }
            if (entry->d_type == DT_DIR){
                std::string subfolder = std::string(path) + "/" + entry->d_name;
                get_amiibos_directories(subfolder.c_str(), r);
            }
        }
        closedir(dir);
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

    bool GamesSort(const amiibo::Game* a, const amiibo::Game* b)
    {
        std::string astring = a->GetName();
        std::string bstring = b->GetName();
        if(astring == "ALL") return true;
        if(bstring == "ALL") return false;
        transform(astring.begin(), astring.end(), astring.begin(), ::tolower);
        transform(bstring.begin(), bstring.end(), bstring.begin(), ::tolower);
        return astring < bstring;
    }

    bool AmiibosSort(const amiibo::Amiibo* a, const amiibo::Amiibo* b)
    {
        std::string astring = a->GetName();
        std::string bstring = b->GetName();
        transform(astring.begin(), astring.end(), astring.begin(), ::tolower);
        transform(bstring.begin(), bstring.end(), bstring.begin(), ::tolower);
        return astring < bstring;
    }

    bool StringSort(const std::string& a, const std::string& b)
    {
        std::string astring = a;
        std::string bstring = b;
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

    std::string bufferToString(char* buffer, int bufflen)
    {
        std::string ret(buffer, bufflen);
        return ret;
    }

    char* bufferToCString(char *buff, int buffSize, char *str)
    {
        memset(str, '\0', buffSize + 1);
        return(strncpy(str, buff, buffSize));
    }

    std::string getLastFromPath(std::string str)
    {
        std::size_t found = str.find_last_of("/\\");
        return str.substr(found+1);
    }

    bool isRandomUuid(std::string jsonPath)
    {
        std::ifstream ifs(jsonPath);
        auto amiiboJson = json::parse(ifs);
        if(ifs.is_open())
            ifs.close();
        return amiiboJson.value<bool>("randomizeUuid", false);
    }

    std::string getActiveAmiibo()
    {
        char key[] = { 0 };
        char amiibo[FS_MAX_PATH] = { 0 };
		Result rs = nfpemuGetAmiibo(amiibo);
        if(rs == 0 && R_FAILED(strcmp(key,amiibo)))
            return utils::getLastFromPath(std::string(amiibo));
        return "none";
    }

    std::vector<std::string> ReadFileLines(std::string Path, u32 LineOffset, u32 LineCount)
    {
        std::vector<std::string> data;
        std::string path = Path;
        u64 fsize = GetFileSize(path);
        if(fsize == 0) return data;
        std::string tmpline;
        u32 tmpc = 0;
        u32 tmpo = 0;
        u64 szrem = fsize;
        u64 off = 0;
        u8 *tmpdata = (u8*)memalign(0x1000, 0x100);
        bool end = false;
        while(szrem && !end)
        {
            u64 rsize = ReadFileBlock(path, off, std::min((u64)0x100, szrem), tmpdata);
            if(rsize == 0) return data;
            szrem -= rsize;
            off += rsize;
            for(u32 i = 0; i < rsize; i++)
            {
                char ch = (char)tmpdata[i];
                if(ch == '\n')
                {
                    if(tmpc >= LineCount)
                    {
                        end = true;
                        break;
                    }
                    if((tmpo < LineOffset) && (LineOffset != 0))
                    {
                        tmpo++;
                        tmpline = "";
                        continue;
                    }
                    std::string tab = "\t";
                    while(true)
                    {
                        size_t spos = tmpline.find(tab);
                        if(spos == std::string::npos) break;
                        tmpline.replace(spos, tab.length(), "    ");
                    }
                    data.insert(data.end(), tmpline);
                    tmpc++;
                    tmpline = "";
                }
                else tmpline += (char)ch;
            }
        }
        free(tmpdata);
        return data;
    }

    u64 GetFileSize(std::string Path)
    {
        u64 sz = 0;
        std::string path = Path;
        struct stat st;
        if(stat(path.c_str(), &st) == 0) sz = st.st_size;
        return sz;
    }

    u64 ReadFileBlock(std::string Path, u64 Offset, u64 Size, u8 *Out)
    {
        u64 rsz = 0;
        std::string path = Path;
        FILE *f = fopen(path.c_str(), "rb");
        if(f)
        {
            fseek(f, Offset, SEEK_SET);
            rsz = fread(Out, 1, Size, f);
        }
        fclose(f);
        return rsz;
    }
}
