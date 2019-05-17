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

    void MainLayout::GetAmiibos()
    {
        set::Settings *settings = new set::Settings("sdmc:/switch/AmiiSwap/settings.txt");
        this->amiiboGames = settings->GetGames();
    }
    
    void MainLayout::populateGamesMenu()
    {
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
                mainapp->SetFooterText("Amiibos: " + std::to_string(amiiboFiles.size()));
                mainapp->SetHelpText("A: select  X: Toggle RandomUUID");
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
        mainapp->SetFooterText("Games: " + std::to_string(amiiboGames.size()));
        mainapp->SetHelpText("A: select  X: Add new game  Y: add Amiibo to game");
    	return (this->gamesMenu);
    }

    pu::element::Menu *MainLayout::GetAmiiboMenu()
    {
    	return (this->amiiboMenu);
    }

    void MainLayout::addGame_Click()
    {
        std::string name = utils::UserInput("Game Name", "");
        if(name != ""){
            amiibo::AmiiboGame *game = new amiibo::AmiiboGame(name);
            this->gamesMenu->AddItem(CreateItem(game));
            set::Settings *s = mainapp->GetSettings();
            s->AddGame(game);
            mainapp->UpdateSettings();
            pu::overlay::Toast *toast = new pu::overlay::Toast("Add new Game", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }
    
    void MainLayout::addAmiibos_Click(amiibo::AmiiboGame *game)
    {
        pu::overlay::Toast *toast = new pu::overlay::Toast("Add amiibos to " + game->GetName(), 20, {255,255,255,255}, {0,0,0,200});
        mainapp->StartOverlayWithTimeout(toast, 1500);
    }
}
