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
		MainLanguage.Labels = json::parse(ifs);
		ifs.close();

		InitLabelMap();
	}

	void InitLabelMap()
	{
		MainLanguage.labelMap[Label::ABOUT_AUTHOR] = "ABOUT_AUTHOR";
		MainLanguage.labelMap[Label::ABOUT_CREDITS] = "ABOUT_CREDITS";
		MainLanguage.labelMap[Label::ABOUT_DEVS] = "ABOUT_DEVS";
		MainLanguage.labelMap[Label::ABOUT_TESTERS] = "ABOUT_TESTERS";
		MainLanguage.labelMap[Label::AMIIBO_DETAILS_FIRST_DATE] = "AMIIBO_DETAILS_FIRST_DATE";
		MainLanguage.labelMap[Label::AMIIBO_DETAILS_LAST_DATE] = "AMIIBO_DETAILS_LAST_DATE";
		MainLanguage.labelMap[Label::AMIIBO_DETAILS_PATH] = "AMIIBO_DETAILS_PATH";
		MainLanguage.labelMap[Label::APP_NAME] = "APP_NAME";
		MainLanguage.labelMap[Label::BOOT_CREATING] = "BOOT_CREATING";
		MainLanguage.labelMap[Label::BOOT_LOADING] = "BOOT_LOADING";
		MainLanguage.labelMap[Label::BOOT_SETTINGS] = "BOOT_SETTINGS";
		MainLanguage.labelMap[Label::BOOT_UPDATING] = "BOOT_UPDATING";
		MainLanguage.labelMap[Label::CANCEL] = "CANCEL";
		MainLanguage.labelMap[Label::DISABLED] = "DISABLED";
		MainLanguage.labelMap[Label::EMUIIBO_DISABLE] = "EMUIIBO_DISABLE";
		MainLanguage.labelMap[Label::EMUIIBO_ENABLE] = "EMUIIBO_ENABLE";
		MainLanguage.labelMap[Label::EMUIIBO_ENABLE_ONCE] = "EMUIIBO_ENABLE_ONCE";
		MainLanguage.labelMap[Label::EMUIIBO_SCAN] = "EMUIIBO_SCAN";
		MainLanguage.labelMap[Label::ENABLED] = "ENABLED";
		MainLanguage.labelMap[Label::ERROR_NOT_INSTALLED] = "ERROR_NOT_INSTALLED";
		MainLanguage.labelMap[Label::FOOTER_ABOUT] = "FOOTER_ABOUT";
		MainLanguage.labelMap[Label::FOOTER_AMIIBO] = "FOOTER_AMIIBO";
		MainLanguage.labelMap[Label::FOOTER_AMIIBO_SELECT] = "FOOTER_AMIIBO_SELECT";
		MainLanguage.labelMap[Label::FOOTER_EMUIIBO_DISABLE] = "FOOTER_EMUIIBO_DISABLE";
		MainLanguage.labelMap[Label::FOOTER_EMUIIBO_ENABLE] = "FOOTER_EMUIIBO_ENABLE";
		MainLanguage.labelMap[Label::FOOTER_EMUIIBO_ENABLE_ONCE] = "FOOTER_EMUIIBO_ENABLE_ONCE";
		MainLanguage.labelMap[Label::FOOTER_EMUIIBO_SCAN] = "FOOTER_EMUIIBO_SCAN";
		MainLanguage.labelMap[Label::FOOTER_GAMES] = "FOOTER_GAMES";
		MainLanguage.labelMap[Label::FOOTER_IMAGES_DELETE] = "FOOTER_IMAGES_DELETE";
		MainLanguage.labelMap[Label::FOOTER_IMAGES_RENAME] = "FOOTER_IMAGES_RENAME";
		MainLanguage.labelMap[Label::FOOTER_IMAGES_SEARCH] = "FOOTER_IMAGES_SEARCH";
		MainLanguage.labelMap[Label::FOOTER_MAIN_ABOUT] = "FOOTER_MAIN_ABOUT";
		MainLanguage.labelMap[Label::FOOTER_MAIN_AMIIBO] = "FOOTER_MAIN_AMIIBO";
		MainLanguage.labelMap[Label::FOOTER_MAIN_EMUIIBO] = "FOOTER_MAIN_EMUIIBO";
		MainLanguage.labelMap[Label::FOOTER_MAIN_IMAGES] = "FOOTER_MAIN_IMAGES";
		MainLanguage.labelMap[Label::FOOTER_MAIN_SELECTED_AMIIBO] = "FOOTER_MAIN_SELECTED_AMIIBO";
		MainLanguage.labelMap[Label::FOOTER_MAIN_USER_MANUAL] = "FOOTER_MAIN_USER_MANUAL";
		MainLanguage.labelMap[Label::FOOTER_SELECTED_AMIIBO] = "FOOTER_SELECTED_AMIIBO";
		MainLanguage.labelMap[Label::HEADER_AMIIBO] = "HEADER_AMIIBO";
		MainLanguage.labelMap[Label::HEADER_EMUIIBO_ACTIVE] = "HEADER_EMUIIBO_ACTIVE";
		MainLanguage.labelMap[Label::HEADER_EMUIIBO_ACTIVE_ONCE] = "HEADER_EMUIIBO_ACTIVE_ONCE";
		MainLanguage.labelMap[Label::HEADER_EMUIIBO_NOT_ACTIVE] = "HEADER_EMUIIBO_NOT_ACTIVE";
		MainLanguage.labelMap[Label::HEADER_EMUIIBO_UNKNOWN] = "HEADER_EMUIIBO_UNKNOWN";
		MainLanguage.labelMap[Label::HEADER_NO_AMIIBO] = "HEADER_NO_AMIIBO";
		MainLanguage.labelMap[Label::HELP_ADD] = "HELP_ADD";
		MainLanguage.labelMap[Label::HELP_BACK] = "HELP_BACK";
		MainLanguage.labelMap[Label::HELP_FINISH] = "HELP_FINISH";
		MainLanguage.labelMap[Label::HELP_MANAGE] = "HELP_MANAGE";
		MainLanguage.labelMap[Label::HELP_MINUS] = "HELP_MINUS";
		MainLanguage.labelMap[Label::HELP_OK] = "HELP_OK";
		MainLanguage.labelMap[Label::HELP_SELECT] = "HELP_SELECT";
		MainLanguage.labelMap[Label::HELP_TOGGLE] = "HELP_TOGGLE";
		MainLanguage.labelMap[Label::IMAGES_DELETE] = "IMAGES_DELETE";
		MainLanguage.labelMap[Label::IMAGES_RENAME] = "IMAGES_RENAME";
		MainLanguage.labelMap[Label::IMAGES_SEARCH] = "IMAGES_SEARCH";
		MainLanguage.labelMap[Label::KEYBOARD_HINT] = "KEYBOARD_HINT";
		MainLanguage.labelMap[Label::MAIN_ABOUT] = "MAIN_ABOUT";
		MainLanguage.labelMap[Label::MAIN_AMIIBO] = "MAIN_AMIIBO";
		MainLanguage.labelMap[Label::MAIN_EMUIIBO] = "MAIN_EMUIIBO";
		MainLanguage.labelMap[Label::MAIN_IMAGES] = "MAIN_IMAGES";
		MainLanguage.labelMap[Label::MAIN_SELECTED_AMIIBO] = "MAIN_SELECTED_AMIIBO";
		MainLanguage.labelMap[Label::MAIN_SETTINGS] = "MAIN_SETTINGS";
		MainLanguage.labelMap[Label::MAIN_USER_MANUAL] = "MAIN_USER_MANUAL";
		MainLanguage.labelMap[Label::NO] = "NO";
		MainLanguage.labelMap[Label::RANDOM_DIALOG_TEXT] = "RANDOM_DIALOG_TEXT";
		MainLanguage.labelMap[Label::RANDOM_DIALOG_TITLE] = "RANDOM_DIALOG_TITLE";
		MainLanguage.labelMap[Label::RANDOM_UUID] = "RANDOM_UUID";
		MainLanguage.labelMap[Label::SELECT_DIALOG_TEXT] = "SELECT_DIALOG_TEXT";
		MainLanguage.labelMap[Label::SELECT_DIALOG_TITLE] = "SELECT_DIALOG_TITLE";
		MainLanguage.labelMap[Label::TOAST_ACTIVE_AMIIBO] = "TOAST_ACTIVE_AMIIBO";
		MainLanguage.labelMap[Label::TOAST_AMIIBO_ADDED] = "TOAST_AMIIBO_ADDED";
		MainLanguage.labelMap[Label::TOAST_AMIIBO_REMOVED] = "TOAST_AMIIBO_REMOVED";
		MainLanguage.labelMap[Label::TOAST_EMUIIBO_DISABLED] = "TOAST_EMUIIBO_DISABLED";
		MainLanguage.labelMap[Label::TOAST_EMUIIBO_ENABLED] = "TOAST_EMUIIBO_ENABLED";
		MainLanguage.labelMap[Label::TOAST_EMUIIBO_ENABLED_ONCE] = "TOAST_EMUIIBO_ENABLED_ONCE";
		MainLanguage.labelMap[Label::TOAST_EMUIIBO_NOT_INSTALLED] = "TOAST_EMUIIBO_NOT_INSTALLED";
		MainLanguage.labelMap[Label::TOAST_EMUIIBO_SCAN] = "TOAST_EMUIIBO_SCAN";
		MainLanguage.labelMap[Label::TOAST_GAMES_NO_AMIIBO] = "TOAST_GAMES_NO_AMIIBO";
		MainLanguage.labelMap[Label::TOAST_GAME_ADDED] = "TOAST_GAME_ADDED";
		MainLanguage.labelMap[Label::TOAST_GAME_ALL_CANT_DELETE] = "TOAST_GAME_ALL_CANT_DELETE";
		MainLanguage.labelMap[Label::TOAST_GAME_ALL_POPULATED] = "TOAST_GAME_ALL_POPULATED";
		MainLanguage.labelMap[Label::TOAST_GAME_ALL_RESERVED] = "TOAST_GAME_ALL_RESERVED";
		MainLanguage.labelMap[Label::TOAST_GAME_REMOVED] = "TOAST_GAME_REMOVED";
		MainLanguage.labelMap[Label::TOAST_IMAGES_DELETED] = "TOAST_IMAGES_DELETED";
		MainLanguage.labelMap[Label::TOAST_IMAGES_FOUND] = "TOAST_IMAGES_FOUND";
		MainLanguage.labelMap[Label::TOAST_IMAGES_IMAGES] = "TOAST_IMAGES_IMAGES";
		MainLanguage.labelMap[Label::TOAST_IMAGES_RENAMED] = "TOAST_IMAGES_RENAMED";
		MainLanguage.labelMap[Label::TOAST_NO_AMIIBO] = "TOAST_NO_AMIIBO";
		MainLanguage.labelMap[Label::TOAST_RANDOM] = "TOAST_RANDOM";
		MainLanguage.labelMap[Label::TOGGLE] = "TOGGLE";
		MainLanguage.labelMap[Label::YES] = "YES";
	}

	std::string GetLabel(Label label)
	{
		return MainLanguage.Labels[MainLanguage.labelMap[label]].get<std::string>();
	}

	std::string GetLangCode()
	{
		return MainLanguage.langCode;
	}
}