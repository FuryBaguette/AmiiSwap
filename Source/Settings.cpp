#include "MainApplication.hpp"
#include <iostream>
#include <fstream>
namespace set
{
	void Initialize(std::string settingsPath)
	{
		FindAllAmiibos();
		if (SettingsExists(settingsPath)) {
			PopulateGames(settingsPath);
			UpdateSettingsFile(settingsPath);
		} else {
			CreateSettingsFile(settingsPath);
		}
		PopulateGames(settingsPath);
	}

	std::vector<amiibo::Game*> GetGames()
	{
		return MainSettings.games;
	}

	int GetGamesSize()
	{
		return MainSettings.games.size();
	}

	std::vector<amiibo::Amiibo*> GetAllAmiibos()
	{
		return MainSettings.allAmiibos;
	}

	void AddGame(amiibo::Game* game)
	{
		MainSettings.games.insert(MainSettings.games.end(), game);
		sort(MainSettings.games.begin(), MainSettings.games.end(), utils::GamesSort);
		UpdateSettingsFile("sdmc:/switch/AmiiSwap/settings.txt");
	}

	void RemoveGame(pu::String gameName)
	{
		int position = 0;
		for (auto & game : MainSettings.games) {
			if (game->GetName() == gameName) {
				MainSettings.games.erase(MainSettings.games.begin() + position);
				break;
			}
			position++;
		}
		UpdateSettingsFile("sdmc:/switch/AmiiSwap/settings.txt");
	}

	bool SettingsExists(std::string settingsPath)
	{
		std::ifstream settingsIfs(settingsPath);
		if (settingsIfs.good()) {
			if (settingsIfs.is_open())
				settingsIfs.close();
			return true;
		} else {
			return false;
		}
	}

	void CreateSettingsFile(std::string settingsPath)
	{
		std::ofstream settingsOfs(settingsPath);
		settingsOfs << "[ALL]" << "\r" << "\n";
		for (auto & amiibo : MainSettings.allAmiiboNames) {
			settingsOfs << amiibo.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
		}
		if (settingsOfs.is_open())
			settingsOfs.close();
	}

	void UpdateSettingsFile(std::string settingsPath)
	{
		std::vector<std::string> amiibosInAll;
		std::vector<std::string> amiibosInConfig;

		bool all = false;
		for (auto & game : MainSettings.games) {
			if (game->GetName() == "ALL") {
				all = true;
			} else {
				std::vector<std::string> files = game->GetAmiibos();
				for (auto & element : game->GetAmiibos()) {
					if (find(amiibosInConfig.begin(), amiibosInConfig.end(), element) == amiibosInConfig.end()) {
						amiibosInConfig.insert(amiibosInConfig.end(), element);
					}
				}
				std::vector<std::string>().swap(files);
			}
		}
		std::sort(amiibosInConfig.begin(), amiibosInConfig.end(), &utils::NoPathSort);

		for (auto & element : MainSettings.allAmiiboNames) {
			if (find(amiibosInConfig.begin(), amiibosInConfig.end(), element.substr(sizeof(emuiiboFolder))) == amiibosInConfig.end()) {
				amiibosInAll.insert(amiibosInAll.end(), element);
			}
		}
		
		std::ofstream settingsOfs(settingsPath, std::ofstream::trunc);

		if (!all) {
			settingsOfs << "[ALL]" << "\r" << "\n";
			for (auto & element : amiibosInAll) {
				settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
			}
		}

		for (auto& game : MainSettings.games) {
			if (game->GetName() == "ALL") {
				settingsOfs << "[ALL]" << "\r" << "\n";
				for (auto & element : amiibosInAll) {
					settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
				}
			}
			else {
				std::vector<std::string> amiibos = game->GetAmiibos();
				settingsOfs << "[" << game->GetName().AsUTF8() << "]" << "\r" << "\n";
				for (auto element : amiibos) {
					settingsOfs << element << "\r" << "\n";
				}
				std::vector<std::string>().swap(amiibos);
			}
		}

		if (settingsOfs.is_open())
			settingsOfs.close();
	}

	void FindAllAmiibos()
	{
		std::vector<amiibo::Amiibo*>().swap(MainSettings.allAmiibos);
		std::vector<std::string>().swap(MainSettings.allAmiiboNames);
		std::string name;
		utils::get_amiibos_directories(emuiiboFolder, &MainSettings.allAmiiboNames);
		std::sort(MainSettings.allAmiiboNames.begin(), MainSettings.allAmiiboNames.end(), &utils::NoPathSort);
		for (auto& amiibo : MainSettings.allAmiiboNames) {
			std::size_t found = amiibo.find_last_of("/\\");
			if (found != std::string::npos) {
				name = amiibo.substr(found + 1);
			}
			else {
				name = amiibo;
			}
			if (name != "miis") {
				amiibo::Amiibo* file = new amiibo::Amiibo(name, amiibo, amiibo + "/amiibo.icon");
				MainSettings.allAmiibos.insert(MainSettings.allAmiibos.end(), file);
			}
		}
	}

	void PopulateGames(std::string settingsPath)
	{
		std::vector<amiibo::Game*>().swap(MainSettings.games);
		bool inGame = false;
		std::ifstream file(settingsPath);
		std::string line;
		amiibo::Game* currentGame;

		while (std::getline(file, line)) {
			if (line.at(0) == '[') {
				if (inGame)
					MainSettings.games.insert(MainSettings.games.end(), currentGame);
				else
					inGame = true;
				std::string gameName = line.substr(1, line.size() - 3);
				currentGame = new amiibo::Game(gameName);
			}
			else if (isalnum(line.at(0))) {
				std::string amiiboName = line.substr(0, line.size() - 1);
				std::string amiiboDir = std::string(emuiiboFolder) + "/" + amiiboName;
				if (utils::folderExists(amiiboDir)) {
					currentGame->AddAmiibo(amiiboName);
				}
			}
		}
		MainSettings.games.insert(MainSettings.games.end(), currentGame);
		sort(MainSettings.games.begin(), MainSettings.games.end(), utils::GamesSort);
	}
}
