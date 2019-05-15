#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    ErrorLayout::ErrorLayout()
    {
        this->warningText = new pu::element::TextBlock(0, 80, "", 30);
        this->warningText->SetColor({255,0,0,255});
        this->Add(this->warningText);
    }

    void ErrorLayout::SetText(std::string Text)
    {
        this->warningText->SetText(Text);
    }
}
