#include <MainApplication.hpp>

extern char *fake_heap_end;
extern lang::Language *language;
extern set::Settings *settings;


namespace ui
{
	MainApplication *mainapp;
	static void *ghaddr;

	void Initialize()
	{
        srand(time(NULL));
		if(R_FAILED(svcSetHeapSize(&ghaddr, 0x10000000))) exit(1);
		fake_heap_end = (char*)ghaddr + 0x10000000;
		if (R_FAILED(setInitialize())) exit(1);
    }

	void Finalize()
	{
		if (utils::IsEmuiiboPresent())
			nfpemuExit();
		svcSetHeapSize(&ghaddr, ((u8*)envGetHeapOverrideAddr() + envGetHeapOverrideSize()) - (u8*)ghaddr);
	}

	MainApplication::MainApplication() : pu::Application()
	{
		lang::Initialize();
		this->bootLayout = new BootLayout();
		this->LoadLayout(this->bootLayout);
		this->CallForRender();

		this->header = new pu::element::Rectangle(0, 0, 1280, 79, {0,102,153,255});
		this->footer = new pu::element::Rectangle(0, 681, 1280, 39, {0,102,153,255});
		this->headerShadow = new pu::element::Rectangle(0, 79, 1280, 1, {0,51,102,255});
		this->footerShadow = new pu::element::Rectangle(0, 680, 1280, 1, {0,51,102,255});

		this->logo = new pu::element::Image(10, 10, utils::GetRomFsResource("Common/logo.png"));
		this->logo->SetHeight(60);
		this->logo->SetWidth(60);

		this->headerText = new pu::element::TextBlock(80, 20, lang::GetDictionaryEntry(0) + std::string(AMIISWAP_VERSION), 40);
		this->headerText->SetColor({255,255,255,255});
		
		this->emuiiboLed = new pu::element::Rectangle(1255, 10, 15, 15, {0,102,153,255}, 5);
		this->emuiiboLed->SetBorderRadius(5);

		this->emuiiboText = new pu::element::TextBlock(80, 30, "", 20);
		this->emuiiboText->SetColor({255,255,255,255});
		this->emuiiboText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

		this->amiiboText = new pu::element::TextBlock(80, 55, "", 20);
		this->amiiboText->SetColor({255,255,255,255});
		this->amiiboText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);

		this->footerText = new pu::element::TextBlock(10, 690, lang::GetDictionaryEntry(1), 20);
		this->footerText->SetColor({255,255,255,255});

		this->helpText = new pu::element::TextBlock(10, 690, "", 20);
		this->helpText->SetColor({255,255,255,255});
		this->helpText->SetHorizontalAlign(pu::element::HorizontalAlign::Right);
		this->helpText->SetText(lang::GetDictionaryEntry(2));
		
		this->bootLayout->SetProgress(3.0f);
		this->CallForRender();

		this->errorLayout = new ErrorLayout();
		this->errorLayout->Add(this->header);
		this->errorLayout->Add(this->footer);
		this->errorLayout->Add(this->logo);
		this->errorLayout->Add(this->headerText);
		this->errorLayout->Add(this->footerText);
		this->errorLayout->Add(this->helpText);
		
		this->bootLayout->SetProgress(6.0f);
		this->CallForRender();
		
		this->gamesLayout = new GamesLayout();
		this->gamesLayout->Add(this->header);
		this->gamesLayout->Add(this->footer);
		this->gamesLayout->Add(this->headerShadow);
		this->gamesLayout->Add(this->footerShadow);
		this->gamesLayout->Add(this->emuiiboLed);
		this->gamesLayout->Add(this->logo);
		this->gamesLayout->Add(this->headerText);
		this->gamesLayout->Add(this->emuiiboText);
		this->gamesLayout->Add(this->amiiboText);
		this->gamesLayout->Add(this->footerText);
		this->gamesLayout->Add(this->helpText);
		
		this->bootLayout->SetProgress(9.0f);
		this->CallForRender();
		
		this->amiibosLayout = new AmiibosLayout();
		this->amiibosLayout->Add(this->header);
		this->amiibosLayout->Add(this->footer);
		this->amiibosLayout->Add(this->headerShadow);
		this->amiibosLayout->Add(this->footerShadow);
		this->amiibosLayout->Add(this->emuiiboLed);
		this->amiibosLayout->Add(this->logo);
		this->amiibosLayout->Add(this->headerText);
		this->amiibosLayout->Add(this->emuiiboText);
		this->amiibosLayout->Add(this->amiiboText);
		this->amiibosLayout->Add(this->footerText);
		this->amiibosLayout->Add(this->helpText);
		
