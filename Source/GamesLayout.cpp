#include "MainApplication.hpp"

extern lang::Language *language;
extern set::Settings *settings;

namespace ui
{
    extern MainApplication *mainapp;

    GamesLayout::GamesLayout() : pu::Layout()
    {
        this->gamesMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->allAmiibosMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 60, 10);
        this->gamesMenu->SetOnFocusColor({102,153,204,255});
        this->allAmiibosMenu->SetOnFocusColor({102,153,204,255});
        this->gamesMenu->SetScrollbarColor({102,153,204,255});
        this->allAmiibosMenu->SetScrollbarColor({102,153,204,255});
        this->allAmiibosMenu->SetVisible(false);
        this->Add(this->gamesMenu);
        this->Add(this->allAmiibosMenu);
        this->SetElementOnFocus(this->gamesMenu);
        this->SetOnInput(std::bind(&GamesLayout::manage_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

	void GamesLayout::manage_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B && !this->waitInput){
            mainapp->SetHelpText(lang::GetDictionaryEntry(2));
            mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
            mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
            mainapp->GetMainLayout()->selectionChange();
            mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}

    GamesLayout::~GamesLayout()
    {
        delete this->gamesMenu;
        delete this->allAmiibosMenu;
    }
	/*
    std::vector<amiibo::Game*> GamesLayout::GetAmiiboGames()
    {
        return this->amiiboGames;
    }
	*/
    void GamesLayout::populateGamesMenu()
    {
        this->gamesMenu->ClearItems();
		for (auto & game : set::GetGames()) {
			this->gamesMenu->AddItem(CreateItem(game));
        }
    }

    pu::element::MenuItem *GamesLayout::CreateItem(amiibo::Game *game)
    {
        pu::element::MenuItem *item = new pu::element::MenuItem(game->GetName());
        std::string iconFile = game->GetName() + ".png";
        std::string amiiswapFolder ="sdmc:/switch/AmiiSwap/game_icons/";
        if(std::fstream{amiiswapFolder+iconFile}){
            item->SetIcon(amiiswapFolder+iconFile);
        }else{
            item->SetIcon(utils::GetRomFsResource("Common/game.png"));
        }
        item->AddOnClick(std::bind(&GamesLayout::category_Click, this, game), KEY_A);
        item->AddOnClick(std::bind(&GamesLayout::addGame_Click, this), KEY_X);
        item->AddOnClick(std::bind(&GamesLayout::addAmiibos_Click, this, game), KEY_Y);
        item->AddOnClick(std::bind(&GamesLayout::removeGame_Click, this), KEY_MINUS);
        return item;
    }

    void GamesLayout::category_Click(amiibo::Game *game)
    {
    	if (game->GetAmiibos().empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(54), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        } else {
            mainapp->SetFooterText(lang::GetDictionaryEntry(55) + game->GetName() +": " + std::to_string(game->GetAmiibos().size()));
            mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(7));
            mainapp->GetAmiibosLayout()->populateAmiibosMenu(game);
            mainapp->LoadLayout(mainapp->GetAmiibosLayout());
        }
    }

