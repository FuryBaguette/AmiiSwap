namespace amiibo
{
    class AmiiboGame;
    class AmiiboFile {
    	public:
    		AmiiboFile(std::string Name, std::string DirPath, std::string IconPath);
    		std::string GetName();
    		std::string GetPath();
            std::string GetIconPath();
            std::vector<AmiiboGame *> GetParents(std::vector<AmiiboGame *> games);
            std::string DisplayParents(std::vector<AmiiboGame *> games);
    	private:
    		std::string Name;
    		std::string DirPath;
            std::string IconPath;
            std::vector<AmiiboGame *> Parents;
    };

    class AmiiboGame {
    	public:
    		AmiiboGame(std::string Name);
    		std::string GetName();
    		std::string GetPath();
    		std::vector<AmiiboFile*> GetBinFiles();
    		void AddAmiiboFile(AmiiboFile*);
    		void SetAmiiboFiles(std::vector<AmiiboFile*> amiiboFiles);
    	private:
    		std::string Name;
    		std::string FolderPath;
    		std::vector<AmiiboFile*> amiiboFiles;
    };
}
