#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    ErrorLayout::ErrorLayout() : pu::Layout()
    {
        this->warningText = new pu::element::TextBlock(40, 100, "", 30);
        this->warningText->SetColor({204,0,0,255});
        this->warningText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
        this->Add(this->warningText);
    }

    ErrorLayout::~ErrorLayout()
    {
        delete this->warningText;
    }

    void ErrorLayout::SetText(std::string Text)
    {
        this->warningText->SetText(Text);
    }
}
