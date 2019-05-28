#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AboutLayout : public pu::Layout
	{
	    public:
	        AboutLayout();
			~AboutLayout();
		private:
			pu::element::TextBlock *leftText;
			pu::element::TextBlock *rightText;
			pu::element::Rectangle *bground;
	};
}
