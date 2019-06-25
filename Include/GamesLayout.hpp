#pragma once

#include <pu/Plutonium>

namespace ui
{
	class GamesLayout : public pu::Layout
	{
	    public:
	        GamesLayout();
			~GamesLayout();
			void manage_Input(u64 Down, u64 Up, u64 Held);
			void multiSelectItem_Click(amiibo::Amiibo *menuItem, amiibo::Game *game);
			void category_Click(amiibo::Game *game);
			void addGame_Click();
			void removeGame_Click();
			void addAmiibos_Click(amiibo::Game *game);
			void GamesLayout::backToGame_Click();
			void populateGamesMenu();
			pu::element::MenuItem *GamesLayout::CreateItem(amiibo::Game *game);
			pu::element::Menu *GamesLayout::GetGamesMenu();
			pu::element::Menu *GamesLayout::GetAllAmiibosMenu();
	    private:
			pu::element::Menu *gamesMenu;
	        pu::element::Menu *allAmiibosMenu;
			bool waitInput = false;
			bool isEmuuibo;
	};
}
