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

	MainApplication::MainApplication()
	{
		this->mainLayout = new MainLayout();
		this->setLayout = new SettingsLayout();
	    this->LoadLayout(this->mainLayout);
		this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
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
	}

	void MainApplication::SetWaitBack(bool state)
	{
		this->waitBack = state;
	}

	void SetMainApplication(MainApplication *MainApp)
	{
		mainapp = MainApp;
	}
}
