#include "MainApplication.hpp"
extern lang::Language *language;
extern set::Settings *settings;

namespace ui
{
    extern MainApplication *mainapp;

    GamesLayout::GamesLayout()
    {
        this->gamesMenu = pu::ui::elm::Menu::New(0, 80, 1280, pu::ui::Color(255,255,255,255), 100, 6);
        this->allAmiibosMenu = pu::ui::elm::Menu::New(0, 80, 1280, pu::ui::Color(255,255,255,255), 60, 10);
        this->gamesMenu->SetOnFocusColor(pu::ui::Color(102,153,204,255));
        this->allAmiibosMenu->SetOnFocusColor(pu::ui::Color(102,153,204,255));
        this->gamesMenu->SetScrollbarColor(pu::ui::Color(102,153,204,255));
        this->allAmiibosMenu->SetScrollbarColor(pu::ui::Color(102,153,204,255));
        this->allAmiibosMenu->SetVisible(false);
        this->Add(this->allAmiibosMenu);
		this->Add(this->gamesMenu);
        //this->SetElementOnFocus(this->gamesMenu);
        this->SetOnInput(std::bind(&GamesLayout::manage_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

	void GamesLayout::manage_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B && !this->waitInput){
            mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT));
            mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
            //mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
            mainapp->GetMainLayout()->selectionChange();
            mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}
	
	void GamesLayout::populateGamesMenu()
    {
        this->gamesMenu->ClearItems();
		for (auto & game : set::GetGames()) {
			CreateItem(game);
        }
    }

    void GamesLayout::CreateItem(amiibo::Game *game)
    {
        pu::ui::elm::MenuItem::Ref item = pu::ui::elm::MenuItem::New(game->GetName());
        std::string amiiswapFolder ="sdmc:/switch/AmiiSwap/game_icons/";
        item->SetIcon(utils::GetRomFsResource("Common/game.png"));
		std::vector<pu::String> iconFileCandidates = { game->GetName() + ".png",game->GetName() + ".jpg",game->GetName() + ".jpeg" };
		for (auto& iconfile : iconFileCandidates) {
			if (utils::fileExists(amiiswapFolder + iconfile)) {
				std::string icon = amiiswapFolder + iconfile.AsUTF8();
				item->SetIcon(icon);
				break;
			}
		}
		item->AddOnClick(std::bind(&GamesLayout::category_Click, this, game), KEY_A);
        item->AddOnClick(std::bind(&GamesLayout::addGame_Click, this), KEY_X);
        item->AddOnClick(std::bind(&GamesLayout::addAmiibos_Click, this, game), KEY_Y);
        item->AddOnClick(std::bind(&GamesLayout::removeGame_Click, this), KEY_MINUS);
        this->gamesMenu->AddItem(item);
    }

