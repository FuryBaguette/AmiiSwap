#include <MainApplication.hpp>
extern char *fake_heap_end;

namespace ui
{
	MainApplication *mainapp;
	static void *ghaddr;

	void Initialize()
	{
        srand(time(NULL));
		if(R_FAILED(svcSetHeapSize(&ghaddr, 0x10000000))) exit(1);
		fake_heap_end = (char*)ghaddr + 0x10000000;
    }

	void Finalize()
	{
		if (utils::IsEmuiiboPresent())
			nfpemuExit();
		svcSetHeapSize(&ghaddr, ((u8*)envGetHeapOverrideAddr() + envGetHeapOverrideSize()) - (u8*)ghaddr);
	}

	MainApplication::MainApplication() : pu::Application()
	{
		this->header = new pu::element::Rectangle(0, 0, 1280, 79, {0,102,153,255});
		this->footer = new pu::element::Rectangle(0, 681, 1280, 39, {0,102,153,255});
		this->headerShadow = new pu::element::Rectangle(0, 79, 1280, 1, {0,51,102,255});
		this->footerShadow = new pu::element::Rectangle(0, 680, 1280, 1, {0,51,102,255});
		this->emuiiboLed = new pu::element::Rectangle(1255, 10, 15, 15, {0,102,153,255}, 7);

		this->logo = new pu::element::Image(10, 10, utils::GetRomFsResource("Common/logo.png"));
		this->logo->SetHeight(60);
		this->logo->SetWidth(60);

		this->headerText = new pu::element::TextBlock(80, 20, "AmiiSwap " + std::string(AMIISWAP_VERSION), 40);
		this->headerText->SetColor({255,255,255,255});

		this->emuiiboText = new pu::element::TextBlock(80, 30, "", 20);
		this->emuiiboText->SetColor({255,255,255,255});
		this->emuiiboText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

		this->amiiboText = new pu::element::TextBlock(80, 55, "", 20);
		this->amiiboText->SetColor({255,255,255,255});
		this->amiiboText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

		this->footerText = new pu::element::TextBlock(10, 690, "", 20);
		this->footerText->SetColor({255,255,255,255});

		this->helpText = new pu::element::TextBlock(10, 690, "", 20);
		this->helpText->SetColor({255,255,255,255});
		this->helpText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

		this->errorLayout = new ErrorLayout();
		this->errorLayout->Add(this->header);
		this->errorLayout->Add(this->footer);
		this->errorLayout->Add(this->logo);
		this->errorLayout->Add(this->headerText);
		this->errorLayout->Add(this->footerText);
		this->errorLayout->Add(this->helpText);

		if (!utils::IsEmuiiboPresent()) {
            this->ShowError("Emuiibo is not running on this console, please install it before using AmiiSwap");
		} else {
			nfpemuInitialize();
        	utils::EnsureDirectories();
			InitSettings();			

			this->emuiiboText->SetText("emuiibo is " + this->GetEmuiiboStatus());
			this->helpText->SetText("A: Select "); 

			this->mainLayout = new MainLayout();
			this->mainLayout->Add(this->header);
			this->mainLayout->Add(this->footer);
			this->mainLayout->Add(this->headerShadow);
			this->mainLayout->Add(this->footerShadow);
			this->mainLayout->Add(this->emuiiboLed);
			this->mainLayout->Add(this->logo);
			this->mainLayout->Add(this->headerText);
			this->mainLayout->Add(this->emuiiboText);
			this->mainLayout->Add(this->amiiboText);
			this->mainLayout->Add(this->footerText);
			this->mainLayout->Add(this->helpText);

			this->manageLayout = new ManageLayout();
			this->manageLayout->Add(this->header);
			this->manageLayout->Add(this->footer);
			this->manageLayout->Add(this->headerShadow);
			this->manageLayout->Add(this->footerShadow);
			this->manageLayout->Add(this->emuiiboLed);
			this->manageLayout->Add(this->logo);
			this->manageLayout->Add(this->headerText);
			this->manageLayout->Add(this->emuiiboText);
			this->manageLayout->Add(this->amiiboText);
			this->manageLayout->Add(this->footerText);
			this->manageLayout->Add(this->helpText);

			this->emuiiboLayout = new EmuiiboLayout();
			this->emuiiboLayout->Add(this->header);
			this->emuiiboLayout->Add(this->footer);
			this->emuiiboLayout->Add(this->headerShadow);
			this->emuiiboLayout->Add(this->footerShadow);
			this->emuiiboLayout->Add(this->emuiiboLed);
			this->emuiiboLayout->Add(this->logo);
			this->emuiiboLayout->Add(this->headerText);
			this->emuiiboLayout->Add(this->emuiiboText);
			this->emuiiboLayout->Add(this->amiiboText);
			this->emuiiboLayout->Add(this->footerText);
			this->emuiiboLayout->Add(this->helpText);

			this->setLayout = new SettingsLayout();
			this->setLayout->Add(this->header);
			this->setLayout->Add(this->footer);
			this->setLayout->Add(this->headerShadow);
			this->setLayout->Add(this->footerShadow);
			this->setLayout->Add(this->emuiiboLed);
			this->setLayout->Add(this->logo);
			this->setLayout->Add(this->headerText);
			this->setLayout->Add(this->emuiiboText);
			this->setLayout->Add(this->amiiboText);
			this->setLayout->Add(this->footerText);
			this->setLayout->Add(this->helpText);

			this->aboutLayout = new AboutLayout();
			this->aboutLayout->Add(this->header);
			this->aboutLayout->Add(this->footer);
			this->aboutLayout->Add(this->headerShadow);
			this->aboutLayout->Add(this->footerShadow);
			this->aboutLayout->Add(this->emuiiboLed);
			this->aboutLayout->Add(this->logo);
			this->aboutLayout->Add(this->headerText);
			this->aboutLayout->Add(this->emuiiboText);
			this->aboutLayout->Add(this->amiiboText);
			this->aboutLayout->Add(this->footerText);
			this->aboutLayout->Add(this->helpText);

			this->AddThread(std::bind(&MainApplication::UpdateEmuiiboStatus, this));
			this->SetOnInput(std::bind(&MainApplication::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			this->LoadLayout(this->mainLayout);
			this->start = std::chrono::steady_clock::now();
		}
	}

    MainApplication::~MainApplication()
    {
		delete this->mainLayout;
		delete this->manageLayout;
		delete this->setLayout;
		delete this->errorLayout;
		delete this->aboutLayout;
		delete this->emuiiboLayout;
		delete this->logo;
		delete this->header;
		delete this->headerText;
		delete this->amiiboText;
		delete this->emuiiboText;
		delete this->footer;
		delete this->footerText;
		delete this->helpText;
		delete this->headerShadow;
		delete this->footerShadow;
		delete this->emuiiboLed;
	}

	void MainApplication::SetWaitBack(bool state)
	{
		this->waitBack = state;
	}

	bool MainApplication::GetWaitBack()
	{
		return this->waitBack;
	}

	void SetMainApplication(MainApplication *MainApp)
	{
		mainapp = MainApp;
	}

	void MainApplication::SetFooterText(std::string Text)
	{
		if(this->footerText != NULL) this->footerText->SetText(Text);
	}

	void MainApplication::SetHelpText(std::string Text)
	{
		if(this->helpText != NULL) this->helpText->SetText(Text);
	}
	void MainApplication::ShowError(std::string text)
	{
		this->header->SetColor({204,0,0,255});
		this->footer->SetColor({204,0,0,255});
		this->LoadLayout(this->errorLayout);
		this->errorLayout->SetText(text);
	}

	AboutLayout *MainApplication::GetAboutLayout()
    {
        return this->aboutLayout;
    }
	
	ManageLayout *MainApplication::GetManageLayout()
    {
        return this->manageLayout;
    }
	
	MainLayout *MainApplication::GetMainLayout()
    {
        return this->mainLayout;
    }

	SettingsLayout *MainApplication::GetSettingsLayout()
    {
        return this->setLayout;
    }

	ErrorLayout *MainApplication::GetErrorLayout()
    {
        return this->errorLayout;
    }

	EmuiiboLayout *MainApplication::GetEmuiiboLayout()
    {
        return this->emuiiboLayout;
    }
	set::Settings *MainApplication::GetSettings()
    {
        return this->settings;
    }

	void MainApplication::SetSettings(set::Settings *s)
    {
        this->settings = s;
    }

	void MainApplication::InitSettings()
    {
        std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt";
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> allAmiibos;
        utils::get_amiibos_directories(emuiiboFolder, &allAmiibos);
        allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/miis"), allAmiibos.end());
		std::sort(allAmiibos.begin(), allAmiibos.end(), &utils::NoPathSort);
        std::ifstream settingsIfs(settingsPath);
        if(!settingsIfs.good()){ //no settings.txt found, generate.
           	std::ofstream settingsOfs(settingsPath);
			settingsOfs << "[ALL]" << "\r" << "\n";
            for (auto & elem : allAmiibos) {
                settingsOfs << elem.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
            }
            if(settingsOfs.is_open())
                settingsOfs.close();
			set::Settings *s = new set::Settings(settingsPath);
            this->SetSettings(s);
        } else { // settings.txt available compare with actually available amiibos and rewrite
            if(settingsIfs.is_open())
                settingsIfs.close();
			set::Settings *s = new set::Settings(settingsPath);
            this->SetSettings(s);
            std::vector<amiibo::AmiiboGame*> gamesInConfig = this->settings->GetGames();
            std::vector<std::string> amiibosInConfig;
            std::vector<std::string> removedAmiibos;
			bool all=false;
            for(auto & game : gamesInConfig) {
				if (game->GetName()=="ALL")
					all=true;
                std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
                for (auto & element : files) {
					if(find(amiibosInConfig.begin(), amiibosInConfig.end(), std::string(element->GetName())) == amiibosInConfig.end())
                    	amiibosInConfig.push_back(element->GetName());
                }
            }
			std::sort(amiibosInConfig.begin(), amiibosInConfig.end(), &utils::NoPathSort);
			
            for(auto & element : amiibosInConfig){
                if(find(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element) != allAmiibos.end()){
                    allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element), allAmiibos.end());
                } else {
                    removedAmiibos.push_back(element);
                }
            }

            std::ofstream settingsOfs(settingsPath,std::ofstream::trunc);
			
			if(!all) {
				settingsOfs << "[ALL]" << "\r" << "\n";
				for(auto & element : allAmiibos){
						settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
				}
			}

            for(auto & game : gamesInConfig) {
				std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
				settingsOfs << "[" << game->GetName() <<"]" << "\r" << "\n";
				for (auto & element : files) {
					if(find(removedAmiibos.begin(), removedAmiibos.end(), element->GetName()) == removedAmiibos.end()) // missing amiibos are already ignored when creating menu but let's keep settings clean
						settingsOfs << element->GetName() << "\r" << "\n";
				}

				if (game->GetName() == "ALL"){
					for(auto & element : allAmiibos){
						settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
					}
				}
            }

		   if(settingsOfs.is_open())
                settingsOfs.close();
        }
    }

