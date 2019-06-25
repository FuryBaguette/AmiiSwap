#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    AmiibosLayout::AmiibosLayout() : pu::Layout()
    {
		this->amiibosMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 60, 10);
        this->amiibosMenu->SetOnFocusColor({102,153,204,255});
        this->amiibosMenu->SetScrollbarColor({102,153,204,255});
        this->amiibosMenu->SetVisible(true);
        this->Add(this->amiibosMenu);
        this->SetElementOnFocus(this->amiibosMenu);
        this->SetOnInput(std::bind(&AmiibosLayout::manage_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

	void AmiibosLayout::manage_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B){
            this->amiibosMenu->ClearItems();
            mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
            mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(6) + lang::GetDictionaryEntry(8) + lang::GetDictionaryEntry(9));
            mainapp->GetGamesLayout()->SetElementOnFocus(mainapp->GetGamesLayout()->GetGamesMenu());
            mainapp->GetGamesLayout()->GetGamesMenu()->SetVisible(true);
            mainapp->LoadLayout(mainapp->GetGamesLayout());
		}
	}

    AmiibosLayout::~AmiibosLayout()
    {
        delete this->amiibosMenu;
    }
    /*
    void AmiibosLayout::createAmiibosMenu()
    {
        this->amiibosMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 60, 10);
        this->amiibosMenu->SetOnFocusColor({102,153,204,255});
        this->amiibosMenu->SetScrollbarColor({102,153,204,255});
        this->amiibosMenu->SetVisible(true);
        this->Add(this->amiibosMenu);
        this->SetElementOnFocus(this->amiibosMenu);
        this->SetOnInput(std::bind(&AmiibosLayout::manage_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
	*/
    void AmiibosLayout::populateAmiibosMenu(amiibo::Game *game)
    {
        for (auto amiibo : game->GetAmiibos()) {
            this->amiibosMenu->AddItem(CreateItem(amiibo));
        }
		this->amiibosMenu->SetSelectedIndex(0);
    }

    pu::element::MenuItem *AmiibosLayout::CreateItem(std::string amiibo)
    {
        std::string amiiboName = amiibo;
        std::string amiiboPath = "sdmc:/emuiibo/" + amiibo;
        size_t size = amiibo.find_last_of("/\\");
        if (size != std::string::npos)
            amiiboName = amiibo.substr(size + 1);
        pu::element::MenuItem *item = new pu::element::MenuItem(amiiboName);

        item->SetIcon(amiiboPath + "/amiibo.icon");
        item->AddOnClick(std::bind(&AmiibosLayout::selectItem_Click, this, amiibo), KEY_A);
        item->AddOnClick(std::bind(&AmiibosLayout::randomizeUuid_Click, this, amiibo), KEY_X);
        return item;
    }

    void AmiibosLayout::selectItem_Click(std::string amiibo)
    {
        std::string amiiboName = amiibo;
        std::string amiiboPath = "sdmc:/emuiibo/" + amiibo;
        size_t size = amiibo.find_last_of("/\\");
        if (size != std::string::npos)
            amiiboName = amiibo.substr(size + 1);
        int sopt = mainapp->CreateShowDialog(lang::GetDictionaryEntry(31) + amiiboName + " ?", lang::GetDictionaryEntry(32) + amiiboName, { lang::GetDictionaryEntry(33), lang::GetDictionaryEntry(34) }, true, amiiboPath + "/amiibo.icon");
        if (sopt == 0) {
            nfpemuSetAmiibo(amiiboPath.c_str());
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(37) + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void AmiibosLayout::randomizeUuid_Click(std::string amiibo)
    {
    	//if (!this->waitInput) {
    	//	mainapp->SetWaitBack(true);
            std::string amiiboName = amiibo;
            std::string amiiboPath = "sdmc:/emuiibo/" + amiibo;
            std::string jsonPath = amiiboPath + "/amiibo.json";
            size_t size = amiibo.find_last_of("/\\");
            if (size != std::string::npos)
                amiiboName = amiibo.substr(size + 1);
            bool randomStatus = utils::isRandomUuid(jsonPath);
            int sopt = mainapp->CreateShowDialog(lang::GetDictionaryEntry(38), lang::GetDictionaryEntry(39) + amiiboName + lang::GetDictionaryEntry(40) + (randomStatus ? lang::GetDictionaryEntry(25) : lang::GetDictionaryEntry(26)), { lang::GetDictionaryEntry(35), lang::GetDictionaryEntry(36) }, true, amiiboPath + "/amiibo.icon");
            if(sopt == 0){
                toggleRandomUuid(jsonPath, !randomStatus);
                pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(39) + amiiboName + ((!randomStatus) ? lang::GetDictionaryEntry(25) : lang::GetDictionaryEntry(26)), 20, {255,255,255,255}, {0,51,102,255});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
        //    mainapp->SetWaitBack(false);
    	//} else this->waitInput = false;
    }

    void AmiibosLayout::toggleRandomUuid(std::string jsonPath, bool toggle)
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

    pu::element::Menu *AmiibosLayout::GetAmiibosMenu()
    {
    	return (this->amiibosMenu);
    }
}
