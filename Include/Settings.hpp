namespace set
{
    class Settings {
    	public:
            Settings(std::string path);
    		std::vector<amiibo::AmiiboGame*> GetGames();
    	private:
    		std::vector<amiibo::AmiiboGame*> games;
            std::string settingsPath;
    };

}
