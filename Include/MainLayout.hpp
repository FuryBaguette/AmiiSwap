#pragma once

#include <pu/Plutonium>

namespace ui
{
	class MainLayout : public pu::ui::Layout
	{
	    public:
	        MainLayout();
			PU_SMART_CTOR(MainLayout);
			void manage_Click();
			void emuiibo_Click();
			void images_Click();
			void settings_Click();
			void manual_Click();
			void about_Click();
			void showSelected_Click();
			void disabled_Click();
			void populateMainMenu();
			void selectionChange();
			pu::ui::elm::Menu::Ref MainLayout::GetMainMenu();
	    private:
	        pu::ui::elm::Menu::Ref mainMenu;
	};
}
