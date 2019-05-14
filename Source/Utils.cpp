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

    void InitSettings()
    {
        std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt";
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> allAmiibos;
        get_amiibos_directories(emuiiboFolder, &allAmiibos);
        allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/miis"), allAmiibos.end());
        std::ifstream settingsIfs(settingsPath);
        if(!settingsIfs.good()){ //no settings.txt found, generate.
           std::ofstream settingsOfs(settingsPath);
            settingsOfs << "[ALL]" << "\r" << "\n";
            for (auto & elem : allAmiibos) {
                settingsOfs << elem.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
            }
            if(settingsOfs.is_open())
                settingsOfs.close();
        } else { // settings.txt available compare with actually available amiibos and rewrite
            if(settingsIfs.is_open())
                settingsIfs.close();
            set::Settings *settings = new set::Settings(settingsPath);
            std::vector<amiibo::AmiiboGame*> configuredGames = settings->GetGames();
            std::vector<std::string> configuredAmiibos;
            std::vector<std::string> removedAmiibos;
            for(auto & game : configuredGames) {
                std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
                for (auto & element : files) {
                    configuredAmiibos.push_back(element->GetName());
                } 
            }
            
            for(auto & element : configuredAmiibos){
                if(find(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element) != allAmiibos.end()){
                    allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element), allAmiibos.end());
                } else {
                    removedAmiibos.push_back(element);
                }
            }
            
            std::ofstream settingsOfs(settingsPath,std::ofstream::trunc);
            bool allwritten = false;
            for(auto & game : configuredGames) {
                bool writegame = true;
                std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
                for (auto & element : files) {
                    if(find(removedAmiibos.begin(), removedAmiibos.end(), element->GetName()) == removedAmiibos.end()){ // missing amiibos are already ignored when creating menu but let's keep settings clean
                        if(writegame){
                            settingsOfs << "[" << game->GetName() <<"]" << "\r" << "\n";
                            writegame = false;
                        }
                        settingsOfs << element->GetName() << "\r" << "\n";
                    }
                }
                if (game->GetName() == "ALL" && !writegame){
                    allwritten = true;
                    for(auto & element : allAmiibos){
                        settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
                    }
                }
            }
            
            if(!allwritten && !allAmiibos.empty()){
                settingsOfs << "[ALL]" << "\r" << "\n";
                for(auto & element : allAmiibos){
                    settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
                }
            }
            
            if(settingsOfs.is_open())
                settingsOfs.close();
        }
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
        return r;
    }

    std::string GetRomFsResource(std::string path)
    {
        if(path.at(0)=='/')
            return "romfs:" + path;
        else
            return "romfs:/" + path;            
    }
}
