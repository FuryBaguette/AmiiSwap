#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    ManageLayout::ManageLayout() : pu::Layout()
    {
        this->gamesMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->amiiboMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 60, 10);
        this->gamesMenu->SetOnFocusColor({102,153,204,255});
        this->amiiboMenu->SetOnFocusColor({102,153,204,255});
        this->gamesMenu->SetScrollbarColor({102,153,204,255});
        this->amiiboMenu->SetScrollbarColor({102,153,204,255});
        this->amiiboMenu->SetVisible(false);
        this->Add(this->gamesMenu);
        this->Add(this->amiiboMenu);
        this->SetElementOnFocus(this->gamesMenu);
        populateGamesMenu();
        this->SetOnInput(std::bind(&ManageLayout::manage_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

	void ManageLayout::manage_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B && !mainapp->GetWaitBack()){
			if(mainapp->GetManageLayout()->GetAmiiboMenu()->IsVisible()){
				mainapp->GetManageLayout()->backToGame_Click();
			} else {
				mainapp->SetFooterText("");
				mainapp->SetHelpText("A: Select ");
				mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
				mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
				mainapp->LoadLayout(mainapp->GetMainLayout());
			}
		}
	}

    ManageLayout::~ManageLayout()
    {
        delete this->gamesMenu;
        delete this->amiiboMenu;
    }

    std::vector<amiibo::AmiiboGame*> ManageLayout::GetAmiiboGames()
    {
        return (this->amiiboGames);
    }

    void ManageLayout::GetAmiibos()
    {
        set::Settings *settings = new set::Settings("sdmc:/switch/AmiiSwap/settings.txt");
        this->amiiboGames = settings->GetGames();
    }

    void ManageLayout::populateGamesMenu()
    {
        this->gamesMenu->ClearItems();
        this->GetAmiibos();
        for (auto & element : this->amiiboGames) {
            this->gamesMenu->AddItem(CreateItem(element));
        }
    }

    pu::element::MenuItem *ManageLayout::CreateItem(amiibo::AmiiboGame *element)
    {
        pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
        std::string iconFile = element->GetName() + ".png";
        std::string amiiswapFolder ="sdmc:/switch/AmiiSwap/";
        if(std::fstream{amiiswapFolder+iconFile}){
            item->SetIcon(amiiswapFolder+iconFile);
        }else{
            item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        }
        item->AddOnClick(std::bind(&ManageLayout::category_Click, this, element), KEY_A);
        item->AddOnClick(std::bind(&ManageLayout::addGame_Click, this), KEY_X);
        item->AddOnClick(std::bind(&ManageLayout::addAmiibos_Click, this, element), KEY_Y);
        item->AddOnClick(std::bind(&ManageLayout::removeGame_Click, this), KEY_MINUS);
        return item;
    }

    void ManageLayout::category_Click(amiibo::AmiiboGame *game)
    {
    	this->waitInput = true;
    	this->amiiboMenu->ClearItems();
    	this->amiiboFiles = game->GetBinFiles();
        if (amiiboFiles.empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("List is empty", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        } else {
        	for (auto & element : amiiboFiles) {
                std::string amiiboName = element->GetName();
                size_t size = amiiboName.find("/");
                if (size != std::string::npos)
                    amiiboName = amiiboName.substr(size + 1);
                pu::element::MenuItem *item = new pu::element::MenuItem(amiiboName);
                item->SetIcon(element->GetIconPath());
                item->AddOnClick(std::bind(&ManageLayout::selectItem_Click, this, element), KEY_A);
                item->AddOnClick(std::bind(&ManageLayout::randomizeUuid_Click, this, element), KEY_X);
                item->AddOnClick(std::bind(&ManageLayout::backToGame_Click, this), KEY_B);
                this->amiiboMenu->AddItem(item);
        	}
            mainapp->SetFooterText("Amiibos in " + game->GetName() +": " + std::to_string(amiiboFiles.size()));
            mainapp->SetHelpText("A: Select  X: Toggle RandomUUID ");
        	this->amiiboMenu->SetSelectedIndex(0);
        	this->SetElementOnFocus(this->amiiboMenu);
        	this->amiiboMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }

    void ManageLayout::selectItem_Click(amiibo::AmiiboFile *element)
    {
    	if (!this->waitInput) {
    		mainapp->SetWaitBack(true);
            std::string amiiboName = element->GetName();
            size_t size = amiiboName.find("/");
            if (size != std::string::npos)
                amiiboName = amiiboName.substr(size + 1);
            int sopt = mainapp->CreateShowDialog("Use " + amiiboName + " ?", "This will set the current Amiibo to " + amiiboName, { "Yes", "No" }, true, element->GetIconPath());
    		if (sopt == 0) {
                nfpemuSetAmiibo(element->GetPath().c_str());
                pu::overlay::Toast *toast = new pu::overlay::Toast("Active amiibo updated to: " + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
    		mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }

    void ManageLayout::randomizeUuid_Click(amiibo::AmiiboFile *element)
    {
    	if (!this->waitInput) {
    		mainapp->SetWaitBack(true);
            std::string amiiboName = element->GetName();
            std::string jsonPath = "sdmc:/emuiibo/" + amiiboName + "/amiibo.json";
            size_t size = amiiboName.find("/");
            if (size != std::string::npos)
                amiiboName = amiiboName.substr(size + 1);
            bool randomStatus = utils::isRandomUuid(jsonPath);
            int sopt = mainapp->CreateShowDialog("Toggle Randomize UUID?", "Randomize UUID for " + amiiboName + " is actually " + (randomStatus ? "enabled":"disabled"), { "Toggle", "Cancel" }, true, element->GetIconPath());
            if(sopt == 0){
                toggleRandomUuid(jsonPath, !randomStatus);
                pu::overlay::Toast *toast = new pu::overlay::Toast("Random UUID for " + amiiboName + ((!randomStatus) ? " enabled." : " disabled"), 20, {255,255,255,255}, {0,51,102,255});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
            mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }

    void ManageLayout::toggleRandomUuid(std::string jsonPath, bool toggle)
    {
        std::ifstream ifs(jsonPath);
        auto amiiboJson = json::parse(ifs);
        if(ifs.is_open())
            ifs.close();
        amiiboJson["randomizeUuid"] = toggle;
        std::ofstream ofs(jsonPath);
        ofs << amiiboJson.dump(4);
        if(ofs.is_open())
            ofs.close();
    }

    pu::element::Menu *ManageLayout::GetGamesMenu()
    {
        mainapp->SetFooterText("Games: " + std::to_string(this->amiiboGames.size()));
        mainapp->SetHelpText("A: Select  X: Add new game  Y: Manage game's amiibos  Minus: Remove game ");
    	return (this->gamesMenu);
    }

    pu::element::Menu *ManageLayout::GetAmiiboMenu()
    {
    	return (this->amiiboMenu);
    }

    void ManageLayout::addGame_Click()
    {
        std::string name = utils::UserInput("Game Name", "");
        std::transform(name.begin(), name.end(), name.begin(), utils::ClearForbidden);
       
        if(name == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL name is reserved", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }

        if(name != ""){
            amiibo::AmiiboGame *game = new amiibo::AmiiboGame(name);
            this->amiiboGames.push_back(game);
            this->gamesMenu->AddItem(CreateItem(game));
            mainapp->UpdateSettings();
            populateGamesMenu();
            mainapp->SetFooterText("Games: " + std::to_string(amiiboGames.size()));
            pu::overlay::Toast *toast = new pu::overlay::Toast(name + " added", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void ManageLayout::removeGame_Click()
    {
        std::string gameName = this->gamesMenu->GetSelectedItem()->GetName();
        if(gameName == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL deletion is not allowed", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        int position = 0;
        for (auto & elem : this->amiiboGames) {
            if (elem->GetName() == gameName) {
                this->amiiboGames.erase(this->amiiboGames.begin() + position);
                mainapp->SetFooterText("Games: " + std::to_string(this->amiiboGames.size()));
                pu::overlay::Toast *toast = new pu::overlay::Toast(gameName + " removed", 20, {255,255,255,255}, {0,51,102,255});
                mainapp->StartOverlayWithTimeout(toast, 1500);
                break;
            }
            position++;
        }
        mainapp->UpdateSettings();
        populateGamesMenu();
        this->gamesMenu->SetSelectedIndex(0);
    }

    void ManageLayout::GetAllAmiibos()
    {
        std::vector<std::string> amiibos;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        std::sort(amiibos.begin(), amiibos.end(), &utils::NoPathSort);
        for (auto & elem : amiibos) {
            std::size_t found = elem.find_last_of("/\\");
			//std::string name = elem.substr(found+1);
            std::string name = elem.substr(sizeof(emuiiboFolder));
            if (name != "miis") {
                amiibo::AmiiboFile *file = new amiibo::AmiiboFile(name, elem, elem + "amiibo.png");
                this->allAmiiboFiles.push_back(file);
            }
        }
    }

    void ManageLayout::multiSelectItem_Click(amiibo::AmiiboFile *element, amiibo::AmiiboGame *game)
    {
        std::string amiiboName = element->GetName();
        size_t size = amiiboName.find("/");
        if (size != std::string::npos)
            amiiboName = amiiboName.substr(size + 1);
        std::vector<amiibo::AmiiboFile*> gameFiles = game->GetBinFiles();
        bool isInGame = false;
        int position = 0;
        pu::overlay::Toast *toast;
        for (auto & elem : gameFiles) {
            if (elem->GetName() == element->GetName()) {
                isInGame = true;
                break;
            }
            position++;
        }
        if (!isInGame) {
            game->AddAmiiboFile(element);
            this->amiiboMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/ingame2.png"));
            toast = new pu::overlay::Toast("Added: " + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
        } else {
            gameFiles.erase(gameFiles.begin() + position);
            game->SetAmiiboFiles(gameFiles);
            this->amiiboMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/notingame2.png"));
            toast = new pu::overlay::Toast("Removed: " + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
        }
        this->amiiboGames.erase(std::remove(this->amiiboGames.begin(), this->amiiboGames.end(), game), this->amiiboGames.end());
        this->amiiboGames.push_back(game);
        mainapp->UpdateSettings();
        mainapp->StartOverlayWithTimeout(toast, 700);
    }

    void ManageLayout::addAmiibos_Click(amiibo::AmiiboGame *game)
    {
        this->amiiboMenu->ClearItems();
        this->allAmiiboFiles.clear();
        this->GetAllAmiibos();
        this->waitInput = true;
        if(game->GetName() == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL is autamatically populated.", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        if (this->allAmiiboFiles.empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("You don't have any amiibos", 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        } else {
            mainapp->SetFooterText("Selecting amiibos for " + game->GetName());
            mainapp->SetHelpText("A: Select  B: Finish & Quit ");
        	for (auto & element : this->allAmiiboFiles) {
                bool inGame = false;
                std::string amiiboName = element->GetName();
                size_t size = amiiboName.find("/");
                if (size != std::string::npos)
                    amiiboName = amiiboName.substr(size + 1);
                pu::element::MenuItem *item = new pu::element::MenuItem(amiiboName);
                std::vector<amiibo::AmiiboGame *> parents = element->GetParents(this->amiiboGames);
                for (auto & elem : parents) {
                    if (elem->GetName() == game->GetName()) {
                        item->SetIcon(utils::GetRomFsResource("Common/ingame2.png"));
                        inGame = true;
                        break;
                    }
                }
                if (!inGame)
                    item->SetIcon(utils::GetRomFsResource("Common/notingame2.png"));
                item->AddOnClick(std::bind(&ManageLayout::multiSelectItem_Click, this, element, game), KEY_A);
                item->AddOnClick(std::bind(&ManageLayout::backToGame_Click, this), KEY_B);
                this->amiiboMenu->AddItem(item);
        	}
        	this->amiiboMenu->SetSelectedIndex(0);
        	this->SetElementOnFocus(this->amiiboMenu);
        	this->amiiboMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }

    void ManageLayout::backToGame_Click()
    {
        mainapp->SetFooterText("Games: " + std::to_string(mainapp->GetManageLayout()->GetAmiiboGames().size()));
        mainapp->SetHelpText("A: Select  X: Add new game  Y: Manage game's amiibos  Minus: Remove game ");
        this->SetElementOnFocus(this->gamesMenu);
        this->amiiboMenu->SetVisible(false);
        this->gamesMenu->SetVisible(true);    
    }
}
