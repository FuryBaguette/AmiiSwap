#pragma once

#include <pu/Plutonium>

namespace ui
{
	class BootLayout : public pu::Layout
	{
	    public:
	        BootLayout();
			~BootLayout();
			void SetText(std::string Text);	
			void SetProgress(float Progress);
	    private:
	        pu::element::TextBlock *bootText;
			pu::element::Rectangle *bground;
			pu::element::ProgressBar *progressBar;
	};
}