	void MainApplication::UpdateSettings()
	{
        std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt";
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> allAmiibos;
        utils::get_amiibos_directories(emuiiboFolder, &allAmiibos);
        allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/miis"), allAmiibos.end());
		std::sort(allAmiibos.begin(), allAmiibos.end(), &utils::NoPathSort);
		std::vector<amiibo::AmiiboGame*> gamesInConfig = this->manageLayout->GetAmiiboGames();
		std::vector<std::string> amiibosInConfig;
		std::vector<std::string> removedAmiibos;
		for(auto & game : gamesInConfig) {
			std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
			for (auto & element : files) {
				if(find(amiibosInConfig.begin(), amiibosInConfig.end(), std::string(element->GetName())) == amiibosInConfig.end())
					amiibosInConfig.push_back(element->GetName());
			}
		}
		std::sort(amiibosInConfig.begin(), amiibosInConfig.end(), &utils::NoPathSort);

		for(auto & element : amiibosInConfig){
			if(find(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element) != allAmiibos.end()){
				allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element), allAmiibos.end());
			} else {
				removedAmiibos.push_back(element);
			}
		}

		std::ofstream settingsOfs(settingsPath,std::ofstream::trunc);
		for(auto & game : gamesInConfig) {
			std::vector<amiibo::AmiiboFile*> files = game->GetBinFiles();
			settingsOfs << "[" << game->GetName() <<"]" << "\r" << "\n";
			for (auto & element : files) {
				if(find(removedAmiibos.begin(), removedAmiibos.end(), element->GetName()) == removedAmiibos.end()) // missing amiibos are already ignored when creating menu but let's keep settings clean
					settingsOfs << element->GetName() << "\r" << "\n";
			}

			if (game->GetName() == "ALL"){
				for(auto & element : allAmiibos){
					settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
				}
			}
		}

		if(settingsOfs.is_open())
			settingsOfs.close();
	}

	int MainApplication::GetEmuiiboStatus()
	{
		NfpEmuToggleStatus nfpStatus;
		std::string status = "";
		Result rc = nfpemuGetToggleStatus(&nfpStatus);
		if(rc == 0)
			return nfpStatus;
		return -1;
	}

	void MainApplication::UpdateEmuiiboStatus()
	{
		auto ct = std::chrono::steady_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(ct - this->start).count();
        if((diff >= 500))
        {
			switch(this->GetEmuiiboStatus()){
				case NfpEmuToggleStatus_Off:
					this->emuiiboText->SetText("emuiibo status: disabled ");
					this->emuiiboLed->SetColor({255,0,0,255});
					break;
				case NfpEmuToggleStatus_On:
					this->emuiiboText->SetText("emuiibo status: enabled ");
					this->emuiiboLed->SetColor({0,153,0,255});
					break;
				case NfpEmuToggleStatus_Once:
					this->emuiiboText->SetText("emuiibo status: enabled once ");
					this->emuiiboLed->SetColor({255,128,0,255});
					break;
				default:
					this->emuiiboText->SetText("emuiibo status: unknown ");
					this->emuiiboLed->SetColor({0,102,153,255});
					break;
			}
			this->activeAmiibo = utils::trim_right_copy(utils::getActiveAmiibo());
			this->amiiboText->SetText("active amiibo: " + this->activeAmiibo + " ");
		}
	}

	void MainApplication::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if(Down & KEY_PLUS) this->Close();
    }
}
