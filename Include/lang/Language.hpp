#pragma once
#include <switch.h>

namespace lang
{
	struct Language
	{
		std::string langCode = "en";
		json Strings;
	};
	
	void Initialize();
	static Language MainLanguage;
	std::string GetDictionaryEntry(u32 Index);
	std::string GetLangCode();
}
