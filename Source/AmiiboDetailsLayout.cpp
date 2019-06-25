#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    AmiiboDetailsLayout::AmiiboDetailsLayout() : pu::Layout()
    {
        this->amiiboImage = new pu::element::Image(10, 90, utils::GetRomFsResource("Common/logo.png"));
        this->amiiboImage->SetWidth(200);
        this->amiiboImage->SetHeight(200);
        this->Add(this->amiiboImage);

        this->amiiboName = new pu::element::TextBlock(220, 260, "", 30);
        this->Add(this->amiiboName);

        this->firstWrite = new pu::element::TextBlock(10, 310, "", 20);
        this->Add(this->firstWrite);

        this->lastWrite = new pu::element::TextBlock(10, 350, "", 20);
        this->Add(this->lastWrite);

        this->amiiboPath = new pu::element::TextBlock(10, 390, "", 20);
        this->Add(this->amiiboPath);

        this->amiiboRandom = new pu::element::TextBlock(10, 430, "", 20);
        this->Add(this->amiiboRandom);

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
        {
            if (Down & KEY_B){
                mainapp->SetHelpText(lang::GetDictionaryEntry(2));
                mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
                mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
                mainapp->GetMainLayout()->selectionChange();
                mainapp->LoadLayout(mainapp->GetMainLayout());
            }
        });
    }

    AmiiboDetailsLayout::~AmiiboDetailsLayout()
    {
        delete this->amiiboImage;
		delete this->amiiboName;
		delete this->amiiboPath;
		delete this->firstWrite;
		delete this->lastWrite;
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
            this->amiiboRandom->SetText(lang::GetDictionaryEntry(24) + lang::GetDictionaryEntry(25));
        else
            this->amiiboRandom->SetText(lang::GetDictionaryEntry(24) + lang::GetDictionaryEntry(26));
        this->amiiboName->SetText((std::string)amiiboJson["name"].get<std::string>());
        this->amiiboPath->SetText(lang::GetDictionaryEntry(27) + path);
        this->firstWrite->SetText(lang::GetDictionaryEntry(28) + std::to_string((u16)amiiboJson["firstWriteDate"][0].get<int>()) + "/" + std::to_string((u8)amiiboJson["firstWriteDate"][1].get<int>()) + "/" + std::to_string((u8)amiiboJson["firstWriteDate"][2].get<int>()));
        this->lastWrite->SetText(lang::GetDictionaryEntry(29) + std::to_string((u16)amiiboJson["lastWriteDate"][0].get<int>()) + "/" + std::to_string((u8)amiiboJson["lastWriteDate"][1].get<int>()) + "/" + std::to_string((u8)amiiboJson["lastWriteDate"][2].get<int>()));
        //mainapp->CallForRender();
    }
}
