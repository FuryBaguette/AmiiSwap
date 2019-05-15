#include "MainApplication.hpp"
#include "json.hpp"

using json = nlohmann::json;

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
        utils::Log("MainLayout constructor start");
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
    }

    void MainLayout::populateGamesMenu()
    {
        this->GetAmiibos();
        auto gamesCount = this->amiiboGames.size();
        for (auto & element : this->amiiboGames) {
            pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
            std::string iconFile = element->GetName() + ".png";
            std::string amiiswapFolder ="sdmc:/switch/AmiiSwap/";
            if(std::fstream{amiiswapFolder+iconFile}){
                item->SetIcon(amiiswapFolder+iconFile);
            }else{
                item->SetIcon(utils::GetRomFsResource("Common/game.png"));
            }
            item->AddOnClick(std::bind(&MainLayout::category_Click, this, element), KEY_A);
            this->gamesMenu->AddItem(item);
        }
        mainapp->SetFooterText("Games: " + std::to_string(gamesCount));
    }

    void MainLayout::category_Click(amiibo::AmiiboGame *game)
    {
    	this->waitInput = true;
    	this->amiiboMenu->ClearItems();
    	std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
        if (files.empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast("List is empty", 20, {255,255,255,255}, {0,0,0,200});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        } else {
        	for (auto & element : files) {
                auto amiibosCount = files.size();
                mainapp->SetFooterText("Amiibos: " + std::to_string(amiibosCount));
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
        auto gamesCount = this->amiiboGames.size();
        mainapp->SetFooterText("Games: " + std::to_string(gamesCount));
    	return (this->gamesMenu);
    }

    pu::element::Menu *MainLayout::GetAmiiboMenu()
    {
    	return (this->amiiboMenu);
    }
}
