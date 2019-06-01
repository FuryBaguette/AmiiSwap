#pragma once

#include <pu/Plutonium>

namespace ui
{
	class MainLayout : public pu::Layout
	{
	    public:
	        MainLayout();
			~MainLayout();
			void manage_Click();
			void emuiibo_Click();
			void images_Click();
			void settings_Click();
			void about_Click();
			void showSelected_Click();
			void populateMainMenu();
			void selectionChange();
			pu::element::Menu *MainLayout::GetMainMenu();
	    private:
	        pu::element::Menu *mainMenu;
	};
}
