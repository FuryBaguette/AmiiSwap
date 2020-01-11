#include "MainApplication.hpp"

extern lang::Language *language;
extern set::Settings *settings;

namespace ui
{
    extern MainApplication *mainapp;

    AboutLayout::AboutLayout()
    {
        this->bground = pu::ui::elm::Rectangle::New(0, 80, 1280, 600, pu::ui::Color(0,102,153,255));
        this->Add(this->bground);

		std::string team = lang::GetLabel(lang::Label::ABOUT_AUTHOR) + "\nFuryBaguette\n\n" + lang::GetLabel(lang::Label::ABOUT_DEVS) + "\nFuryBaguette\nAD2076\n\n";
		team += lang::GetLabel(lang::Label::ABOUT_TESTERS) + "\ndeurk\nJaded\nMackybean\nSuch Meme, Many Skill\ntommykins";
        this->leftText = pu::ui::elm::TextBlock::New(40, 100, "-", 25);
        leftText->SetColor(pu::ui::Color(255,255,255,255));
        leftText->SetText(team);
        this->Add(this->leftText);

        std::string credits = lang::GetLabel(lang::Label::ABOUT_CREDITS) + "\nXorTroll - For emuiibo\nspx01 - For his fork of switch-nfp-mitm\naverne and ogniK5377 for the fork of fork of fork\nSubv - For making switch-nfp-mitm";
        this->rightText = pu::ui::elm::TextBlock::New(640, 100, "-", 25);
        rightText->SetColor(pu::ui::Color(255,255,255,255));
        rightText->SetText(credits);
        this->Add(this->rightText);

        this->SetOnInput(std::bind(&AboutLayout::about_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
	
	void AboutLayout::about_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B){
			mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT));
			mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
			//mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
			mainapp->GetMainLayout()->selectionChange();
			mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}
}
