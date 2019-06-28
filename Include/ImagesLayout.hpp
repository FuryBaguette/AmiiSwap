#pragma once

#include <pu/Plutonium>

namespace ui
{
	class ImagesLayout : public pu::Layout
	{
	    public:
	        ImagesLayout();
			~ImagesLayout();
			void search_Click();
			void rename_Click();
			void remove_Click();
			void populateImagesMenu();
			void selectionChange();
			pu::element::Menu *ImagesLayout::GetImagesMenu();
	    private:
	        pu::element::Menu *imagesMenu;
			pu::element::ProgressBar *progressBar;
	};
}
