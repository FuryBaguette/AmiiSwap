#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    ErrorLayout::ErrorLayout() : pu::Layout()
    {
        this->warningText = new pu::element::TextBlock(0, 80, "", 30);
        this->warningText->SetColor({255,0,0,255});
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
