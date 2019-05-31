#pragma once

#include <pu/Plutonium>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <filesystem>
#include "nfpemu.h"
#include "Amiibo.hpp"
#include "Utils.hpp"
#include "Settings.hpp"
#include "MainLayout.hpp"
#include "ManageLayout.hpp"
#include "SettingsLayout.hpp"
#include "ErrorLayout.hpp"
#include "AboutLayout.hpp"
#include "EmuiiboLayout.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace ui
{
	inline char* getExtension(const char* str)
	{
		const char* p;
		for (p = str+strlen(str); p >= str && *p != '.'; p--);
		return (char*)p;
	}

	class MainApplication : public pu::Application
	{
	    public:
	        MainApplication();
			~MainApplication();
			MainLayout *GetMainLayout();
			ManageLayout *GetManageLayout();
			SettingsLayout *GetSettingsLayout();
			ErrorLayout *GetErrorLayout();
			AboutLayout *GetAboutLayout();
			EmuiiboLayout *GetEmuiiboLayout();
			set::Settings *GetSettings();
			void SetSettings(set::Settings *s);
			void SetWaitBack(bool state);
			bool GetWaitBack();
			void SetFooterText(std::string Text);
			void SetHelpText(std::string Text);
			void ShowError(std::string text);
			void InitSettings();
			void UpdateSettings();
			void OnInput(u64 Down, u64 Up, u64 Held);
			int MainApplication::GetEmuiiboStatus();
			void MainApplication::UpdateEmuiiboStatus();
			std::string activeAmiibo;
	    private:
			set::Settings *settings;
	        ui::MainLayout *mainLayout;
	        ui::ManageLayout *manageLayout;
			ui::SettingsLayout *setLayout;
			ui::ErrorLayout *errorLayout;
			ui::AboutLayout *aboutLayout;
			ui::EmuiiboLayout *emuiiboLayout;
			bool waitBack = false;
			pu::element::Image *logo;
			pu::element::Rectangle *header;
			pu::element::Rectangle *headerShadow;
			pu::element::Rectangle *footer;
			pu::element::Rectangle *footerShadow;
			pu::element::Rectangle *emuiiboLed;
			pu::element::TextBlock *headerText;
			pu::element::TextBlock *emuiiboText;
			pu::element::TextBlock *amiiboText;
			pu::element::TextBlock *footerText;
			pu::element::TextBlock *helpText;
			//NfpEmuToggleStatus *nfpStatus;
			std::chrono::time_point<std::chrono::steady_clock> start;
	};

	void SetMainApplication(MainApplication *MainApp);
	void Initialize();
	void Finalize();
}
