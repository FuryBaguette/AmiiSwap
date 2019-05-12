#pragma once

#include <pu/Plutonium>

namespace ui
{
	class SettingsLayout : public pu::Layout
	{
	    public:
	        SettingsLayout();
			void GetAmiibos();
			void GetAmiiboGames();
			void amiibo_Click(amiibo::AmiiboFile *game);
	    private:
	        pu::element::TextBlock *titleText;
	        pu::element::Menu *amiiboMenu;
			std::vector<amiibo::AmiiboFile*> amiiboFiles;
			std::vector<amiibo::AmiiboGame*> amiiboGames;
			bool waitInput;
	};
}
