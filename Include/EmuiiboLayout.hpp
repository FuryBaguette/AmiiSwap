#pragma once

#include <pu/Plutonium>

namespace ui
{
	class EmuiiboLayout : public pu::Layout
	{
	    public:
	        EmuiiboLayout();
			~EmuiiboLayout();
			void enable_Click();
			void enableonce_Click();
			void disable_Click();
			void scan_Click();
			void populateEmuiiboMenu();
			void selectionChange();
			pu::element::Menu *EmuiiboLayout::GetEmuiiboMenu();
	    private:
	        pu::element::Menu *emuiiboMenu;
			pu::element::ProgressBar *progressBar;
	};
}
