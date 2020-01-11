#pragma once

#include <pu/Plutonium>

namespace ui
{
	class GamesLayout : public pu::ui::Layout
	{
	    public:
	        GamesLayout();
			PU_SMART_CTOR(GamesLayout);
			void manage_Input(u64 Down, u64 Up, u64 Held);
			void multiSelectItem_Click(amiibo::Amiibo *menuItem, amiibo::Game *game);
			void category_Click(amiibo::Game *game);
			void addGame_Click();
			void removeGame_Click();
			void addAmiibos_Click(amiibo::Game *game);
			void backToGame_Click();
			void populateGamesMenu();
			void CreateItem(amiibo::Game *game);
			pu::ui::elm::Menu::Ref GetGamesMenu();
			pu::ui::elm::Menu::Ref GetAllAmiibosMenu();
	    private:
			pu::ui::elm::Menu::Ref gamesMenu;
	        pu::ui::elm::Menu::Ref allAmiibosMenu;
			bool waitInput = false;
			bool isEmuuibo;
	};
}
