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
		this->header = new pu::element::Rectangle(0, 0, 1280, 80, {0,102,153,255});
		this->footer = new pu::element::Rectangle(0, 680, 1280, 40, {0,102,153,255});

		this->logo = new pu::element::Image(10, 10, utils::GetRomFsResource("Common/logo.png"));
		this->logo->SetHeight(60);
		this->logo->SetWidth(60);

		this->headerText = new pu::element::TextBlock(80, 15, "AmiiSwap " + std::string(AMIISWAP_VERSION), 40);
		this->headerText->SetColor({255,255,255,255});

		this->emuiiboText = new pu::element::TextBlock(80, 55, "-", 20);
		this->emuiiboText->SetColor({255,255,255,255});
		this->emuiiboText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

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
			this->mainLayout->Add(this->logo);
			this->mainLayout->Add(this->headerText);
			this->mainLayout->Add(this->emuiiboText);
			this->mainLayout->Add(this->footerText);
			this->mainLayout->Add(this->helpText);

			this->manageLayout = new ManageLayout();
			this->manageLayout->Add(this->header);
			this->manageLayout->Add(this->footer);
			this->manageLayout->Add(this->logo);
			this->manageLayout->Add(this->headerText);
			this->manageLayout->Add(this->emuiiboText);
			this->manageLayout->Add(this->footerText);
			this->manageLayout->Add(this->helpText);

			this->emuiiboLayout = new EmuiiboLayout();
			this->emuiiboLayout->Add(this->header);
			this->emuiiboLayout->Add(this->footer);
			this->emuiiboLayout->Add(this->logo);
			this->emuiiboLayout->Add(this->headerText);
			this->emuiiboLayout->Add(this->emuiiboText);
			this->emuiiboLayout->Add(this->footerText);
			this->emuiiboLayout->Add(this->helpText);

			this->setLayout = new SettingsLayout();
			this->setLayout->Add(this->header);
			this->setLayout->Add(this->footer);
			this->setLayout->Add(this->logo);
			this->setLayout->Add(this->headerText);
			this->setLayout->Add(this->emuiiboText);
			this->setLayout->Add(this->footerText);
			this->setLayout->Add(this->helpText);

			this->aboutLayout = new AboutLayout();
			this->aboutLayout->Add(this->header);
			this->aboutLayout->Add(this->footer);
			this->aboutLayout->Add(this->logo);
			this->aboutLayout->Add(this->headerText);
			this->aboutLayout->Add(this->emuiiboText);
			this->aboutLayout->Add(this->footerText);
			this->aboutLayout->Add(this->helpText);

			this->AddThread(std::bind(&MainApplication::UpdateEmuiiboStatus, this));
			this->SetOnInput(std::bind(&MainApplication::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			this->LoadLayout(this->mainLayout);
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
		delete this->footer;
		delete this->footerText;
		delete this->helpText;
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

	std::string MainApplication::GetEmuiiboStatus()
	{
		NfpEmuToggleStatus nfpStatus;
		std::string status = "";
		Result rc = nfpemuGetToggleStatus(&nfpStatus);
		if(rc == 0){
			switch(nfpStatus){
				case NfpEmuToggleStatus_Off:
					status = "disabled";
					break;
				case NfpEmuToggleStatus_On:
					status = "enabled";
					break;
				case NfpEmuToggleStatus_Once:
					status = "enabled once";
					break;
				default:
					status = "unknown";
					break;
			}	
		}
		return status;
	}

	void MainApplication::ShowSelectedAmiibo()
	{
		char* amiibo;
		Result rs = nfpemuGetAmiibo(amiibo);
		//utils::Log(std::to_string(rs));
		if(rs == 0){
			//utils::Log("rs==0");
			pu::overlay::Toast *toast = new pu::overlay::Toast("Selected amiibo is: ", 20, {255,255,255,255}, {0,0,0,200});
			mainapp->StartOverlayWithTimeout(toast, 1500);
		} else {
			pu::overlay::Toast *toast = new pu::overlay::Toast("No amiibo selected.", 20, {255,255,255,255}, {0,0,0,200});
			mainapp->StartOverlayWithTimeout(toast, 1500);
		}
	}

	void MainApplication::UpdateEmuiiboStatus()
	{
		this->emuiiboText->SetText("emuiibo is " + this->GetEmuiiboStatus() + " ");
	}

	void MainApplication::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if(Down & KEY_PLUS) this->Close();
		//else if(Down & KEY_ZL) this->ShowSelectedAmiibo();
    }
}
