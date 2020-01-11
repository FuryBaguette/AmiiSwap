#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    EmuiiboLayout::EmuiiboLayout()
    {
        this->emuiiboMenu = pu::ui::elm::Menu::New(0, 80, 1280, pu::ui::Color(255,255,255,255), 100, 6);
        this->emuiiboMenu->SetOnFocusColor(pu::ui::Color(102,153,204,255));
        this->emuiiboMenu->SetScrollbarColor(pu::ui::Color(102,153,204,255));
        this->emuiiboMenu->SetOnSelectionChanged(std::bind(&EmuiiboLayout::selectionChange,this));
        this->progressBar = pu::ui::elm::ProgressBar::New(640,420,600,20,100.0f);
        this->progressBar->SetColor(pu::ui::Color(255,255,255,255));
        this->progressBar->SetProgressColor(pu::ui::Color(0,102,153,255));
        this->progressBar->SetMaxValue(100);
        this->progressBar->SetVisible(false);
        this->Add(this->emuiiboMenu);
        this->Add(this->progressBar);
        //this->SetElementOnFocus(this->emuiiboMenu);
        populateEmuiiboMenu();

        this->SetOnInput(std::bind(&EmuiiboLayout::emuiibo_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
	
	void EmuiiboLayout::emuiibo_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B){
			mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT));
			mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
			//mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
			mainapp->GetMainLayout()->selectionChange();
			mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}

    void EmuiiboLayout::populateEmuiiboMenu()
    {
        this->emuiiboMenu->ClearItems();

        pu::ui::elm::MenuItem::Ref item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::EMUIIBO_ENABLE));
        item->SetIcon(utils::GetRomFsResource("Common/toggle.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::enable_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);

        item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::EMUIIBO_ENABLE_ONCE));
        item->SetIcon(utils::GetRomFsResource("Common/toggleonce.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::enableonce_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);

        item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::EMUIIBO_DISABLE));
        item->SetIcon(utils::GetRomFsResource("Common/untoggle.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::disable_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);

        item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::EMUIIBO_SCAN));
        item->SetIcon(utils::GetRomFsResource("Common/scan.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::scan_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);
    }

    void EmuiiboLayout::enable_Click()
    {
        nfpemuSetEmulationOff();
        nfpemuSetEmulationOnForever();
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_EMUIIBO_ENABLED), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    void EmuiiboLayout::enableonce_Click()
    {
        nfpemuSetEmulationOnOnce();
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_EMUIIBO_ENABLED_ONCE), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    void EmuiiboLayout::disable_Click()
    {
        nfpemuSetEmulationOff();
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_EMUIIBO_DISABLED), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    void EmuiiboLayout::scan_Click()
    {
        this->progressBar->ClearProgress();
        this->progressBar->SetVisible(true);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->progressBar);
        char amiibo[FS_MAX_PATH] = { 0 };
        bool isOk;
		nfpemuGetCurrentAmiibo(amiibo, &isOk);
        this->progressBar->SetProgress(25.0f);
        mainapp->CallForRender();
        nfpemuRefresh();
        this->progressBar->IncrementProgress(50.0f);
        mainapp->CallForRender();
        set::Initialize();
        this->progressBar->SetProgress(75.0f);
        mainapp->CallForRender();
        nfpemuSetCustomAmiibo(amiibo);
        this->progressBar->SetProgress(100.0f);
        mainapp->CallForRender();
        this->progressBar->SetVisible(false);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->emuiiboMenu);
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_EMUIIBO_SCAN), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    pu::ui::elm::Menu::Ref EmuiiboLayout::GetEmuiiboMenu()
    {
        return (this->emuiiboMenu);
    }

    void EmuiiboLayout::selectionChange()
    {
        switch(this->emuiiboMenu->GetSelectedIndex()){
            case 0:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_EMUIIBO_ENABLE));
                break;
            case 1:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_EMUIIBO_ENABLE_ONCE));
                break;
            case 2:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_EMUIIBO_DISABLE));
                break;
            case 3:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_EMUIIBO_SCAN));
                break;
            default:
                mainapp->SetFooterText("");
                break;
        }
    }
}
