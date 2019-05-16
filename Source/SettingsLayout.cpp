#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    void SettingsLayout::GetAmiibos()
    {
        std::vector<std::string> amiibos = utils::get_directories("sdmc:/emuiibo");
        for (auto & elem : amiibos) {
            std::size_t found = elem.find_last_of("/\\");
			std::string name = elem.substr(found+1);
            if (name != "miis") {
                amiibo::AmiiboFile *file = new amiibo::AmiiboFile(name, elem, elem + "amiibo.png");
                this->amiiboFiles.push_back(file);
            }
        }
    }

    void SettingsLayout::GetAmiiboGames()
    {
        set::Settings *settings = new set::Settings("sdmc:/switch/AmiiSwap/settings.txt");
        this->amiiboGames = settings->GetGames();
    }

    SettingsLayout::SettingsLayout()  : pu::Layout()
    {
        this->GetAmiibos();
        this->GetAmiiboGames();

	    this->amiiboMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
        this->titleText = new pu::element::TextBlock(640, 10, "AmiiSwap Settings");

		for (auto & element : this->amiiboFiles) {
			pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
			item->AddOnClick(std::bind(&SettingsLayout::amiibo_Click, this, element), KEY_A);
			this->amiiboMenu->AddItem(item);
		}

        this->amiiboMenu->SetOnFocusColor({102,153,204,255});
        this->amiiboMenu->SetScrollbarColor({102,153,204,255});
        this->titleText->SetHorizontalAlign(pu::element::HorizontalAlign::Center);

        this->Add(this->titleText);
		this->Add(this->amiiboMenu);
		this->SetElementOnFocus(this->amiiboMenu);
    }

    void SettingsLayout::amiibo_Click(amiibo::AmiiboFile *element)
    {
    	if (!this->waitInput) {
    		mainapp->SetWaitBack(true);
    		int sopt = mainapp->CreateShowDialog(element->GetName(), "What do you want to do with " + element->GetName() + "?\nIt's in the following game(s):\n" + element->DisplayParents(this->amiiboGames), { "Add to game", "Remove from game", "Cancel" }, true, element->GetIconPath());
    		if (sopt == 0) {
                pu::overlay::Toast *toast = new pu::overlay::Toast("Add to game", 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            } else if (sopt == 1) {
                pu::overlay::Toast *toast = new pu::overlay::Toast("Remove from game", 20, {255,255,255,255}, {0,0,0,200});
                mainapp->StartOverlayWithTimeout(toast, 1500);
            }
    		mainapp->SetWaitBack(false);
    	} else this->waitInput = false;
    }
}
