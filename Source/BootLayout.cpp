#include "MainApplication.hpp"

extern lang::Language language;

namespace ui
{
    extern MainApplication *mainapp;

    BootLayout::BootLayout()
    {
        this->bground = pu::ui::elm::Rectangle::New(0, 0, 1280, 720, pu::ui::Color(0,102,153,255));
        this->Add(this->bground);
		//std::string lab = lang::GetLabel(lang::Label::BOOT_LOADING);
        this->bootText = pu::ui::elm::TextBlock::New(640, 360, utils::replace(lang::GetLabel(lang::Label::BOOT_LOADING),"{{TEST}}","FUNZIONA"), 40);
        this->bootText->SetColor(pu::ui::Color(255,255,255,255));
        this->bootText->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Center);
        this->Add(this->bootText);
        this->progressBar = pu::ui::elm::ProgressBar::New(320,420,640,10,100.0f);
        this->progressBar->SetColor(pu::ui::Color(0,51,102,255));
        this->progressBar->SetProgressColor(pu::ui::Color(255,255,255,255));
        this->progressBar->SetVisible(true);
        this->Add(this->progressBar);
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
