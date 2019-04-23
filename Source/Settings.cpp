#include "MainApplication.hpp"

namespace set
{
	Settings::Settings(std::string path)
	{
		this->settingsPath = path;

		bool inGame = false;
		std::ifstream file(path);
		std::string line;
		amiibo::AmiiboGame *currentGame;

		while (std::getline(file, line)) {
			if (line.at(0) == '[') {
				if (inGame) this->games.push_back(currentGame);
				else inGame = true;
				std::string gameName = line.substr(1, line.size() - 3);
				currentGame = new amiibo::AmiiboGame(gameName);
			} else if (isalnum(line.at(0))) {
				std::string amiiboName = line.substr(0, line.size()-1);
				std::string binFile = "sdmc:/emuiibo/" + amiiboName + "/amiibo.bin";
				if (utils::fileExists(binFile)) {
					amiibo::AmiiboFile *file = new amiibo::AmiiboFile(amiiboName, binFile, "sdmc:/emuiibo/" + amiiboName + "/amiibo.png");
					currentGame->AddAmiiboFile(file);
				}
			}
		}
		this->games.push_back(currentGame);
	}

	std::vector<amiibo::AmiiboGame*> Settings::GetGames()
	{
		return (this->games);
	}
}
