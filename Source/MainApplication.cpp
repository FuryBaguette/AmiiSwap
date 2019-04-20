#include <MainApplication.hpp>

namespace ui
{
	MainApplication *mainapp;

	MainApplication::MainApplication()
	{
	    this->mainLayout = new MainLayout();
	    this->LoadLayout(this->mainLayout);
		this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
	    {
	        if(Down & KEY_PLUS) this->Close();
			else if (Down & KEY_B && !this->waitBack) {
				this->mainLayout->GetGamesMenu()->SetVisible(true);
				this->mainLayout->SetElementOnFocus(this->mainLayout->GetGamesMenu());
				this->mainLayout->GetAmiiboMenu()->SetVisible(false);
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
