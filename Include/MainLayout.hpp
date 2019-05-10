#pragma once

#include <pu/Plutonium>

namespace ui
{
	class MainLayout : public pu::Layout
	{
	    public:
	        MainLayout();
			void selectItem_Click(amiibo::AmiiboFile *menuItem);
			void randomizeUuid_Click(amiibo::AmiiboFile *menuItem);
			void category_Click(amiibo::AmiiboGame *game);
			void GetAmiibos();
			pu::element::Menu *MainLayout::GetGamesMenu();
			pu::element::Menu *MainLayout::GetAmiiboMenu();
	    private:
	        pu::element::TextBlock *titleText;
			pu::element::TextBlock *warningText;
	        pu::element::Menu *gamesMenu;
	        pu::element::Menu *amiiboMenu;
			std::vector<amiibo::AmiiboGame*> amiiboGames;
			bool waitInput;
			bool isEmuuibo;
	};
}
