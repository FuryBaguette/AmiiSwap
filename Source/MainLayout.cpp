#include "MainApplication.hpp"

extern lang::Language *language;

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

        pu::element::MenuItem *item = new pu::element::MenuItem(lang::GetDictionaryEntry(76));
        item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::manage_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);
        
        item = new pu::element::MenuItem(lang::GetDictionaryEntry(77));
        item->SetIcon(utils::GetRomFsResource("Common/emuiibo.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::emuiibo_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem(lang::GetDictionaryEntry(78));
        item->SetIcon(utils::GetRomFsResource("Common/images.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::images_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem(lang::GetDictionaryEntry(79));
        item->SetIcon(utils::GetRomFsResource("Common/active.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::showSelected_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem(lang::GetDictionaryEntry(80));
        item->SetIcon(utils::GetRomFsResource("Common/gears.png"));
        if(emuiibo) {
            item->AddOnClick(std::bind(&MainLayout::settings_Click, this), KEY_A);
        } else {
            item->AddOnClick(std::bind(&MainLayout::disabled_Click, this), KEY_A);
            item->SetColor({204,0,0,255});
        }
        
        item = new pu::element::MenuItem(lang::GetDictionaryEntry(81));
        item->SetIcon(utils::GetRomFsResource("Common/user-manual.png"));
        item->AddOnClick(std::bind(&MainLayout::manual_Click, this), KEY_A);
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem(lang::GetDictionaryEntry(82));
        item->SetIcon(utils::GetRomFsResource("Common/about.png"));
        item->AddOnClick(std::bind(&MainLayout::about_Click, this), KEY_A);
        this->mainMenu->AddItem(item);
    }

    void MainLayout::manage_Click()
    {
        mainapp->GetGamesLayout()->GetGamesMenu()->SetVisible(true);
        mainapp->GetGamesLayout()->SetElementOnFocus(mainapp->GetGamesLayout()->GetGamesMenu());
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetGamesLayout());
        mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
        mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(6) + lang::GetDictionaryEntry(8) + lang::GetDictionaryEntry(9));
    }

    void MainLayout::emuiibo_Click()
    {
        mainapp->GetEmuiiboLayout()->GetEmuiiboMenu()->SetVisible(true);
        mainapp->GetEmuiiboLayout()->SetElementOnFocus(mainapp->GetEmuiiboLayout()->GetEmuiiboMenu());
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetEmuiiboLayout());
        mainapp->SetFooterText(lang::GetDictionaryEntry(83));
        mainapp->SetHelpText(lang::GetDictionaryEntry(3));
    }

    void MainLayout::images_Click()
    {
        //mainapp->DeleteManageLayout();
        mainapp->GetImagesLayout()->GetImagesMenu()->SetVisible(true);
        mainapp->GetImagesLayout()->SetElementOnFocus(mainapp->GetImagesLayout()->GetImagesMenu());
        this->GetMainMenu()->SetVisible(false);
    	mainapp->LoadLayout(mainapp->GetImagesLayout());
        mainapp->SetFooterText(lang::GetDictionaryEntry(84));
        mainapp->SetHelpText(lang::GetDictionaryEntry(3));
    }

    void MainLayout::settings_Click()
    {
        pu::overlay::Toast *toast = new pu::overlay::Toast("settings", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);
        //mainapp->LoadLayout(mainapp->GetGamesLayout());
    }

    void MainLayout::manual_Click()
    {
        //mainapp->DeleteManageLayout();
        this->mainMenu->SetVisible(false);
        mainapp->LoadLayout(mainapp->GetManualLayout());
        mainapp->GetManualLayout()->LoadFile(utils::GetRomFsResource("Language/manual/" + lang::GetLangCode() + "_manual.txt"));
        mainapp->SetFooterText(lang::GetDictionaryEntry(85));
        mainapp->SetHelpText(lang::GetDictionaryEntry(5));
    }

    void MainLayout::about_Click()
    {
        //mainapp->DeleteManageLayout();
        this->mainMenu->SetVisible(false);
        mainapp->LoadLayout(mainapp->GetAboutLayout());
        mainapp->SetFooterText(lang::GetDictionaryEntry(86) + " (https://discord.gg/ap6yfR2)");
        mainapp->SetHelpText(lang::GetDictionaryEntry(5));
    }

	void MainLayout::showSelected_Click()
	{
        //mainapp->DeleteManageLayout();
        char key[] = { 0 };
		char amiibo[FS_MAX_PATH] = { 0 };
		Result rs = nfpemuGetAmiibo(amiibo);
        if(rs == 0 && R_FAILED(strcmp(key,amiibo))) {
            std::string path = std::string(amiibo);
            mainapp->LoadLayout(mainapp->GetAmiiboDetailsLayout());
            mainapp->SetFooterText(lang::GetDictionaryEntry(87));
            mainapp->SetHelpText(lang::GetDictionaryEntry(5));
            mainapp->GetAmiiboDetailsLayout()->LoadAmiibo(path);
            //std::string amiiboName = utils::getLastFromPath(path);
            //bool random = utils::isRandomUuid(path+"/amiibo.json");
			//mainapp->CreateShowDialog("Active amiibo: " + amiiboName, "Located in: " + path + "\nRandom UUID: " + ((random) ? "enabled":"disabled"), { "Close" }, true, path + "/amiibo.icon");
		} else {
			pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(88), 20, {255,255,255,255}, {0,51,102,255});
			mainapp->StartOverlayWithTimeout(toast, 1500);
		}
	}

	void MainLayout::disabled_Click()
	{
		pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(89), 20, {255,255,255,255}, {204,0,0,255});
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
                mainapp->SetFooterText(lang::GetDictionaryEntry(1));
                break;
            case 1:
                mainapp->SetFooterText(lang::GetDictionaryEntry(83));
                break;
            case 2:
                mainapp->SetFooterText(lang::GetDictionaryEntry(90));
                break;
            case 3:
                mainapp->SetFooterText(lang::GetDictionaryEntry(91));
                break;
            case 4:
                mainapp->SetFooterText(lang::GetDictionaryEntry(85));
                break;
            case 5:
                mainapp->SetFooterText(lang::GetDictionaryEntry(92));
                break;
            default:
                mainapp->SetFooterText("");
                break;    
        }
    }
}
