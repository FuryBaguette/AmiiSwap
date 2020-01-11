#pragma once

#include <pu/Plutonium>

namespace ui
{
	class ErrorLayout : public pu::ui::Layout
	{
	    public:
	        ErrorLayout();
			PU_SMART_CTOR(ErrorLayout);
			void SetText(std::string Text);
			void error_Input(u64 Down, u64 Up, u64 Held);
	    private:
	        pu::ui::elm::TextBlock::Ref warningText;
	};
}
