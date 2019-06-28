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
            mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_GAMES) + std::to_string(set::GetGamesSize()));
            mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT) + lang::GetLabel(lang::Label::HELP_ADD) + lang::GetLabel(lang::Label::HELP_MANAGE) + lang::GetLabel(lang::Label::HELP_MINUS));
            mainapp->GetGamesLayout()->SetElementOnFocus(mainapp->GetGamesLayout()->GetGamesMenu());
            mainapp->GetGamesLayout()->GetGamesMenu()->SetVisible(true);
            mainapp->LoadLayout(mainapp->GetGamesLayout());
		}
	}

    AmiibosLayout::~AmiibosLayout()
    {
        delete this->amiibosMenu;
    }
   
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
        int sopt = mainapp->CreateShowDialog(lang::GetLabel(lang::Label::SELECT_DIALOG_TITLE) + amiiboName + " ?", lang::GetLabel(lang::Label::SELECT_DIALOG_TEXT) + amiiboName, { lang::GetLabel(lang::Label::YES), lang::GetLabel(lang::Label::NO) }, true, amiiboPath + "/amiibo.icon");
        if (sopt == 0) {
            nfpemuSetAmiibo(amiiboPath.c_str());
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetLabel(lang::Label::TOAST_ACTIVE_AMIIBO) + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void AmiibosLayout::randomizeUuid_Click(std::string amiibo)
    {
    	std::string amiiboName = amiibo;
        std::string amiiboPath = "sdmc:/emuiibo/" + amiibo;
        std::string jsonPath = amiiboPath + "/amiibo.json";
        size_t size = amiibo.find_last_of("/\\");
        if (size != std::string::npos)
            amiiboName = amiibo.substr(size + 1);
        bool randomStatus = utils::isRandomUuid(jsonPath);
        int sopt = mainapp->CreateShowDialog(lang::GetLabel(lang::Label::RANDOM_DIALOG_TITLE), lang::GetLabel(lang::Label::RANDOM_DIALOG_TEXT_1) + amiiboName + lang::GetLabel(lang::Label::RANDOM_DIALOG_TEXT_2) + (randomStatus ? lang::GetLabel(lang::Label::ENABLED) : lang::GetLabel(lang::Label::DISABLED)), { lang::GetLabel(lang::Label::TOGGLE), lang::GetLabel(lang::Label::CANCEL) }, true, amiiboPath + "/amiibo.icon");
        if(sopt == 0){
            toggleRandomUuid(jsonPath, !randomStatus);
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetLabel(lang::Label::TOAST_RANDOM) + amiiboName + ((!randomStatus) ? lang::GetLabel(lang::Label::ENABLED) : lang::GetLabel(lang::Label::DISABLED)), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
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
