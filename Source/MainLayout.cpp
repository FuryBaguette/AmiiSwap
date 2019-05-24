#include "MainApplication.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace ui
{
    extern MainApplication *mainapp;

    MainLayout::MainLayout() : pu::Layout()
    {
        this->gamesMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 75, 8);
        this->amiiboMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 75, 8);
        this->gamesMenu->SetOnFocusColor({102,153,204,255});
        this->amiiboMenu->SetOnFocusColor({102,153,204,255});
        this->gamesMenu->SetScrollbarColor({102,153,204,255});
        this->amiiboMenu->SetScrollbarColor({102,153,204,255});
        this->amiiboMenu->SetVisible(false);
        this->Add(this->gamesMenu);
        this->Add(this->amiiboMenu);
        this->SetElementOnFocus(this->gamesMenu);
        populateGamesMenu();
    }

    MainLayout::~MainLayout()
    {
        delete this->gamesMenu;
        delete this->amiiboMenu;
    }

    std::vector<amiibo::AmiiboGame*> MainLayout::GetAmiiboGames()
    {
        return (this->amiiboGames);
    }

    void MainLayout::GetAmiibos()
    {
        set::Settings *settings = new set::Settings("sdmc:/switch/AmiiSwap/settings.txt");
        this->amiiboGames = settings->GetGames();
    }

    void MainLayout::populateGamesMenu()
    {
        this->gamesMenu->ClearItems();
        this->GetAmiibos();
        for (auto & element : this->amiiboGames) {
            this->gamesMenu->AddItem(CreateItem(element));
        }
        // TODO understand why this here crashes AMS, but works in other places
        // mainapp->SetFooterText("Games: " + std::to_string(amiiboGames.size()));
    }

    pu::element::MenuItem *MainLayout::CreateItem(amiibo::AmiiboGame *element)
    {
        pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
        std::string iconFile = element->GetName() + ".png";
        std::string amiiswapFolder ="sdmc:/switch/AmiiSwap/";
        if(std::fstream{amiiswapFolder+iconFile}){
            item->SetIcon(amiiswapFolder+iconFile);
        }else{
            item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        }
        item->AddOnClick(std::bind(&MainLayout::category_Click, this, element), KEY_A);
        item->AddOnClick(std::bind(&MainLayout::addGame_Click, this), KEY_X);
        item->AddOnClick(std::bind(&MainLayout::addAmiibos_Click, this, element), KEY_Y);
        item->AddOnClick(std::bind(&MainLayout::removeGame_Click, this), KEY_MINUS);
        return item;
    }

    void MainLayout::category_Click(amiibo::AmiiboGame *game)
    {
    	this->waitInput = true;
    	this->amiiboMenu->ClearItems();
    	this->amiiboFiles = game->GetBinFiles();
        if (amiiboFiles.empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("List is empty", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        } else {
        	for (auto & element : amiiboFiles) {
                mainapp->SetFooterText("Amiibos in " + game->GetName() +": " + std::to_string(amiiboFiles.size()));
                mainapp->SetHelpText("A: Select  X: Toggle RandomUUID");
                std::string amiiboName = element->GetName();
                size_t size = amiiboName.find("/");
                if (size != std::string::npos)
                    amiiboName = amiiboName.substr(size + 1);
                pu::element::MenuItem *item = new pu::element::MenuItem(amiiboName);
                item->SetIcon(element->GetIconPath());
                item->AddOnClick(std::bind(&MainLayout::selectItem_Click, this, element), KEY_A);
                item->AddOnClick(std::bind(&MainLayout::randomizeUuid_Click, this, element), KEY_X);
                this->amiiboMenu->AddItem(item);
        	}
        	this->amiiboMenu->SetSelectedIndex(0);
        	this->SetElementOnFocus(this->amiiboMenu);
        	this->amiiboMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }

    void MainLayout::selectItem_Click(amiibo::AmiiboFile *element)
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
                pu::overlay::Toast *toast = new pu::overlay::Toast("Active amiibo updated to: " + amiiboName, 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
    		mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }

    void MainLayout::randomizeUuid_Click(amiibo::AmiiboFile *element)
    {
    	if (!this->waitInput) {
    		mainapp->SetWaitBack(true);
            std::string amiiboName = element->GetName();
            std::string jsonPath = "sdmc:/emuiibo/" + amiiboName + "/amiibo.json";
            bool randomStatus = isRandomUuid(jsonPath);
            int sopt = mainapp->CreateShowDialog("Randomize UUID?", "This will toggle randomize UUID for the " + amiiboName + " Amiibo\nStatus: " + (randomStatus ? "enabled":"disabled"), { "Toggle", "Cancel" }, true, element->GetIconPath());
            if(sopt == 0){
                toggleRandomUuid(jsonPath, !randomStatus);
                pu::overlay::Toast *toast = new pu::overlay::Toast("Random UUID for " + amiiboName + ((!randomStatus) ? " enabled." : " disabled"), 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
            mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }

    bool MainLayout::isRandomUuid(std::string jsonPath)
    {
        std::ifstream ifs(jsonPath);
        auto amiiboJson = json::parse(ifs);
        if(ifs.is_open())
            ifs.close();
        return amiiboJson.value<bool>("randomizeUuid", false);
    }

    void MainLayout::toggleRandomUuid(std::string jsonPath, bool toggle)
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

    pu::element::Menu *MainLayout::GetGamesMenu()
    {
        mainapp->SetFooterText("Games: " + std::to_string(this->amiiboGames.size()));
        mainapp->SetHelpText("A: Select  X: Add new game  Y: Manage game's amiibos  Minus: Remove game");
    	return (this->gamesMenu);
    }

    pu::element::Menu *MainLayout::GetAmiiboMenu()
    {
    	return (this->amiiboMenu);
    }

    void MainLayout::addGame_Click()
    {
        std::string name = utils::UserInput("Game Name", "");
        
        size_t size = name.find("/");
        if (size != std::string::npos) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("/ in game name is not allowed", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }

        if(find_if(name.begin(), name.end(), isalnum) == name.end()){
            pu::overlay::Toast *toast = new pu::overlay::Toast("Please use only alphanumeric characters", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }

        if(name == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL name is reserved", 20, {255,255,255,255}, {0,0,0,200});
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
            pu::overlay::Toast *toast = new pu::overlay::Toast(name + " added", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void MainLayout::removeGame_Click()
    {
        std::string gameName = this->gamesMenu->GetSelectedItem()->GetName();
        if(gameName == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL deletion is not allowed", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        int position = 0;
        for (auto & elem : this->amiiboGames) {
            if (elem->GetName() == gameName) {
                this->amiiboGames.erase(this->amiiboGames.begin() + position);
                mainapp->SetFooterText("Games: " + std::to_string(this->amiiboGames.size()));
                pu::overlay::Toast *toast = new pu::overlay::Toast(gameName + " removed", 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
                break;
            }
            position++;
        }
        mainapp->UpdateSettings();
        populateGamesMenu();
    }

    void MainLayout::GetAllAmiibos()
    {
        std::vector<std::string> amiibos;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
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

    void MainLayout::multiSelectItem_Click(amiibo::AmiiboFile *element, amiibo::AmiiboGame *game)
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
            this->amiiboMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/ingame.png"));
            toast = new pu::overlay::Toast("Added: " + amiiboName, 20, {255,255,255,255}, {0,0,0,200});
        } else {
            gameFiles.erase(gameFiles.begin() + position);
            game->SetAmiiboFiles(gameFiles);
            this->amiiboMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/notingame.png"));
            toast = new pu::overlay::Toast("Removed: " + amiiboName, 20, {255,255,255,255}, {0,0,0,200});
        }
        this->amiiboGames.erase(std::remove(this->amiiboGames.begin(), this->amiiboGames.end(), game), this->amiiboGames.end());
        this->amiiboGames.push_back(game);
        mainapp->UpdateSettings();
        mainapp->StartOverlayWithTimeout(toast, 700);
    }

    void MainLayout::addAmiibos_Click(amiibo::AmiiboGame *game)
    {
        this->amiiboMenu->ClearItems();
        this->allAmiiboFiles.clear();
        this->GetAllAmiibos();
        this->waitInput = true;
        if(game->GetName() == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast("ALL is autamatically populated.", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        if (this->allAmiiboFiles.empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("You don't have any amiibos", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        } else {
            mainapp->SetFooterText("Selecting amiibos for " + game->GetName());
            mainapp->SetHelpText("A: Select  B: Finish & Quit");
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
                        item->SetIcon(utils::GetRomFsResource("Common/ingame.png"));
                        inGame = true;
                        break;
                    }
                }
                if (!inGame)
                    item->SetIcon(utils::GetRomFsResource("Common/notingame.png"));
                item->AddOnClick(std::bind(&MainLayout::multiSelectItem_Click, this, element, game), KEY_A);
                this->amiiboMenu->AddItem(item);
        	}
        	this->amiiboMenu->SetSelectedIndex(0);
        	this->SetElementOnFocus(this->amiiboMenu);
        	this->amiiboMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }
}