    void GamesLayout::category_Click(amiibo::Game *game)
    {
    	if (game->GetAmiibos().empty()) {
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_GAMES_NO_AMIIBO), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
        } else {
            mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_AMIIBO) + game->GetName() +": " + std::to_string(game->GetAmiibos().size()));
            mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT) + lang::GetLabel(lang::Label::HELP_TOGGLE));
            mainapp->GetAmiibosLayout()->populateAmiibosMenu(game);
            mainapp->LoadLayout(mainapp->GetAmiibosLayout());
        }
    }

    pu::ui::elm::Menu::Ref GamesLayout::GetGamesMenu()
    {
        mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_GAMES) + std::to_string(set::GetGamesSize()));
        mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT) + lang::GetLabel(lang::Label::HELP_ADD) + lang::GetLabel(lang::Label::HELP_MANAGE) + lang::GetLabel(lang::Label::HELP_MINUS));
    	return (this->gamesMenu);
    }

    pu::ui::elm::Menu::Ref GamesLayout::GetAllAmiibosMenu()
    {
    	return this->allAmiibosMenu;
    }

    void GamesLayout::addGame_Click()
    {
        std::string name = utils::UserInput(lang::GetLabel(lang::Label::KEYBOARD_HINT), "");
        std::transform(name.begin(), name.end(), name.begin(), utils::ClearForbidden);
       
        if(name == "ALL"){
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_GAME_ALL_RESERVED), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }

        if(name != ""){
            amiibo::Game *game = new amiibo::Game(name);
            set::AddGame(game);
            populateGamesMenu();
            mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_GAMES) + std::to_string(set::GetGamesSize()));
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(utils::replace(lang::GetLabel(lang::Label::TOAST_GAME_ADDED), "{{GAME_NAME}}", name), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void GamesLayout::removeGame_Click()
    {
        pu::String gameName = this->gamesMenu->GetSelectedItem()->GetName();
        if(gameName == "ALL"){
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_GAME_ALL_CANT_DELETE), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
		set::RemoveGame(gameName);
        mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_GAMES) + std::to_string(set::GetGamesSize()));
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(utils::replace(lang::GetLabel(lang::Label::TOAST_GAME_REMOVED), "{{GAME_NAME}}", gameName), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);
        populateGamesMenu();
        this->gamesMenu->SetSelectedIndex(0);
    }

	void GamesLayout::multiSelectItem_Click(amiibo::Amiibo *amiibo, amiibo::Game *game)
    {
        std::string amiiboName = amiibo->GetName();
        size_t size = amiiboName.find_last_of("/\\");
        if (size != std::string::npos)
            amiiboName = amiiboName.substr(size + 1);
        std::vector<std::string> amiibosInGame = game->GetAmiibos();
        bool isInGame = false;
        int position = 0;
        pu::ui::extras::Toast::Ref toast;
        for (auto aig : amiibosInGame) {
            if (aig == amiibo->GetName()) {
                isInGame = true;
                break;
            }
            position++;
        }
        if (!isInGame) {
            game->AddAmiibo(amiiboName);
            this->allAmiibosMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/ingame2.png"));
            toast = pu::ui::extras::Toast::New(utils::replace(lang::GetLabel(lang::Label::TOAST_AMIIBO_ADDED), "{{AMIIBO_NAME}}", amiiboName), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        } else {
            amiibosInGame.erase(amiibosInGame.begin() + position);
            game->SetAmiibos(amiibosInGame);
            this->allAmiibosMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/notingame2.png"));
            toast = pu::ui::extras::Toast::New(utils::replace(lang::GetLabel(lang::Label::TOAST_AMIIBO_REMOVED), "{{AMIIBO_NAME}}", amiiboName), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        }
        set::RemoveGame(game->GetName());
        set::AddGame(game);
        mainapp->StartOverlayWithTimeout(toast, 700);
    }

    void GamesLayout::addAmiibos_Click(amiibo::Game *game)
    {
        this->allAmiibosMenu->ClearItems();
        this->waitInput = true;
        if(game->GetName() == "ALL"){
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_GAME_ALL_POPULATED), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        if (set::GetAllAmiibos().empty()) {
            pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_NO_AMIIBO), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        } else {
            mainapp->SetFooterText(utils::replace(lang::GetLabel(lang::Label::FOOTER_AMIIBO_SELECT), "{{GAME_NAME}}", game->GetName()));
            mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT) + lang::GetLabel(lang::Label::HELP_FINISH));
        	for (auto & amiibo : set::GetAllAmiibos()) {
                bool inGame = false;
                std::string amiiboName = amiibo->GetName();
                size_t size = amiiboName.find_last_of("/\\");
                if (size != std::string::npos)
                    amiiboName = amiiboName.substr(size + 1);
                pu::ui::elm::MenuItem::Ref item = pu::ui::elm::MenuItem::New(amiiboName);
                std::vector<amiibo::Game*> parents = amiibo->GetParents(set::GetGames());
                for (auto & elem : parents) {
                    if (elem->GetName() == game->GetName()) {
                        item->SetIcon(utils::GetRomFsResource("Common/ingame2.png"));
                        inGame = true;
                        break;
                    }
                }
                if (!inGame)
                    item->SetIcon(utils::GetRomFsResource("Common/notingame2.png"));
                item->AddOnClick(std::bind(&GamesLayout::multiSelectItem_Click, this, amiibo, game), KEY_A);
                item->AddOnClick(std::bind(&GamesLayout::backToGame_Click, this), KEY_B);
                this->allAmiibosMenu->AddItem(item);
        	}
        	this->allAmiibosMenu->SetSelectedIndex(0);
        	//this->SetElementOnFocus(this->allAmiibosMenu);
        	this->allAmiibosMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }

    void GamesLayout::backToGame_Click()
    {
        populateGamesMenu();
        mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_GAMES) + std::to_string(set::GetGamesSize()));
        mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT) + lang::GetLabel(lang::Label::HELP_ADD) + lang::GetLabel(lang::Label::HELP_MANAGE) + lang::GetLabel(lang::Label::HELP_MINUS));
        //this->SetElementOnFocus(this->gamesMenu);
        this->allAmiibosMenu->SetVisible(false);
        this->gamesMenu->SetVisible(true);  
        this->waitInput = false; 
    }
}
