namespace amiibo
{
    class AmiiboFile {
    	public:
    		AmiiboFile(std::string Name, std::string DirPath, std::string IconPath);
    		std::string GetName();
    		std::string GetPath();
            std::string GetIconPath();
    	private:
    		std::string Name;
    		std::string DirPath;
            std::string IconPath;
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
