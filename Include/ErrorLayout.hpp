#pragma once

#include <pu/Plutonium>

namespace ui
{
	class ErrorLayout : public pu::Layout
	{
	    public:
	        ErrorLayout();
			void SetText(std::string Text);
	    private:
	        pu::element::TextBlock *warningText;
	};
}
