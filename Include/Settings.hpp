namespace set
{
	struct Settings
	{
		std::vector<amiibo::Game*> games;
		std::vector<amiibo::Amiibo*> allAmiibos;
		std::vector<std::string> allAmiiboNames;
	};

	const char emuiiboFolder[] = "sdmc:/emuiibo";
	static Settings MainSettings;
	std::vector<amiibo::Game*> GetGames();
	int GetGamesSize();
	std::vector<amiibo::Amiibo*> GetAllAmiibos();
	void AddGame(amiibo::Game* game);
	void RemoveGame(std::string gameName);
	void Initialize(std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt");
	bool SettingsExists(std::string settingsPath);
	void CreateSettingsFile(std::string settingsPath);
	void UpdateSettingsFile(std::string settingsPath);
	void FindAllAmiibos();
	void PopulateGames(std::string settingsPath);
}
