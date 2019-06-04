#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    BootLayout::BootLayout() : pu::Layout()
    {
        this->bground = new pu::element::Rectangle(0, 0, 1280, 720, {0,102,153,255});
        this->Add(this->bground);
        this->bootText = new pu::element::TextBlock(640, 360, "Loading AmiiSwap...", 40);
        this->bootText->SetColor({255,255,255,255});
        this->bootText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
        this->Add(this->bootText);
        //this->SetOnInput(std::bind(&ErrorLayout::error_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    BootLayout::~BootLayout()
    {
        delete this->bootText;
        delete this->bground;
    }

	void BootLayout::SetText(std::string Text)
    {
        this->bootText->SetText(Text);
    }
}
