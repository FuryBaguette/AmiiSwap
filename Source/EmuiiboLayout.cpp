#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    EmuiiboLayout::EmuiiboLayout() : pu::Layout()
    {
        this->emuiiboMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->emuiiboMenu->SetOnFocusColor({102,153,204,255});
        this->emuiiboMenu->SetScrollbarColor({102,153,204,255});
        this->Add(this->emuiiboMenu);
        this->SetElementOnFocus(this->emuiiboMenu);
        populateEmuiiboMenu();

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
        {
            if (Down & KEY_B){
                mainapp->SetFooterText("");
                mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
                mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
                mainapp->LoadLayout(mainapp->GetMainLayout());
            }
        });
    }

    EmuiiboLayout::~EmuiiboLayout()
    {
        delete this->emuiiboMenu;
    }

    void EmuiiboLayout::populateEmuiiboMenu()
    {
        this->emuiiboMenu->ClearItems();

        pu::element::MenuItem *item = new pu::element::MenuItem("Enable");
        item->SetIcon(utils::GetRomFsResource("Common/toggle.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::enable_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);
        
        item = new pu::element::MenuItem("Enable once");
        item->SetIcon(utils::GetRomFsResource("Common/toggleonce.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::enableonce_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);

        item = new pu::element::MenuItem("Disable");
        item->SetIcon(utils::GetRomFsResource("Common/untoggle.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::disable_Click, this), KEY_A);
        this->emuiiboMenu->AddItem(item);
    }

    void EmuiiboLayout::enable_Click()
    {
        nfpemuUntoggle();
        nfpemuToggle();
        pu::overlay::Toast *toast = new pu::overlay::Toast("Emuiibo enabled", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);     
    }

    void EmuiiboLayout::enableonce_Click()
    {
        nfpemuToggleOnce();
        pu::overlay::Toast *toast = new pu::overlay::Toast("Emuiibo enabled once", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);      
    }
    
    void EmuiiboLayout::disable_Click()
    {
        nfpemuUntoggle();
        pu::overlay::Toast *toast = new pu::overlay::Toast("Emuiibo disabled", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    pu::element::Menu *EmuiiboLayout::GetEmuiiboMenu()
    {
        return (this->emuiiboMenu);
    }
}
