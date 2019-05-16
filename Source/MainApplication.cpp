#include <MainApplication.hpp>

namespace ui
{
	MainApplication *mainapp;

	void Initialize()
	{
		//if (R_FAILED(nfpemuInitialize())) exit(1);
	}

	void Finalize()
	{
		nfpemuExit();
	}

	MainApplication::MainApplication() : pu::Application()
	{
		if (!utils::IsEmuiiboPresent()) {
            this->ShowError("Emuiibo is not running on this console, please install it before using AmiiSwap");
		} else {
			nfpemuInitialize();
        	utils::EnsureDirectories(); 	
			utils::InitSettings();
			
			this->header = new pu::element::Rectangle(0, 0, 1280, 80, {0,102,153,255});
			
			this->footer = new pu::element::Rectangle(0, 680, 1280, 40, {0,102,153,255});
			
			this->logo = new pu::element::Image(10, 10, utils::GetRomFsResource("Common/logo.png"));
			this->logo->SetHeight(60);
			this->logo->SetWidth(60);
			
			this->headerText = new pu::element::TextBlock(640, 15, "AmiiSwap", 40);
			this->headerText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);
			this->headerText->SetColor({255,255,255,255});
			
			this->footerText = new pu::element::TextBlock(10, 690, "-", 20);
			this->footerText->SetColor({255,255,255,255});
			
			this->mainLayout = new MainLayout();
			this->mainLayout->Add(this->header);
			this->mainLayout->Add(this->footer);
			this->mainLayout->Add(this->logo);
			this->mainLayout->Add(this->headerText);
			this->mainLayout->Add(this->footerText);
			
			this->setLayout = new SettingsLayout();
			this->setLayout->Add(this->header);
			this->setLayout->Add(this->footer);
			this->setLayout->Add(this->logo);
			this->setLayout->Add(this->headerText);
			this->setLayout->Add(this->footerText);
			
			this->errorLayout = new ErrorLayout();
			this->errorLayout->Add(this->header);
			this->errorLayout->Add(this->footer);
			this->errorLayout->Add(this->logo);
			this->errorLayout->Add(this->headerText);
			this->errorLayout->Add(this->footerText);
			
			this->mainLayout->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
			{
				if(Down & KEY_PLUS) this->Close();
				else if (Down & KEY_B && !this->waitBack) {
					this->mainLayout->GetGamesMenu()->SetVisible(true);
					this->mainLayout->SetElementOnFocus(this->mainLayout->GetGamesMenu());
					this->mainLayout->GetAmiiboMenu()->SetVisible(false);
				} else if (Down & KEY_MINUS) {
					this->LoadLayout(this->setLayout);
				}
			});
			this->LoadLayout(this->mainLayout);
			GetMainLayout()->populateGamesMenu();
		}
	}

    MainApplication::~MainApplication()
    {
		delete this->mainLayout;
		delete this->setLayout;
		delete this->errorLayout;
		delete this->logo;
		delete this->header;
		delete this->headerText;
		delete this->footer;
		delete this->footerText;
	}

	void MainApplication::SetWaitBack(bool state)
	{
		this->waitBack = state;
	}

	void SetMainApplication(MainApplication *MainApp)
	{
		mainapp = MainApp;
	}

	void MainApplication::SetFooterText(std::string Text)
	{
		//TODO doesn't work. make it work and add SetHeaderText
		if(this->footerText != NULL) this->footerText->SetText(Text);
	}

	void MainApplication::ShowError(std::string text)
	{
		this->errorLayout->SetText(text);
		this->LoadLayout(this->errorLayout);
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
}
