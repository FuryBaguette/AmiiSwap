#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    void MainLayout::GetFolders()
    {
    	std::vector<std::string> gameFolders = utils::get_directories("sdmc:/emuiibo/");
        if (gameFolders.empty())
            mainapp->Close();
    	for (auto & element : gameFolders) {
    		std::size_t found = element.find_last_of("/\\");
    		std::string namePath = element.substr(found+1);

    		amiibo::AmiiboGame *game = new amiibo::AmiiboGame(namePath, "sdmc:/emuiibo/" + element + "/");

    		std::vector<amiibo::AmiiboFile*> amiiboFiles;

    		utils::getFiles(game->GetPath(), [&game](const std::string &path) {
    			std::size_t found1 = path.find_last_of("/\\");
    			std::string namePath1 = path.substr(found1+1);
                std::string iconPath = path.substr(0, path.size() - 4) + ".png";
    			namePath1.erase(namePath1.size() - 4);
    			amiibo::AmiiboFile *file = new amiibo::AmiiboFile(namePath1, path, iconPath);

    			game->AddAmiiboFile(file);
    		});

    		this->amiiboGames.push_back(game);
    	}
    }

    MainLayout::MainLayout()
    {
    	if (!utils::IsEmuiiboPresent()) {
            this->warningText = new pu::element::TextBlock(0, 0, "Emuiibo is not running on this console, please install it before using AmiiSwap");
			this->Add(this->warningText);
        } else {
			nfpemuInitialize();
        	utils::EnsureDirectories();
    		this->GetFolders();

    		this->gamesMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
    	    this->amiiboMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
            this->titleText = new pu::element::TextBlock(640, 10, "AmiiSwap");

    		for (auto & element : this->amiiboGames) {
    			pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
                item->SetIcon("sdmc:/switch/AmiiSwap/game.png");
    			item->AddOnClick(std::bind(&MainLayout::category_Click, this, element), KEY_A);
    			this->gamesMenu->AddItem(item);
    		}

            gamesMenu->SetOnFocusColor({102,153,204,255});
            amiiboMenu->SetOnFocusColor({102,153,204,255});
			gamesMenu->SetScrollbarColor({102,153,204,255});
            amiiboMenu->SetScrollbarColor({102,153,204,255});
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
    	if (!waitInput) {
    		mainapp->SetWaitBack(true);
    		int sopt = mainapp->CreateShowDialog("Use " + element->GetName() + " ?", "This will set the current Amiibo to " + element->GetName(), { "Yes", "No" }, true, element->GetIconPath());
    		if (sopt == 0) {
                nfpemuRequestUseCustomAmiibo(element->GetPath().c_str());
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
