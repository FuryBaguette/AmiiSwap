namespace amiibo
{
    class Game;
    class Amiibo {
    	public:
    		Amiibo(std::string Name, std::string DirPath, std::string IconPath);
    		std::string GetName();
    		std::string GetPath();
            std::string GetIconPath();
            std::vector<Game *> GetParents(std::vector<Game *> games);
            std::string DisplayParents(std::vector<Game *> games);
    	private:
    		std::string Name;
    		std::string DirPath;
            std::string IconPath;
            std::vector<Game *> Parents;
    };

    class Game {
    	public:
    		Game(std::string Name);
    		pu::String GetName();
    		std::string GetPath();
    		std::vector<std::string> GetAmiibos();
    		void AddAmiibo(std::string amiibo);
    		void SetAmiibos(std::vector<std::string> amiibos);
    	private:
    		std::string Name;
    		std::string FolderPath;
    		std::vector<std::string> amiibos;
    };
}
