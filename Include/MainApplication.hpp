#pragma once

#include <pu/Plutonium>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <stdlib.h>
#include <malloc.h>
#include <filesystem>
#include "nfpemu.h"
#include "Amiibo.hpp"
#include "Utils.hpp"
#include "Settings.hpp"
#include "MainLayout.hpp"
#include "GamesLayout.hpp"
#include "SettingsLayout.hpp"
#include "ErrorLayout.hpp"
#include "AboutLayout.hpp"
#include "EmuiiboLayout.hpp"
#include "ImagesLayout.hpp"
#include "ManualLayout.hpp"
#include "BootLayout.hpp"
#include "AmiiboDetailsLayout.hpp"
#include "AmiibosLayout.hpp"
#include "lang/Language.hpp"
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

	class MainApplication : public pu::ui::Application
	{
	    public:
			//MainApplication();
			using Application::Application;
			void OnLoad() override;
			PU_SMART_CTOR(MainApplication)

			// We need to define this, and use it to initialize everything
	        /*MainApplication();
			~MainApplication();*/
			MainLayout::Ref &GetMainLayout();
			GamesLayout::Ref &GetGamesLayout();
			SettingsLayout::Ref &GetSettingsLayout();
			ErrorLayout::Ref &GetErrorLayout();
			AboutLayout::Ref &GetAboutLayout();
			EmuiiboLayout::Ref &GetEmuiiboLayout();
			ImagesLayout::Ref &GetImagesLayout();
			ManualLayout::Ref &GetManualLayout();
			BootLayout::Ref &GetBootLayout();
			AmiiboDetailsLayout::Ref &GetAmiiboDetailsLayout();
			AmiibosLayout::Ref &GetAmiibosLayout();
			void SetFooterText(pu::String Text);
			void SetHelpText(std::string Text);
			void ShowError(std::string text);
			void OnInput(u64 Down, u64 Up, u64 Held);
			int MainApplication::GetEmuiiboStatus();
			void MainApplication::UpdateEmuiiboStatus();
			std::string activeAmiibo;
	    private:
			ui::MainLayout::Ref mainLayout;
	        ui::GamesLayout::Ref gamesLayout;
			ui::SettingsLayout::Ref setLayout;
			ui::ErrorLayout::Ref errorLayout;
			ui::AboutLayout::Ref aboutLayout;
			ui::EmuiiboLayout::Ref emuiiboLayout;
			ui::ImagesLayout::Ref imagesLayout;
			ui::ManualLayout::Ref manualLayout;
			ui::BootLayout::Ref bootLayout;
			ui::AmiiboDetailsLayout::Ref amiiboDetailsLayout;
			ui::AmiibosLayout::Ref amiibosLayout;
			pu::ui::elm::Image::Ref logo;
			pu::ui::elm::Rectangle::Ref header;
			pu::ui::elm::Rectangle::Ref headerShadow;
			pu::ui::elm::Rectangle::Ref footer;
			pu::ui::elm::Rectangle::Ref footerShadow;
			pu::ui::elm::Rectangle::Ref emuiiboLed;
			pu::ui::elm::TextBlock::Ref headerText;
			pu::ui::elm::TextBlock::Ref emuiiboText;
			pu::ui::elm::TextBlock::Ref amiiboText;
			pu::ui::elm::TextBlock::Ref footerText;
			pu::ui::elm::TextBlock::Ref helpText;
			std::chrono::time_point<std::chrono::steady_clock> start;
	};

	void SetMainApplication(MainApplication::Ref MainApp);
	/*void Initialize();
	void Finalize();*/
}
