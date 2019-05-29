#pragma once

#include <pu/Plutonium>

namespace ui
{
	class ErrorLayout : public pu::Layout
	{
	    public:
	        ErrorLayout();
			~ErrorLayout();
			void SetText(std::string Text);
			void error_Input(u64 Down, u64 Up, u64 Held);
	    private:
	        pu::element::TextBlock *warningText;
	};
}
