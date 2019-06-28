#include "MainApplication.hpp"

extern lang::Language language;

namespace ui
{
    extern MainApplication *mainapp;

    BootLayout::BootLayout() : pu::Layout()
    {
        this->bground = new pu::element::Rectangle(0, 0, 1280, 720, {0,102,153,255});
        this->Add(this->bground);
        this->bootText = new pu::element::TextBlock(640, 360, lang::GetLabel(lang::Label::BOOT_LOADING), 40);
        this->bootText->SetColor({255,255,255,255});
        this->bootText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
        this->Add(this->bootText);
        this->progressBar = new pu::element::ProgressBar(320,420,640,10,100.0f);
        this->progressBar->SetColor({0,51,102,255});
        this->progressBar->SetProgressColor({255,255,255,255});
        this->progressBar->SetVisible(true);
        this->Add(this->progressBar);
    }

    BootLayout::~BootLayout()
    {
        delete this->bootText;
        delete this->bground;
        delete this->progressBar;
    }

	void BootLayout::SetText(std::string Text)
    {
        this->bootText->SetText(Text);
    }

	void BootLayout::SetProgress(float Progress)
    {
        this->progressBar->SetProgress(Progress);
    }
}
