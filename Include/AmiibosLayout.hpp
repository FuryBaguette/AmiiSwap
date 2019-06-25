#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AmiibosLayout : public pu::Layout
	{
	    public:
	        AmiibosLayout();
			~AmiibosLayout();
			void manage_Input(u64 Down, u64 Up, u64 Held);
			void selectItem_Click(std::string amiibo);
			void randomizeUuid_Click(std::string amiibo);
			//void GetAllAmiibos();
			void populateAmiibosMenu(amiibo::Game *game);
			pu::element::MenuItem *AmiibosLayout::CreateItem(std::string amiibo);
			pu::element::Menu *AmiibosLayout::GetAmiibosMenu();
	    private:
			pu::element::Menu *amiibosMenu;
			void toggleRandomUuid(std::string jsonPath, bool toggle);
			//void createAmiibosMenu();
	};
}
