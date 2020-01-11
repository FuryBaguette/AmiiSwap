#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    ErrorLayout::ErrorLayout()
    {
        this->warningText = pu::ui::elm::TextBlock::New(40, 100, "", 30);
        this->warningText->SetColor(pu::ui::Color(204,0,0,255));
        this->warningText->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Center);
        this->Add(this->warningText);
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
