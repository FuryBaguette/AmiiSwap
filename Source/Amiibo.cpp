#include "MainApplication.hpp"

namespace amiibo
{
	AmiiboFile::AmiiboFile(std::string Name, std::string BinPath)
	{
		this->Name = Name;
		this->BinPath = BinPath;
	}

	std::string AmiiboFile::GetName()
	{
		return (this->Name);
	}

	std::string AmiiboFile::GetPath()
	{
		return (this->BinPath);
	}

	AmiiboGame::AmiiboGame(std::string Name, std::string FolderPath)
	{
		this->Name = Name;
		this->FolderPath = FolderPath;
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
