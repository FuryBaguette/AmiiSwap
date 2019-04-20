namespace amiibo
{
    class AmiiboFile {
    	public:
    		AmiiboFile(std::string Name, std::string BinPath);
    		std::string GetName();
    		std::string GetPath();
    	private:
    		std::string Name;
    		std::string BinPath;
    };

    class AmiiboGame {
    	public:
    		AmiiboGame(std::string Name, std::string FolderPath);
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