		this->bootLayout->SetProgress(12.0f);
		this->CallForRender();
		
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
		
		this->bootLayout->SetProgress(15.0f);
		this->CallForRender();
		
		this->imagesLayout = new ImagesLayout();
		this->imagesLayout->Add(this->header);
		this->imagesLayout->Add(this->footer);
		this->imagesLayout->Add(this->headerShadow);
		this->imagesLayout->Add(this->footerShadow);
		this->imagesLayout->Add(this->emuiiboLed);
		this->imagesLayout->Add(this->logo);
		this->imagesLayout->Add(this->headerText);
		this->imagesLayout->Add(this->emuiiboText);
		this->imagesLayout->Add(this->amiiboText);
		this->imagesLayout->Add(this->footerText);
		this->imagesLayout->Add(this->helpText);
		
		this->bootLayout->SetProgress(18.0f);
		this->CallForRender();
		
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
		
		this->bootLayout->SetProgress(21.0f);
		this->CallForRender();
		
		this->manualLayout = new ManualLayout();
		this->manualLayout->Add(this->header);
		this->manualLayout->Add(this->footer);
		this->manualLayout->Add(this->headerShadow);
		this->manualLayout->Add(this->footerShadow);
		this->manualLayout->Add(this->emuiiboLed);
		this->manualLayout->Add(this->logo);
		this->manualLayout->Add(this->headerText);
		this->manualLayout->Add(this->emuiiboText);
		this->manualLayout->Add(this->amiiboText);
		this->manualLayout->Add(this->footerText);
		this->manualLayout->Add(this->helpText);

		this->bootLayout->SetProgress(24.0f);
		this->CallForRender();
		
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
		
		this->bootLayout->SetProgress(27.0f);
		this->CallForRender();
		
		this->amiiboDetailsLayout = new AmiiboDetailsLayout();
		this->amiiboDetailsLayout->Add(this->header);
		this->amiiboDetailsLayout->Add(this->footer);
		this->amiiboDetailsLayout->Add(this->headerShadow);
		this->amiiboDetailsLayout->Add(this->footerShadow);
		this->amiiboDetailsLayout->Add(this->emuiiboLed);
		this->amiiboDetailsLayout->Add(this->logo);
		this->amiiboDetailsLayout->Add(this->headerText);
		this->amiiboDetailsLayout->Add(this->emuiiboText);
		this->amiiboDetailsLayout->Add(this->amiiboText);
		this->amiiboDetailsLayout->Add(this->footerText);
		this->amiiboDetailsLayout->Add(this->helpText);
		
		this->bootLayout->SetProgress(30.0f);
		this->CallForRender();
		
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
		
		this->bootLayout->SetProgress(33.0f);
		this->CallForRender();
		
