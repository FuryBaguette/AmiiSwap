#include "MainApplication.hpp"

namespace lang
{
	void Initialize()
	{
		u64 lcode = 0;
		s32 lang = 0;
		setGetSystemLanguage(&lcode);
		setMakeLanguage(lcode, &lang);
		std::string lstring;
		switch (lang)
		{
		case SetLanguage_ENUS:
		case SetLanguage_ENGB:
			lstring = "en";
			break;
		case SetLanguage_IT:
			lstring = "it";
			break;
		case SetLanguage_FR:
		case SetLanguage_FRCA:
			lstring = "fr";
			break;
		case SetLanguage_JA:
		case SetLanguage_DE:
		case SetLanguage_ES:
		case SetLanguage_ZHCN:
		case SetLanguage_KO:
		case SetLanguage_NL:
		case SetLanguage_PT:
		case SetLanguage_RU:
		case SetLanguage_ZHTW:
		case SetLanguage_ES419:
		default:
			lstring = "en";
			break;
		}
		MainLanguage.langCode = lstring;
		lstring += ".json";
		std::ifstream ifs(utils::GetRomFsResource("Language/" + lstring));
		MainLanguage.Strings = json::parse(ifs);
		ifs.close();
	}

	std::string GetDictionaryEntry(u32 Index)
	{
		return MainLanguage.Strings[Index].get<std::string>();
	}

	std::string GetLangCode()
	{
		return MainLanguage.langCode;
	}
}