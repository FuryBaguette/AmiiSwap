#pragma once

#include <pu/Plutonium>

namespace ui
{
	class ImagesLayout : public pu::ui::Layout
	{
	    public:
	        ImagesLayout();
			PU_SMART_CTOR(ImagesLayout);
			void search_Click();
			void rename_Click();
			void remove_Click();
			void populateImagesMenu();
			void selectionChange();
			pu::ui::elm::Menu::Ref ImagesLayout::GetImagesMenu();
			void images_Input(u64 Down, u64 Up, u64 Held);
	    private:
	        pu::ui::elm::Menu::Ref imagesMenu;
			pu::ui::elm::ProgressBar::Ref progressBar;
	};
}
