#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AmiibosLayout : public pu::ui::Layout
	{
	    public:
	        AmiibosLayout();
			PU_SMART_CTOR(AmiibosLayout);
			void manage_Input(u64 Down, u64 Up, u64 Held);
			void selectItem_Click(std::string amiibo);
			void randomizeUuid_Click(std::string amiibo);
			void populateAmiibosMenu(amiibo::Game *game);
			pu::ui::elm::MenuItem::Ref &AmiibosLayout::CreateItem(std::string amiibo);
			pu::ui::elm::Menu::Ref AmiibosLayout::GetAmiibosMenu();
	    private:
			pu::ui::elm::Menu::Ref amiibosMenu;
			void toggleRandomUuid(std::string jsonPath, bool toggle);
	};
}
