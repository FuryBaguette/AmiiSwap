#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    void MainLayout::GetAmiibos()
    {
        set::Settings *settings = new set::Settings("sdmc:/switch/AmiiSwap/settings.txt");
        this->amiiboGames = settings->GetGames();
    }

    MainLayout::MainLayout()
    {
    	if (!utils::IsEmuiiboPresent()) {
            this->warningText = new pu::element::TextBlock(0, 0, "Emuiibo is not running on this console, please install it before using AmiiSwap");
			this->Add(this->warningText);
        } else {
			nfpemuInitialize();
        	utils::EnsureDirectories();
    		this->GetAmiibos();

    		this->gamesMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
    	    this->amiiboMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
            this->titleText = new pu::element::TextBlock(640, 10, "AmiiSwap");

    		for (auto & element : this->amiiboGames) {
    			pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
                item->SetIcon("sdmc:/switch/AmiiSwap/game.png");
    			item->AddOnClick(std::bind(&MainLayout::category_Click, this, element), KEY_A);
    			this->gamesMenu->AddItem(item);
    		}

            this->gamesMenu->SetOnFocusColor({102,153,204,255});
            this->amiiboMenu->SetOnFocusColor({102,153,204,255});
			this->gamesMenu->SetScrollbarColor({102,153,204,255});
            this->amiiboMenu->SetScrollbarColor({102,153,204,255});
            this->amiiboMenu->SetVisible(false);
            this->titleText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);

            this->Add(this->titleText);
    	    this->Add(this->gamesMenu);
    		this->Add(this->amiiboMenu);
    		this->SetElementOnFocus(this->gamesMenu);
        }
    }

    void MainLayout::category_Click(amiibo::AmiiboGame *game)
    {
    	this->waitInput = true;
    	this->amiiboMenu->ClearItems();
    	std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
    	for (auto & element : files) {
    		pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
            item->SetIcon(element->GetIconPath());
    		item->AddOnClick(std::bind(&MainLayout::item_Click, this, element), KEY_A);
    		this->amiiboMenu->AddItem(item);
    	}

    	this->amiiboMenu->SetSelectedIndex(0);
    	this->SetElementOnFocus(this->amiiboMenu);
    	this->amiiboMenu->SetVisible(true);
    	this->gamesMenu->SetVisible(false);
    }

    void MainLayout::item_Click(amiibo::AmiiboFile *element)
    {
    	if (!this->waitInput) {
    		mainapp->SetWaitBack(true);
    		int sopt = mainapp->CreateShowDialog("Use " + element->GetName() + " ?", "This will set the current Amiibo to " + element->GetName(), { "Yes", "No" }, true, element->GetIconPath());
    		if (sopt == 0) {
                nfpemuSetAmiibo(element->GetPath().c_str());
                pu::overlay::Toast *toast = new pu::overlay::Toast("Active amiibo updated to: " + element->GetName(), 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
    		mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }

    pu::element::Menu *MainLayout::GetGamesMenu()
    {
    	return (this->gamesMenu);
    }

    pu::element::Menu *MainLayout::GetAmiiboMenu()
    {
    	return (this->amiiboMenu);
    }
}
