#pragma once

#include <pu/Plutonium>

namespace ui
{
	class EmuiiboLayout : public pu::ui::Layout
	{
	    public:
	        EmuiiboLayout();
			PU_SMART_CTOR(EmuiiboLayout);
			void enable_Click();
			void enableonce_Click();
			void disable_Click();
			void scan_Click();
			void populateEmuiiboMenu();
			void selectionChange();
			pu::ui::elm::Menu::Ref EmuiiboLayout::GetEmuiiboMenu();
			void emuiibo_Input(u64 Down, u64 Up, u64 Held);
	    private:
	        pu::ui::elm::Menu::Ref emuiiboMenu;
			pu::ui::elm::ProgressBar::Ref progressBar;
	};
}
