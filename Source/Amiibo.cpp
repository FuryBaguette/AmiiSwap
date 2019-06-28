#include "MainApplication.hpp"

namespace amiibo
{
	Amiibo::Amiibo(std::string Name, std::string DirPath, std::string IconPath)
	{
		this->Name = Name;
		this->DirPath = DirPath;
		this->IconPath = IconPath;
	}

	std::string Amiibo::GetName()
	{
		return this->Name;
	}

	std::string Amiibo::GetPath()
	{
		return this->DirPath;
	}

	std::string Amiibo::GetIconPath()
	{
		return this->IconPath;
	}

	std::vector<Game*> Amiibo::GetParents(std::vector<Game *> games)
	{
		std::vector<Game *> parents;

		for (auto & game : games) {
			for (auto & amiibo : game->GetAmiibos()) {
				std::string amiiboName = amiibo;
				std::string amiiboPath = "sdmc:/emuiibo/" + amiibo;
				size_t size = amiibo.find_last_of("/\\");
				if (size != std::string::npos)
                	amiiboName = amiibo.substr(size + 1);
				if (amiiboName == this->Name && amiiboPath == this->DirPath)
					parents.insert(parents.end(), game);
			}
		}
		return parents;
	}

	std::string Amiibo::DisplayParents(std::vector<Game *> games)
	{
		std::vector<Game *> amiiboParents = this->GetParents(games);
		std::string parents;

		for (auto & elem : amiiboParents) {
			parents += elem->GetName() + ", ";
		}
		return parents;
	}

	Game::Game(std::string Name)
	{
		this->Name = Name;
	}

	std::string Game::GetName()
	{
		return this->Name;
	}

	std::string Game::GetPath()
	{
		return this->FolderPath;
	}

	std::vector<std::string> Game::GetAmiibos()
	{
		return this->amiibos;
	}

	void Game::AddAmiibo(std::string amiibo)
	{
		this->amiibos.insert(this->amiibos.end(), amiibo);
		std::sort(this->amiibos.begin(), this->amiibos.end(), utils::StringSort);
	}

	void Game::SetAmiibos(std::vector<std::string> newAmiibos)
	{
		newAmiibos.swap(this->amiibos);
	}
}
