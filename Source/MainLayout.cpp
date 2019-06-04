#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    MainLayout::MainLayout() : pu::Layout()
    {
        this->mainMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->mainMenu->SetOnFocusColor({102,153,204,255});
        this->mainMenu->SetScrollbarColor({102,153,204,255});
        this->mainMenu->SetOnSelectionChanged(std::bind(&MainLayout::selectionChange,this));
        this->Add(this->mainMenu);
        this->SetElementOnFocus(this->mainMenu);
        populateMainMenu();
    }

    MainLayout::~MainLayout()
    {
        delete this->mainMenu;
    }

    void MainLayout::populateMainMenu()
    {
        this->mainMenu->ClearItems();
        bool emuiibo = utils::IsEmuiiboPresent();

        pu::element::MenuItem *item = new pu::element::MenuItem("Amiibos");
        item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::manage_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);
        
        item = new pu::element::MenuItem("Emuiibo");
        item->SetIcon(utils::GetRomFsResource("Common/emuiibo.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::emuiibo_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("Images");
        item->SetIcon(utils::GetRomFsResource("Common/images.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::images_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("Selected amiibo");
        item->SetIcon(utils::GetRomFsResource("Common/active.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::showSelected_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("Settings");
        item->SetIcon(utils::GetRomFsResource("Common/gears.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::settings_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        //this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("User Manual");
        item->SetIcon(utils::GetRomFsResource("Common/user-manual.png"));
        item->AddOnClick(std::bind(&MainLayout::manual_Click, this), KEY_A);
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("About");
        item->SetIcon(utils::GetRomFsResource("Common/about.png"));
        item->AddOnClick(std::bind(&MainLayout::about_Click, this), KEY_A);
        this->mainMenu->AddItem(item);
    }

    void MainLayout::manage_Click()
    {
        mainapp->GetManageLayout()->GetGamesMenu()->SetVisible(true);
        mainapp->GetManageLayout()->SetElementOnFocus(mainapp->GetManageLayout()->GetGamesMenu());
        mainapp->GetManageLayout()->GetAmiiboMenu()->SetVisible(false);
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetManageLayout());
        mainapp->SetFooterText("Games: " + std::to_string(mainapp->GetManageLayout()->GetAmiiboGames().size()));
        mainapp->SetHelpText("A: Select  X: Add new game  Y: Manage game's amiibos  Minus: Remove game ");
    }

    void MainLayout::emuiibo_Click()
    {
        mainapp->GetEmuiiboLayout()->GetEmuiiboMenu()->SetVisible(true);
        mainapp->GetEmuiiboLayout()->SetElementOnFocus(mainapp->GetEmuiiboLayout()->GetEmuiiboMenu());
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetEmuiiboLayout());
        mainapp->SetFooterText("Activate amiibo emulation");
        mainapp->SetHelpText("A: OK ");
    }

    void MainLayout::images_Click()
    {
        mainapp->GetImagesLayout()->GetImagesMenu()->SetVisible(true);
        mainapp->GetImagesLayout()->SetElementOnFocus(mainapp->GetImagesLayout()->GetImagesMenu());
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetImagesLayout());
        mainapp->SetFooterText("Search amiibo icons in AmiiSwap and emuiibo folders");
        mainapp->SetHelpText("A: OK ");
    }

    void MainLayout::settings_Click()
    {
        pu::overlay::Toast *toast = new pu::overlay::Toast("settings", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);
        //mainapp->LoadLayout(mainapp->GetManageLayout());
    }

    void MainLayout::manual_Click()
    {
        this->mainMenu->SetVisible(false);
        mainapp->LoadLayout(mainapp->GetManualLayout());
        mainapp->GetManualLayout()->LoadFile(utils::GetRomFsResource("Common/manual.txt"));
        mainapp->SetFooterText("RTFM");
        mainapp->SetHelpText("B: Back ");
    }

    void MainLayout::about_Click()
    {
        this->mainMenu->SetVisible(false);
        mainapp->LoadLayout(mainapp->GetAboutLayout());
        mainapp->SetFooterText("Join us on Discord (https://discord.gg/ap6yfR2)");
        mainapp->SetHelpText("B: Back ");
    }

	void MainLayout::showSelected_Click()
	{
        char key[] = { 0 };
		char amiibo[FS_MAX_PATH] = { 0 };
		Result rs = nfpemuGetAmiibo(amiibo);
        if(rs == 0 && strcmp (key,amiibo) != 0) {
            std::string path = utils::trim_right_copy(std::string(amiibo));
            std::string amiiboName = utils::getLastFromPath(path);
            bool random = utils::isRandomUuid(path+"/amiibo.json");
			mainapp->CreateShowDialog("Active amiibo: " + amiiboName, "Located in: " + path + "\nRandom UUID: " + ((random) ? "enabled":"disabled"), { "Close" }, true, path + "/amiibo.icon");
		} else {
			pu::overlay::Toast *toast = new pu::overlay::Toast("No active amiibo.", 20, {255,255,255,255}, {0,51,102,255});
			mainapp->StartOverlayWithTimeout(toast, 1500);
		}
	}

	void MainLayout::disabled_Click()
	{
		pu::overlay::Toast *toast = new pu::overlay::Toast("Emuiibo is not installed, can't use this feature.", 20, {255,255,255,255}, {204,0,0,255});
		mainapp->StartOverlayWithTimeout(toast, 1500);
	}

    pu::element::Menu *MainLayout::GetMainMenu()
    {
    	return (this->mainMenu);
    }

    void MainLayout::selectionChange()
    {
        switch(this->mainMenu->GetSelectedIndex()){
            case 0:
                mainapp->SetFooterText("Manage and organize your amiibos");
                break;
            case 1:
                mainapp->SetFooterText("Ask emuiibo to toggle amiibo emulation");
                break;
            case 2:
                mainapp->SetFooterText("Manage images for you amiibos");
                break;
            case 3:
                mainapp->SetFooterText("View details about selected amiibo");
                break;
            case 4:
                mainapp->SetFooterText("RTFM");
                break;
            case 5:
                mainapp->SetFooterText("See who brought you AmiiSwap");
                break;
            default:
                mainapp->SetFooterText("");
                break;    
        }
    }
}
