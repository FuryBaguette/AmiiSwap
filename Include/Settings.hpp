namespace set
{
    class Settings {
    	public:
            Settings(std::string path);
    		std::vector<amiibo::AmiiboGame*> GetGames();
            void AddGame(amiibo::AmiiboGame *game);
    	private:
    		std::vector<amiibo::AmiiboGame*> games;
            std::string settingsPath;
    };

}
