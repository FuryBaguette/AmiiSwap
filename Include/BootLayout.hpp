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
	    private:
	        pu::element::TextBlock *bootText;
			pu::element::Rectangle *bground;
	};
}
