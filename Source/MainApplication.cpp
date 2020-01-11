#include <MainApplication.hpp>
#include <iostream>
#include <fstream>
//extern char *fake_heap_end;
extern lang::Language *language;
extern set::Settings *settings;


namespace ui
{
	MainApplication::Ref mainapp;
	/*static void *ghaddr;

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
	}*/

	void MainApplication::OnLoad()
	{
		utils::EnsureDirectories();
		lang::Initialize();
		this->bootLayout = BootLayout::New();
		this->LoadLayout(this->bootLayout);
		this->CallForRender();

		this->header = pu::ui::elm::Rectangle::New(0, 0, 1280, 79, pu::ui::Color(0,102,153,255));
		this->footer = pu::ui::elm::Rectangle::New(0, 681, 1280, 39, pu::ui::Color(0,102,153,255));
		this->headerShadow = pu::ui::elm::Rectangle::New(0, 79, 1280, 1, pu::ui::Color(0,51,102,255));
		this->footerShadow = pu::ui::elm::Rectangle::New(0, 680, 1280, 1, pu::ui::Color(0,51,102,255));
		this->logo = pu::ui::elm::Image::New(10, 10, utils::GetRomFsResource("Common/logo.png"));
		this->logo->SetHeight(60);
		this->logo->SetWidth(60);

		this->headerText = pu::ui::elm::TextBlock::New(80, 20, lang::GetLabel(lang::Label::APP_NAME) + std::string(AMIISWAP_VERSION), 40);
		this->headerText->SetColor(pu::ui::Color(255,255,255,255));

		this->emuiiboLed = pu::ui::elm::Rectangle::New(1255, 10, 15, 15, pu::ui::Color(0,102,153,255), 5);
		this->emuiiboLed->SetBorderRadius(5);

		this->emuiiboText = pu::ui::elm::TextBlock::New(80, 30, "", 20);
		this->emuiiboText->SetColor(pu::ui::Color(255,255,255,255));
		this->emuiiboText->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Right);

