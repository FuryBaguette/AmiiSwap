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
#include "SettingsLayout.hpp"

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
			void SetWaitBack(bool state);
	    private:
	        ui::MainLayout *mainLayout;
			ui::SettingsLayout *setLayout;
			bool waitBack = false;
	};

	void SetMainApplication(MainApplication *MainApp);
	void Initialize();
	void Finalize();
}
