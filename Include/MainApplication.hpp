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
#include "MainLayout.hpp"

namespace ui
{
	inline char* getExtension(const char* str)
	{
		const char* p;
		for (p = str+strlen(str); p >= str && *p != '.'; p--);
		return (char*)p;
	}

	inline bool fileExists (const std::string& name) {
	  struct stat buffer;
	  return (stat (name.c_str(), &buffer) == 0);
	}

	class MainApplication : public pu::Application
	{
	    public:
	        MainApplication();
			void SetWaitBack(bool state);
	    private:
	        ui::MainLayout *mainLayout;
			bool waitBack = false;
	};

	void SetMainApplication(MainApplication *MainApp);
	void Initialize();
	void Finalize();
}
