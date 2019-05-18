#pragma once

#include <pu/Plutonium>

namespace ui
{
	class MainLayout : public pu::Layout
	{
	    public:
	        MainLayout();
			~MainLayout();
			void selectItem_Click(amiibo::AmiiboFile *menuItem);
			void multiSelectItem_Click(amiibo::AmiiboFile *menuItem, amiibo::AmiiboGame *game);
			void randomizeUuid_Click(amiibo::AmiiboFile *menuItem);
			void category_Click(amiibo::AmiiboGame *game);
			void addGame_Click();
			void removeGame_Click();
			void addAmiibos_Click(amiibo::AmiiboGame *game);
			void GetAmiibos();
			std::vector<amiibo::AmiiboGame*> GetAmiiboGames();
			void GetAllAmiibos();
			void populateGamesMenu();
			pu::element::MenuItem *MainLayout::CreateItem(amiibo::AmiiboGame *game);
			pu::element::Menu *MainLayout::GetGamesMenu();
			pu::element::Menu *MainLayout::GetAmiiboMenu();
	    private:
	        pu::element::Menu *gamesMenu;
	        pu::element::Menu *amiiboMenu;
			std::vector<amiibo::AmiiboGame*> amiiboGames;
			std::vector<amiibo::AmiiboFile*> amiiboFiles;
			std::vector<amiibo::AmiiboFile*> allAmiiboFiles;
			bool waitInput;
			bool isEmuuibo;
			bool isRandomUuid(std::string jsonPath);
			void toggleRandomUuid(std::string jsonPath, bool toggle);
	};
}
