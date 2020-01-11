#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    AmiiboDetailsLayout::AmiiboDetailsLayout()
    {
        this->amiiboImage = pu::ui::elm::Image::New(10, 90, utils::GetRomFsResource("Common/logo.png"));
        this->amiiboImage->SetWidth(200);
        this->amiiboImage->SetHeight(200);
        this->Add(this->amiiboImage);

        this->amiiboName = pu::ui::elm::TextBlock::New(220, 260, "", 30);
        this->Add(this->amiiboName);

        this->firstWrite = pu::ui::elm::TextBlock::New(10, 310, "", 20);
        this->Add(this->firstWrite);

        this->lastWrite = pu::ui::elm::TextBlock::New(10, 350, "", 20);
        this->Add(this->lastWrite);

        this->amiiboPath = pu::ui::elm::TextBlock::New(10, 390, "", 20);
        this->Add(this->amiiboPath);

        this->amiiboRandom = pu::ui::elm::TextBlock::New(10, 430, "", 20);
        this->Add(this->amiiboRandom);

        this->SetOnInput(std::bind(&AmiiboDetailsLayout::amiiboDetails_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
	
	void AmiiboDetailsLayout::amiiboDetails_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B){
			mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT));
			mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
			//mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
			mainapp->GetMainLayout()->selectionChange();
			mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}

    void AmiiboDetailsLayout::LoadAmiibo(std::string path)
    {
        std::string imagePath = path + "/amiibo.icon";
        this->amiiboImage->SetImage(imagePath);
        std::string jsonPath = path + "/amiibo.json";
        std::ifstream ifs(jsonPath);
        auto amiiboJson = json::parse(ifs);
        if(ifs.is_open())
            ifs.close();
        this->randomuuid = amiiboJson.value<bool>("randomizeUuid", false);
        if(randomuuid)
            this->amiiboRandom->SetText(lang::GetLabel(lang::Label::RANDOM_UUID) + lang::GetLabel(lang::Label::ENABLED));
        else
            this->amiiboRandom->SetText(lang::GetLabel(lang::Label::RANDOM_UUID) + lang::GetLabel(lang::Label::DISABLED));
        this->amiiboName->SetText((std::string)amiiboJson["name"].get<std::string>());
        this->amiiboPath->SetText(lang::GetLabel(lang::Label::AMIIBO_DETAILS_PATH) + path);
        this->firstWrite->SetText(lang::GetLabel(lang::Label::AMIIBO_DETAILS_FIRST_DATE) + std::to_string((u16)amiiboJson["firstWriteDate"][0].get<int>()) + "/" + std::to_string((u8)amiiboJson["firstWriteDate"][1].get<int>()) + "/" + std::to_string((u8)amiiboJson["firstWriteDate"][2].get<int>()));
        this->lastWrite->SetText(lang::GetLabel(lang::Label::AMIIBO_DETAILS_LAST_DATE) + std::to_string((u16)amiiboJson["lastWriteDate"][0].get<int>()) + "/" + std::to_string((u8)amiiboJson["lastWriteDate"][1].get<int>()) + "/" + std::to_string((u8)amiiboJson["lastWriteDate"][2].get<int>()));
    }
}
