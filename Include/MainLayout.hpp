#pragma once

#include <pu/Plutonium>

namespace ui
{
	class MainLayout : public pu::Layout
	{
	    public:
	        MainLayout();
			void item_Click(amiibo::AmiiboFile *menuItem);
			void category_Click(amiibo::AmiiboGame *game);
			void GetFolders();
			std::vector<amiibo::AmiiboFile*> GetEmuiibo();
			pu::element::Menu *MainLayout::GetGamesMenu();
			pu::element::Menu *MainLayout::GetAmiiboMenu();
	    private:
	        pu::element::TextBlock *titleText;
			pu::element::TextBlock *warningText;
	        pu::element::Menu *gamesMenu;
	        pu::element::Menu *amiiboMenu;
			std::vector<amiibo::AmiiboGame*> amiiboGames;
			std::vector<amiibo::AmiiboFile*> files;
			std::vector<pu::element::MenuItem*> gameItems;
			bool waitInput;
			bool isEmuuibo;
	};
}