		this->amiiboText = pu::ui::elm::TextBlock::New(80, 55, "", 20);
		this->amiiboText->SetColor(pu::ui::Color(255,255,255,255));
		this->amiiboText->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Right);

		this->footerText = pu::ui::elm::TextBlock::New(10, 690, lang::GetLabel(lang::Label::FOOTER_MAIN_AMIIBO), 20);
		this->footerText->SetColor(pu::ui::Color(255,255,255,255));

		this->helpText = pu::ui::elm::TextBlock::New(10, 690, "", 20);
		this->helpText->SetColor(pu::ui::Color(255,255,255,255));
		this->helpText->SetHorizontalAlign(pu::ui::elm::HorizontalAlign::Right);
		this->helpText->SetText(lang::GetLabel(lang::Label::HELP_SELECT));

		this->bootLayout->SetProgress(3.0f);
		this->CallForRender();

		this->errorLayout = ErrorLayout::New();
		this->errorLayout->Add(this->header);
		this->errorLayout->Add(this->footer);
		this->errorLayout->Add(this->logo);
		this->errorLayout->Add(this->headerText);
		this->errorLayout->Add(this->footerText);
		this->errorLayout->Add(this->helpText);
		this->bootLayout->SetProgress(6.0f);
		this->CallForRender();

		this->gamesLayout = GamesLayout::New();
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

		this->amiibosLayout = AmiibosLayout::New();
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

		this->emuiiboLayout = EmuiiboLayout::New();
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

		this->imagesLayout = ImagesLayout::New();
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

		this->setLayout = SettingsLayout::New();
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

		this->manualLayout = ManualLayout::New();
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

		this->aboutLayout = AboutLayout::New();
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

		this->amiiboDetailsLayout = AmiiboDetailsLayout::New();
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

		this->mainLayout = MainLayout::New();
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
		
		this->bootLayout->SetText(lang::GetLabel(lang::Label::BOOT_SETTINGS));
		this->bootLayout->SetProgress(36.0f);
		this->CallForRender();
		set::Initialize("sdmc:/switch/AmiiSwap/settings.txt");
		this->bootLayout->SetProgress(90.0f);
		this->CallForRender();
		this->gamesLayout->populateGamesMenu();
		this->bootLayout->SetProgress(100.0f);
		this->CallForRender();
		if (!utils::IsEmuiiboPresent()) {
			this->header->SetColor(pu::ui::Color(204,0,0,255));
			this->footer->SetColor(pu::ui::Color(204,0,0,255));
			this->emuiiboText->SetText(lang::GetLabel(lang::Label::ERROR_NOT_INSTALLED));
			this->LoadLayout(this->mainLayout);
		} else {
			nfpemuInitialize();
			this->AddThread(std::bind(&MainApplication::UpdateEmuiiboStatus, this));
			this->LoadLayout(this->mainLayout);
			this->start = std::chrono::steady_clock::now();
		}
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
	void SetMainApplication(MainApplication::Ref MainApp)
	{
		mainapp = MainApp;
	}

	void MainApplication::SetFooterText(pu::String Text)
	{
		if(this->footerText != NULL) this->footerText->SetText(Text);
	}

	void MainApplication::SetHelpText(std::string Text)
	{
		if(this->helpText != NULL) this->helpText->SetText(Text);
	}

	void MainApplication::ShowError(std::string text)
	{
		this->header->SetColor(pu::ui::Color(204,0,0,255));
		this->footer->SetColor(pu::ui::Color(204,0,0,255));
		this->errorLayout->SetText(text);
	}

	AboutLayout::Ref &MainApplication::GetAboutLayout()
    {
        return this->aboutLayout;
    }

	GamesLayout::Ref &MainApplication::GetGamesLayout()
    {
        return this->gamesLayout;
    }

	MainLayout::Ref &MainApplication::GetMainLayout()
    {
        return this->mainLayout;
    }

	SettingsLayout::Ref &MainApplication::GetSettingsLayout()
    {
        return this->setLayout;
    }

	ErrorLayout::Ref &MainApplication::GetErrorLayout()
    {
        return this->errorLayout;
    }

	EmuiiboLayout::Ref &MainApplication::GetEmuiiboLayout()
    {
        return this->emuiiboLayout;
    }

	ImagesLayout::Ref &MainApplication::GetImagesLayout()
    {
        return this->imagesLayout;
    }

	ManualLayout::Ref &MainApplication::GetManualLayout()
    {
        return this->manualLayout;
    }

	BootLayout::Ref &MainApplication::GetBootLayout()
    {
        return this->bootLayout;
    }

	AmiiboDetailsLayout::Ref &MainApplication::GetAmiiboDetailsLayout()
    {
        return this->amiiboDetailsLayout;
    }

	AmiibosLayout::Ref &MainApplication::GetAmiibosLayout()
    {
        return this->amiibosLayout;
    }

	int MainApplication::GetEmuiiboStatus()
	{
		EmuEmulationStatus nfpStatus;
		std::string status = "";
		Result rc = nfpemuGetStatus(&nfpStatus);
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
				case EmuEmulationStatus_Off:
					this->emuiiboText->SetText(lang::GetLabel(lang::Label::HEADER_EMUIIBO_NOT_ACTIVE));
					this->emuiiboLed->SetColor(pu::ui::Color(255,0,0,255));
					break;
				case EmuEmulationStatus_OnForever:
					this->emuiiboText->SetText(lang::GetLabel(lang::Label::HEADER_EMUIIBO_ACTIVE));
					this->emuiiboLed->SetColor(pu::ui::Color(0,255,0,255));
					break;
				case EmuEmulationStatus_OnOnce:
					this->emuiiboText->SetText(lang::GetLabel(lang::Label::HEADER_EMUIIBO_ACTIVE_ONCE));
					this->emuiiboLed->SetColor(pu::ui::Color(255,128,0,255));
					break;
				default:
					this->emuiiboText->SetText(lang::GetLabel(lang::Label::HEADER_EMUIIBO_UNKNOWN));
					this->emuiiboLed->SetColor(pu::ui::Color(0,102,153,255));
					break;
			}
			this->activeAmiibo = utils::getActiveAmiibo();
			this->amiiboText->SetText(utils::replace(lang::GetLabel(lang::Label::HEADER_AMIIBO), "{{AMIIBO_NAME}}", this->activeAmiibo) + " ");
		}
	}

	void MainApplication::OnInput(u64 Down, u64 Up, u64 Held)
    {
        if(Down & KEY_PLUS) this->Close();
    }
}
