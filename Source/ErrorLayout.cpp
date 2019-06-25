#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    ErrorLayout::ErrorLayout() : pu::Layout()
    {
        this->warningText = new pu::element::TextBlock(40, 100, "", 30);
        this->warningText->SetColor({204,0,0,255});
        this->warningText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
        this->Add(this->warningText);
        //this->SetOnInput(std::bind(&ErrorLayout::error_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    ErrorLayout::~ErrorLayout()
    {
        delete this->warningText;
    }

	void ErrorLayout::error_Input(u64 Down, u64 Up, u64 Held)
	{
		if(Down & KEY_PLUS) mainapp->Close();
	}

    void ErrorLayout::SetText(std::string Text)
    {
        this->warningText->SetText(Text);
    }
}
