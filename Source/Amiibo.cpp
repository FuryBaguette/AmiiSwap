#include "MainApplication.hpp"

namespace amiibo
{
	AmiiboFile::AmiiboFile(std::string Name, std::string DirPath, std::string IconPath)
	{
		this->Name = Name;
		this->DirPath = DirPath;
		this->IconPath = IconPath;
	}

	std::string AmiiboFile::GetName()
	{
		return (this->Name);
	}

	std::string AmiiboFile::GetPath()
	{
		return (this->DirPath);
	}

	std::string AmiiboFile::GetIconPath()
	{
		return (this->IconPath);
	}

	std::vector<AmiiboGame*> AmiiboFile::GetParents(std::vector<AmiiboGame *> games)
	{
		std::vector<AmiiboGame *> parents;

		for (auto & game : games) {
			for (auto & amiibo : game->GetBinFiles()) {
				if (amiibo->GetName() == this->Name && amiibo->GetPath() == this->DirPath)
					parents.push_back(game);
			}
		}
		return (parents);
	}

	std::string AmiiboFile::DisplayParents(std::vector<AmiiboGame *> games)
	{
		std::vector<AmiiboGame *> amiiboParents = this->GetParents(games);
		std::string parents;

		for (auto & elem : amiiboParents) {
			parents += elem->GetName() + ", ";
		}
		return (parents);
	}

	AmiiboGame::AmiiboGame(std::string Name)
	{
		this->Name = Name;
	}

	std::string AmiiboGame::GetName()
	{
		return (this->Name);
	}

	std::string AmiiboGame::GetPath()
	{
		return (this->FolderPath);
	}

	std::vector<AmiiboFile*> AmiiboGame::GetBinFiles()
	{
		return (this->amiiboFiles);
	}

	void AmiiboGame::AddAmiiboFile(AmiiboFile *file)
	{
		this->amiiboFiles.push_back(file);
	}

	void AmiiboGame::SetAmiiboFiles(std::vector<AmiiboFile*> files)
	{
		this->amiiboFiles.insert(this->amiiboFiles.end(), files.begin(), files.end());
	}
}
