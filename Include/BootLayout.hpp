#pragma once

#include <pu/Plutonium>

namespace ui
{
	class BootLayout : public pu::ui::Layout
	{
	    public:
	        BootLayout();
			PU_SMART_CTOR(BootLayout);
			void SetText(std::string Text);	
			void SetProgress(float Progress);
	    private:
	        pu::ui::elm::TextBlock::Ref bootText;
			pu::ui::elm::Rectangle::Ref bground;
			pu::ui::elm::ProgressBar::Ref progressBar;
	};
}
