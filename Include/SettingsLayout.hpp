#pragma once

#include <pu/Plutonium>

namespace ui
{
	class SettingsLayout : public pu::Layout
	{
	    public:
	        SettingsLayout();
			void GetAmiibos();
			void amiibo_Click(amiibo::AmiiboFile *game);
	    private:
	        pu::element::TextBlock *titleText;
	        pu::element::Menu *amiiboMenu;
			std::vector<amiibo::AmiiboFile*> amiiboFiles;
			bool waitInput;
	};
}