    pu::element::Menu *GamesLayout::GetGamesMenu()
    {
        mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
        mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(6) + lang::GetDictionaryEntry(8) + lang::GetDictionaryEntry(9));
    	return (this->gamesMenu);
    }

    pu::element::Menu *GamesLayout::GetAllAmiibosMenu()
    {
    	return this->allAmiibosMenu;
    }

    void GamesLayout::addGame_Click()
    {
        std::string name = utils::UserInput(lang::GetDictionaryEntry(56), "");
        std::transform(name.begin(), name.end(), name.begin(), utils::ClearForbidden);
       
        if(name == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(57), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }

        if(name != ""){
            amiibo::Game *game = new amiibo::Game(name);
            //this->amiiboGames.insert(this->amiiboGames.end(), game);
			set::AddGame(game);
            //this->gamesMenu->AddItem(CreateItem(game));
            //mainapp->UpdateSettings();
			//set::Initialize();
            populateGamesMenu();
            mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
            pu::overlay::Toast *toast = new pu::overlay::Toast(name + lang::GetDictionaryEntry(58), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
        }
    }

    void GamesLayout::removeGame_Click()
    {
        std::string gameName = this->gamesMenu->GetSelectedItem()->GetName();
        if(gameName == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(59), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
		set::RemoveGame(gameName);
        mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
        pu::overlay::Toast *toast = new pu::overlay::Toast(gameName + lang::GetDictionaryEntry(60), 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);
        //mainapp->UpdateSettings();
        populateGamesMenu();
        this->gamesMenu->SetSelectedIndex(0);
    }
	/*
    void GamesLayout::GetAllAmiibos()
    {
        std::vector<std::string> amiibos;
		std::string name;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        std::sort(amiibos.begin(), amiibos.end(), &utils::NoPathSort);
        for (auto & amiibo : amiibos) {
            std::size_t found = amiibo.find_last_of("/\\");
			if (found != std::string::npos)
				name = amiibo.substr(found + 1);
            if (name != "miis") {
                amiibo::Amiibo *file = new amiibo::Amiibo(name, amiibo, amiibo + "amiibo.icon");
                this->allAmiibos.insert(this->allAmiibos.end(), file);
            }
        }
    }
	*/
    void GamesLayout::multiSelectItem_Click(amiibo::Amiibo *amiibo, amiibo::Game *game)
    {
        std::string amiiboName = amiibo->GetName();
        size_t size = amiiboName.find_last_of("/\\");
        if (size != std::string::npos)
            amiiboName = amiiboName.substr(size + 1);
        std::vector<std::string> amiibosInGame = game->GetAmiibos();
        bool isInGame = false;
        int position = 0;
        pu::overlay::Toast *toast;
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
            toast = new pu::overlay::Toast(lang::GetDictionaryEntry(61) + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
        } else {
            amiibosInGame.erase(amiibosInGame.begin() + position);
            game->SetAmiibos(amiibosInGame);
            this->allAmiibosMenu->GetSelectedItem()->SetIcon(utils::GetRomFsResource("Common/notingame2.png"));
            toast = new pu::overlay::Toast(lang::GetDictionaryEntry(62) + amiiboName, 20, {255,255,255,255}, {0,51,102,255});
        }
        //this->amiiboGames.erase(std::remove(this->amiiboGames.begin(), this->amiiboGames.end(), game), this->amiiboGames.end());
		set::RemoveGame(game->GetName());
        //this->amiiboGames.insert(this->amiiboGames.end(), game);
		set::AddGame(game);
        mainapp->StartOverlayWithTimeout(toast, 700);
    }

    void GamesLayout::addAmiibos_Click(amiibo::Game *game)
    {
        this->allAmiibosMenu->ClearItems();
        this->waitInput = true;
        if(game->GetName() == "ALL"){
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(63), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        }
        if (set::GetAllAmiibos().empty()) {
            pu::overlay::Toast *toast = new pu::overlay::Toast(lang::GetDictionaryEntry(64), 20, {255,255,255,255}, {0,51,102,255});
            mainapp->StartOverlayWithTimeout(toast, 1500);
            return;
        } else {
            mainapp->SetFooterText(lang::GetDictionaryEntry(65) + game->GetName());
            mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(4));
        	for (auto & amiibo : set::GetAllAmiibos()) {
                bool inGame = false;
                std::string amiiboName = amiibo->GetName();
                size_t size = amiiboName.find_last_of("/\\");
                if (size != std::string::npos)
                    amiiboName = amiiboName.substr(size + 1);
                pu::element::MenuItem *item = new pu::element::MenuItem(amiiboName);
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
        	this->SetElementOnFocus(this->allAmiibosMenu);
        	this->allAmiibosMenu->SetVisible(true);
        	this->gamesMenu->SetVisible(false);
        }
    }

    void GamesLayout::backToGame_Click()
    {
        //mainapp->UpdateSettings();
        populateGamesMenu();
        mainapp->SetFooterText(lang::GetDictionaryEntry(30) + std::to_string(set::GetGamesSize()));
        mainapp->SetHelpText(lang::GetDictionaryEntry(2) + lang::GetDictionaryEntry(6) + lang::GetDictionaryEntry(8) + lang::GetDictionaryEntry(9));
        this->SetElementOnFocus(this->gamesMenu);
        this->allAmiibosMenu->SetVisible(false);
        this->gamesMenu->SetVisible(true);  
        this->waitInput = false; 
    }
}
