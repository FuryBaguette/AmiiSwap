#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AboutLayout : public pu::ui::Layout
	{
	    public:
	        AboutLayout();
			PU_SMART_CTOR(AboutLayout);
			void about_Input(u64 Down, u64 Up, u64 Held);
		private:
			pu::ui::elm::TextBlock::Ref leftText;
			pu::ui::elm::TextBlock::Ref rightText;
			pu::ui::elm::Rectangle::Ref bground;
	};
}