		this->SetOnInput(std::bind(&MainApplication::OnInput, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		
		this->bootLayout->SetText(lang::GetDictionaryEntry(12));
		this->bootLayout->SetProgress(36.0f);
		this->CallForRender();
		//InitSettings();
		set::Initialize();
		this->bootLayout->SetProgress(90.0f);
		this->CallForRender();
		this->gamesLayout->populateGamesMenu();
		this->bootLayout->SetProgress(100.0f);
		this->CallForRender();
		if (!utils::IsEmuiiboPresent()) {
			this->header->SetColor({204,0,0,255});
			this->footer->SetColor({204,0,0,255});
			this->emuiiboText->SetText(lang::GetDictionaryEntry(10));
			this->LoadLayout(this->mainLayout);
		} else {
			nfpemuInitialize();
			this->AddThread(std::bind(&MainApplication::UpdateEmuiiboStatus, this));
			this->LoadLayout(this->mainLayout);
			this->start = std::chrono::steady_clock::now();
		}
	}

    MainApplication::~MainApplication()
    {
		delete this->mainLayout;
		delete this->bootLayout;
		delete this->errorLayout;
		delete this->manualLayout;
		delete this->aboutLayout;
		delete this->gamesLayout;
		delete this->amiibosLayout;
		delete this->setLayout;
		delete this->emuiiboLayout;
		delete this->imagesLayout;
		delete this->amiiboDetailsLayout;
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
	/*
	void MainApplication::SetWaitBack(bool state)
	{
		this->waitBack = state;
	}

	bool MainApplication::GetWaitBack()
	{
		return this->waitBack;
	}
	*/
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
		//this->LoadLayout(this->errorLayout);
		this->errorLayout->SetText(text);
	}

	AboutLayout *MainApplication::GetAboutLayout()
    {
        return this->aboutLayout;
    }
	
	GamesLayout *MainApplication::GetGamesLayout()
    {
        return this->gamesLayout;
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

	ImagesLayout *MainApplication::GetImagesLayout()
    {
        return this->imagesLayout;
    }

	ManualLayout *MainApplication::GetManualLayout()
    {
        return this->manualLayout;
    }

	BootLayout *MainApplication::GetBootLayout()
    {
        return this->bootLayout;
    }

	AmiiboDetailsLayout *MainApplication::GetAmiiboDetailsLayout()
    {
        return this->amiiboDetailsLayout;
    }

	AmiibosLayout *MainApplication::GetAmiibosLayout()
    {
        return this->amiibosLayout;
    }
	/*
	void MainApplication::InitSettings()
    {
        std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt";
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> allAmiibos;
		float progress = 36.0f;
        utils::get_amiibos_directories(emuiiboFolder, &allAmiibos);
        allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/miis"), allAmiibos.end());
		allAmiibos.shrink_to_fit();
		std::sort(allAmiibos.begin(), allAmiibos.end(), &utils::NoPathSort);
        std::ifstream settingsIfs(settingsPath);
        if(!settingsIfs.good()){ //no settings.txt found, generate.
			this->bootLayout->SetText(lang::GetDictionaryEntry(13));
           	std::ofstream settingsOfs(settingsPath);
			settingsOfs << "[ALL]" << "\r" << "\n";
            for (auto & elem : allAmiibos) {
				progress++;
                settingsOfs << elem.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
				this->bootLayout->SetProgress((progress/float(allAmiibos.size()))*100.0f);
				this->CallForRender();
            }
            if(settingsOfs.is_open())
                settingsOfs.close();
			//set::Settings *s = new set::Settings(settingsPath);
            //this->SetSettings(s);
			std::vector<std::string>().swap(allAmiibos);
        } else { // settings.txt available compare with actually available amiibos and rewrite
			this->bootLayout->SetText(lang::GetDictionaryEntry(14));
           	if(settingsIfs.is_open())
                settingsIfs.close();
			//set::Settings *s = new set::Settings(settingsPath);
            //this->SetSettings(s);
            std::vector<amiibo::Game*> gamesInConfig = set::GetGames();
            std::vector<std::string> amiibosInConfig;
            std::vector<std::string> removedAmiibos;
			bool all=false;
            for(auto & game : gamesInConfig) {
				if (game->GetName()=="ALL"){
					all=true;
				} else {
					std::vector<std::string> files = game->GetAmiibos();
					for (auto element : files) {
						if(find(amiibosInConfig.begin(), amiibosInConfig.end(), std::string(element)) == amiibosInConfig.end())
							amiibosInConfig.insert(amiibosInConfig.end(), element);
					}
					std::vector<std::string>().swap(files);
				}
            }
			this->bootLayout->SetProgress(45.0f);
			this->CallForRender();
			std::sort(amiibosInConfig.begin(), amiibosInConfig.end(), &utils::NoPathSort);
			
			this->bootLayout->SetProgress(50.0f);
			progress=50.0f;
			this->CallForRender();
		    for(auto & element : amiibosInConfig){
                if(find(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element) != allAmiibos.end()){
                    allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element), allAmiibos.end());
					this->bootLayout->SetProgress(progress + (progress/float(amiibosInConfig.size())*25.0f));
					this->CallForRender();
                } else {
                    removedAmiibos.insert(removedAmiibos.end(), element);
                }
            }
			allAmiibos.shrink_to_fit();

			this->bootLayout->SetProgress(75.0f);
			progress=75.0f;
			this->CallForRender();
		    std::ofstream settingsOfs(settingsPath,std::ofstream::trunc);
			
			if(!all) {
				settingsOfs << "[ALL]" << "\r" << "\n";
				for(auto & element : allAmiibos){
						settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
						this->bootLayout->SetProgress(progress + (progress/float(amiibosInConfig.size())*10.0f));
				}
			}

			this->bootLayout->SetProgress(85.0f);
			progress=85.0f;
			this->CallForRender();
		    for(auto & game : gamesInConfig) {
				if (game->GetName() == "ALL"){
					settingsOfs << "[ALL]" << "\r" << "\n";
					for(auto & element : allAmiibos){
						settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
					}
				} else {
					std::vector<std::string> files = game->GetAmiibos();
					settingsOfs << "[" << game->GetName() <<"]" << "\r" << "\n";
					for (auto element : files) {
						if(find(removedAmiibos.begin(), removedAmiibos.end(), std::string(element)) == removedAmiibos.end()) // missing amiibos are already ignored when creating menu but let's keep settings clean
							settingsOfs << element << "\r" << "\n";
					}
					std::vector<std::string>().swap(files);
				}
				this->bootLayout->SetProgress(progress + (progress/float(amiibosInConfig.size())*10.0f));
            }

			this->bootLayout->SetProgress(95.0f);
			this->CallForRender();
		   	if(settingsOfs.is_open())
                settingsOfs.close();
			//s = new set::Settings(settingsPath);
            //this->SetSettings(s);
		std::vector<amiibo::Game*>().swap(gamesInConfig);
		std::vector<std::string>().swap(amiibosInConfig);
		std::vector<std::string>().swap(removedAmiibos);
		std::vector<std::string>().swap(allAmiibos);
        }
		this->bootLayout->SetProgress(97.5f);
		this->CallForRender();
	}
	*/
	/*
	void MainApplication::UpdateSettings()
	{
        std::string settingsPath = "sdmc:/switch/AmiiSwap/settings.txt";
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> allAmiibos;
        utils::get_amiibos_directories(emuiiboFolder, &allAmiibos);
        allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/miis"), allAmiibos.end());
		allAmiibos.shrink_to_fit();
		std::sort(allAmiibos.begin(), allAmiibos.end(), &utils::NoPathSort);
		std::vector<amiibo::Game*> gamesInConfig = this->gamesLayout->GetAmiiboGames();
		std::vector<std::string> amiibosInConfig;
		std::vector<std::string> removedAmiibos;
		for(auto & game : gamesInConfig) {
			std::vector<std::string> files = game->GetAmiibos();
			for (auto element : files) {
				if(find(amiibosInConfig.begin(), amiibosInConfig.end(), std::string(element)) == amiibosInConfig.end())
					amiibosInConfig.insert(amiibosInConfig.end(), element);
			}
			std::vector<std::string>().swap(files);
		}
		std::sort(amiibosInConfig.begin(), amiibosInConfig.end(), &utils::NoPathSort);

		for(auto & element : amiibosInConfig){
			if(find(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element) != allAmiibos.end()){
				allAmiibos.erase(std::remove(allAmiibos.begin(), allAmiibos.end(), std::string(emuiiboFolder) + "/" + element), allAmiibos.end());
			} else {
				removedAmiibos.insert(removedAmiibos.end(), element);
			}
		}
		allAmiibos.shrink_to_fit();
		std::ofstream settingsOfs(settingsPath,std::ofstream::trunc);
		for(auto & game : gamesInConfig) {
			settingsOfs << "[" << game->GetName() <<"]" << "\r" << "\n";
			if (game->GetName() == "ALL"){
				for(auto & element : allAmiibos){
					settingsOfs << element.substr(sizeof(emuiiboFolder)) << "\r" << "\n";
				}
			} else {
				std::vector<std::string> files = game->GetAmiibos();
				for (auto element : files) {
					if(find(removedAmiibos.begin(), removedAmiibos.end(), element) == removedAmiibos.end()) // missing amiibos are already ignored when creating menu but let's keep settings clean
						settingsOfs << element << "\r" << "\n";
				}
				std::vector<std::string>().swap(files);
			}
		}

		if(settingsOfs.is_open())
			settingsOfs.close();
		//set::Settings *s = new set::Settings(settingsPath);
        //    this->SetSettings(s);
		std::vector<amiibo::Game*>().swap(gamesInConfig);
		std::vector<std::string>().swap(amiibosInConfig);
		std::vector<std::string>().swap(removedAmiibos);
		std::vector<std::string>().swap(allAmiibos);
	}
	*/
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
					this->emuiiboText->SetText(lang::GetDictionaryEntry(15));
					this->emuiiboLed->SetColor({255,0,0,255});
					break;
				case NfpEmuToggleStatus_On:
					this->emuiiboText->SetText(lang::GetDictionaryEntry(16));
					this->emuiiboLed->SetColor({0,255,0,255});
					break;
				case NfpEmuToggleStatus_Once:
					this->emuiiboText->SetText(lang::GetDictionaryEntry(17));
					this->emuiiboLed->SetColor({255,128,0,255});
					break;
				default:
					this->emuiiboText->SetText(lang::GetDictionaryEntry(18));
					this->emuiiboLed->SetColor({0,102,153,255});
					break;
			}
			this->activeAmiibo = utils::getActiveAmiibo();
			this->amiiboText->SetText(lang::GetDictionaryEntry(19) + this->activeAmiibo + " ");
		}
	}

	void MainApplication::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if(Down & KEY_PLUS) this->Close();
    }
}
