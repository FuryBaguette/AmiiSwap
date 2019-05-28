#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    MainLayout::MainLayout() : pu::Layout()
    {
        this->mainMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 75, 8);
        this->mainMenu->SetOnFocusColor({102,153,204,255});
        this->mainMenu->SetScrollbarColor({102,153,204,255});
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

        pu::element::MenuItem *item = new pu::element::MenuItem("Amiibos");
        item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        item->AddOnClick(std::bind(&MainLayout::manage_Click, this), KEY_A);
        this->mainMenu->AddItem(item);
        
        item = new pu::element::MenuItem("Emuiibo");
        item->SetIcon(utils::GetRomFsResource("Common/emuiibo.png"));
        item->AddOnClick(std::bind(&MainLayout::emuiibo_Click, this), KEY_A);
        this->mainMenu->AddItem(item);

        item = new pu::element::MenuItem("Settings");
        item->SetIcon(utils::GetRomFsResource("Common/gears.png"));
        item->AddOnClick(std::bind(&MainLayout::settings_Click, this), KEY_A);
        //this->mainMenu->AddItem(item);

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
        mainapp->SetFooterText("Control emuiibo");
        mainapp->SetHelpText("A: Select ");
    }

    void MainLayout::settings_Click()
    {
        pu::overlay::Toast *toast = new pu::overlay::Toast("settings", 20, {255,255,255,255}, {0,0,0,200});
        mainapp->StartOverlayWithTimeout(toast, 1500);
        //mainapp->LoadLayout(mainapp->GetManageLayout());
    }

    void MainLayout::about_Click()
    {
        this->mainMenu->SetVisible(false);
        mainapp->LoadLayout(mainapp->GetAboutLayout());
        mainapp->SetFooterText("Join us on Discord (https://discord.gg/ap6yfR2)");
        mainapp->SetHelpText("B: Back ");
    }

    pu::element::Menu *MainLayout::GetMainMenu()
    {
    	return (this->mainMenu);
    }
}
