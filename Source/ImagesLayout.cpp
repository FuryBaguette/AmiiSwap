#include "MainApplication.hpp"

extern lang::Language *language;

namespace ui
{
    extern MainApplication *mainapp;

    ImagesLayout::ImagesLayout()
    {
        this->imagesMenu = pu::ui::elm::Menu::New(0, 80, 1280, pu::ui::Color(255,255,255,255), 100, 6);
        this->progressBar = pu::ui::elm::ProgressBar::New(640,120,600,20,100.0f);
        this->progressBar->SetColor(pu::ui::Color(255,255,255,255));
        this->progressBar->SetProgressColor(pu::ui::Color(0,102,153,255));
        this->progressBar->SetVisible(false);
        this->imagesMenu->SetOnFocusColor(pu::ui::Color(102,153,204,255));
        this->imagesMenu->SetScrollbarColor(pu::ui::Color(102,153,204,255));
        this->imagesMenu->SetOnSelectionChanged(std::bind(&ImagesLayout::selectionChange,this));
        this->Add(this->imagesMenu);
        this->Add(this->progressBar);
        //this->SetElementOnFocus(this->imagesMenu);
        populateImagesMenu();

        this->SetOnInput(std::bind(&ImagesLayout::images_Input, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }
	
	void ImagesLayout::images_Input(u64 Down, u64 Up, u64 Held)
	{
		if (Down & KEY_B){
			mainapp->SetHelpText(lang::GetLabel(lang::Label::HELP_SELECT));
			mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
			//mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
			mainapp->GetMainLayout()->selectionChange();
			mainapp->LoadLayout(mainapp->GetMainLayout());
		}
	}

    void ImagesLayout::populateImagesMenu()
    {
        this->imagesMenu->ClearItems();

        pu::ui::elm::MenuItem::Ref item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::IMAGES_SEARCH));
        item->SetIcon(utils::GetRomFsResource("Common/search-folder.png"));
        item->AddOnClick(std::bind(&ImagesLayout::search_Click, this), KEY_A);
        this->imagesMenu->AddItem(item);
        
        item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::IMAGES_RENAME));
        item->SetIcon(utils::GetRomFsResource("Common/edit-image.png"));
        item->AddOnClick(std::bind(&ImagesLayout::rename_Click, this), KEY_A);
        this->imagesMenu->AddItem(item);
        
        item = pu::ui::elm::MenuItem::New(lang::GetLabel(lang::Label::IMAGES_DELETE));
        item->SetIcon(utils::GetRomFsResource("Common/remove-image.png"));
        item->AddOnClick(std::bind(&ImagesLayout::remove_Click, this), KEY_A);
        this->imagesMenu->AddItem(item);
    }

    void ImagesLayout::search_Click()
    {
        int imgCount = 0;
        double progress = 0.0f;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        char amiiswapFolder[] = "sdmc:/switch/AmiiSwap";
        std::string imgName;
        std::string amiiboName;
        std::string newname;
        std::vector<std::string> images;
        std::vector<std::string> amiibos;
        utils::getImages(emuiiboFolder, &images);
        utils::getImages(amiiswapFolder, &images);
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        this->progressBar->SetY(120);
        this->progressBar->SetMaxValue(double(images.size()));
        this->progressBar->SetVisible(true);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->progressBar);
        for(auto image : images){
            progress++;
            this->progressBar->SetProgress((progress/double(images.size()))*100.0f);
            mainapp->CallForRender();
            imgName = utils::getLastFromPath(image);
            const char* ext = ui::getExtension(imgName.c_str());
            imgName = imgName.substr(0,imgName.size() - (unsigned)strlen(ext));
            for(auto amiibo : amiibos){
                amiiboName = utils::getLastFromPath(amiibo);
                if(amiiboName == imgName){
                    newname = amiibo + "/amiibo.icon";
                    if(std::rename(image.c_str(),newname.c_str()) == 0 ){
                        imgCount++;
                    }
                    break;
                }
            }  
        }
        this->progressBar->FillProgress();
        mainapp->CallForRender();
        this->progressBar->SetVisible(false);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->imagesMenu);
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_IMAGES_FOUND) + std::to_string(imgCount) + lang::GetLabel(lang::Label::TOAST_IMAGES_IMAGES), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);     
    }

    void ImagesLayout::rename_Click()
    {
        int imgCount = 0;
        double progress = 0.0f;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> amiibos;
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        std::string oldname;
        std::string newname;
        this->progressBar->SetY(220);
        this->progressBar->SetMaxValue(double(amiibos.size()));
        this->progressBar->SetVisible(true);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->progressBar);
        for(auto amiibo : amiibos){
            progress++;
            this->progressBar->SetProgress((progress/double(amiibos.size()))*100.0f);
            mainapp->CallForRender();
            if (auto dir = opendir(amiibo.c_str())) {
                while (auto f = readdir(dir)) {
                    if (!f->d_name || f->d_name[0] == '.') continue;
                    if (f->d_type == DT_REG ){
                        const char* ext = ui::getExtension(f->d_name);
                        if (strcasecmp(ext, ".png") == 0 || strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0){
                            oldname = amiibo + "/" + f->d_name;
                            newname = amiibo+"/amiibo.icon";
                            if(std::rename(oldname.c_str(),newname.c_str()) == 0 ){
                                imgCount++;
                                break;
                            }
                        }
                    }
                }
                closedir(dir);
            }
        }
        this->progressBar->FillProgress();
        mainapp->CallForRender();
        this->progressBar->SetVisible(false);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->imagesMenu);
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_IMAGES_RENAMED) + std::to_string(imgCount) + lang::GetLabel(lang::Label::TOAST_IMAGES_IMAGES), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);      
    }

    void ImagesLayout::remove_Click()
    {
        int imgCount = 0;
        double progress = 0.0f;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> amiibos;
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        std::string path;
        this->progressBar->SetY(320);
        this->progressBar->SetMaxValue(double(amiibos.size()));
        this->progressBar->SetVisible(true);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->progressBar);
        for(auto amiibo : amiibos){
            progress++;
            this->progressBar->SetProgress((progress/double(amiibos.size()))*100.0f);
            mainapp->CallForRender();
            if (auto dir = opendir(amiibo.c_str())) {
                while (auto f = readdir(dir)) {
                    if (!f->d_name || f->d_name[0] == '.') continue;
                    if (f->d_type == DT_REG && std::string(f->d_name) == "amiibo.icon"){
                        path = amiibo + "/" + f->d_name;
                        if(std::remove(path.c_str()) == 0 ){
                            imgCount++;
                            break;
                        }
                    }
                }
                closedir(dir);
            }
        }
        this->progressBar->FillProgress();
        mainapp->CallForRender();
        this->progressBar->SetVisible(false);
        mainapp->CallForRender();
        //this->SetElementOnFocus(this->imagesMenu);
        pu::ui::extras::Toast::Ref toast = pu::ui::extras::Toast::New(lang::GetLabel(lang::Label::TOAST_IMAGES_DELETED) + std::to_string(imgCount) + lang::GetLabel(lang::Label::TOAST_IMAGES_IMAGES), 20, pu::ui::Color(255,255,255,255), pu::ui::Color(0,51,102,255));
        mainapp->StartOverlayWithTimeout(toast, 1500);      
    }

    pu::ui::elm::Menu::Ref ImagesLayout::GetImagesMenu()
    {
        return (this->imagesMenu);
    }
    
    void ImagesLayout::selectionChange()
    {
        switch(this->imagesMenu->GetSelectedIndex()){
            case 0:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_IMAGES_SEARCH));
                break;
            case 1:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_IMAGES_RENAME));
                break;
            case 2:
                mainapp->SetFooterText(lang::GetLabel(lang::Label::FOOTER_IMAGES_DELETE));
                break;
            default:
                mainapp->SetFooterText("");
                break;    
        }
    }
}
