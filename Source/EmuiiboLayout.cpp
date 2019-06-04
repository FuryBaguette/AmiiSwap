#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    EmuiiboLayout::EmuiiboLayout() : pu::Layout()
    {
        this->emuiiboMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->emuiiboMenu->SetOnFocusColor({102,153,204,255});
        this->emuiiboMenu->SetScrollbarColor({102,153,204,255});
        this->emuiiboMenu->SetOnSelectionChanged(std::bind(&EmuiiboLayout::selectionChange,this));
        this->progressBar = new pu::element::ProgressBar(640,420,600,20,100.0f);
        this->progressBar->SetColor({255,255,255,255});
        this->progressBar->SetProgressColor({0,102,153,255});
        this->progressBar->SetMaxValue(100);
        this->progressBar->SetVisible(false);
        this->Add(this->emuiiboMenu);
        this->Add(this->progressBar);
        this->SetElementOnFocus(this->emuiiboMenu);
        populateEmuiiboMenu();

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
        {
            if (Down & KEY_B){
                mainapp->SetHelpText("A: Select ");
                mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
                mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
                mainapp->GetMainLayout()->selectionChange();
                mainapp->LoadLayout(mainapp->GetMainLayout());
            }
        });
    }

    EmuiiboLayout::~EmuiiboLayout()
    {
        delete this->emuiiboMenu;
        delete this->progressBar;
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

        item = new pu::element::MenuItem("Scan for new Amiibos");
        item->SetIcon(utils::GetRomFsResource("Common/scan.png"));
        item->AddOnClick(std::bind(&EmuiiboLayout::scan_Click, this), KEY_A);
        //this->emuiiboMenu->AddItem(item);  //UNCOMMENT WHEN EMUIIBO WILL MERGE PR AND RELEASE
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
    
    void EmuiiboLayout::scan_Click()
    {
        this->progressBar->ClearProgress();
        this->progressBar->SetVisible(true);
        mainapp->CallForRender();
        this->SetElementOnFocus(this->progressBar);
        char amiibo[FS_MAX_PATH] = { 0 };
		nfpemuGetAmiibo(amiibo);
        this->progressBar->SetProgress(25.0f);
        mainapp->CallForRender();
        //nfpemuRescanAmiibos(); //UNCOMMENT WHEN EMUIIBO WILL MERGE PR AND RELEASE
        this->progressBar->IncrementProgress(50.0f);
        mainapp->CallForRender();
        nfpemuSetAmiibo(amiibo);
        this->progressBar->SetProgress(75.0f);
        mainapp->CallForRender();
        mainapp->UpdateSettings();
        this->progressBar->SetProgress(100.0f);
        mainapp->CallForRender();
        this->progressBar->SetVisible(false);
        mainapp->CallForRender();
        this->SetElementOnFocus(this->emuiiboMenu);
        pu::overlay::Toast *toast = new pu::overlay::Toast("Scan complete", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }

    pu::element::Menu *EmuiiboLayout::GetEmuiiboMenu()
    {
        return (this->emuiiboMenu);
    }
    
    void EmuiiboLayout::selectionChange()
    {
        switch(this->emuiiboMenu->GetSelectedIndex()){
            case 0:
                mainapp->SetFooterText("Activate amiibo emulation");
                break;
            case 1:
                mainapp->SetFooterText("Activate amiibo emulation once");
                break;
            case 2:
                mainapp->SetFooterText("Deactivate amiibo emulation");
                break;
            case 3:
                mainapp->SetFooterText("Scan emuiibo folder for new .bin");
                break;
            default:
                mainapp->SetFooterText("");
                break;    
        }
    }
}
