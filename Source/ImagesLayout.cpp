#include "MainApplication.hpp"

namespace ui
{
    extern MainApplication *mainapp;

    ImagesLayout::ImagesLayout() : pu::Layout()
    {
        this->imagesMenu = new pu::element::Menu(0, 80, 1280, {255,255,255,255}, 100, 6);
        this->progressBar = new pu::element::ProgressBar(640,110,600,40,100);
        this->progressBar->SetColor({255,255,255,255});
        this->progressBar->SetProgressColor({0,102,153,255});
        this->imagesMenu->SetOnFocusColor({102,153,204,255});
        this->imagesMenu->SetScrollbarColor({102,153,204,255});
        this->imagesMenu->SetOnSelectionChanged(std::bind(&ImagesLayout::selectionChange,this));
        this->Add(this->imagesMenu);
        this->SetElementOnFocus(this->imagesMenu);
        populateImagesMenu();

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
        {
            if (Down & KEY_B){
                mainapp->SetHelpText("A: Select ");
                mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
                mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
                mainapp->GetMainLayout()->selectionChange();
                mainapp->LoadLayout(mainapp->GetMainLayout());
            }
        });
    }

    ImagesLayout::~ImagesLayout()
    {
        delete this->imagesMenu;
    }

    void ImagesLayout::populateImagesMenu()
    {
        this->imagesMenu->ClearItems();

        pu::element::MenuItem *item = new pu::element::MenuItem("Search and move images");
        item->SetIcon(utils::GetRomFsResource("Common/search-folder.png"));
        item->AddOnClick(std::bind(&ImagesLayout::search_Click, this), KEY_A);
        this->imagesMenu->AddItem(item);
        
        item = new pu::element::MenuItem("Rename images");
        item->SetIcon(utils::GetRomFsResource("Common/edit-image.png"));
        item->AddOnClick(std::bind(&ImagesLayout::rename_Click, this), KEY_A);
        this->imagesMenu->AddItem(item);
    }

    void ImagesLayout::search_Click()
    {
        int imgCount = 0;
        int progress = 0;
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
        this->progressBar->ClearProgress();
        this->progressBar->SetY(110);
        this->progressBar->SetMaxValue(images.size());
        this->progressBar->SetVisible(true);
        for(auto image : images){
            progress++;
            this->progressBar->SetProgress((progress/images.size())*100);
            imgName = utils::getLastFromPath(image);
            imgName = imgName.substr(0,imgName.size()-4);
            for(auto amiibo : amiibos){
                amiiboName = utils::getLastFromPath(amiibo);
                if(amiiboName == imgName){
                    newname = amiibo + "/amiibo.png";
                    if(std::rename(image.c_str(),newname.c_str()) == 0 ){
                        imgCount++;
                    }
                    break;
                }
            }  
        }
        this->progressBar->SetVisible(false);
        pu::overlay::Toast *toast = new pu::overlay::Toast("Found " + std::to_string(imgCount) + " images", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);     
    }

    void ImagesLayout::rename_Click()
    {
        int imgCount = 0;
        int progress = 0;
        char emuiiboFolder[] = "sdmc:/emuiibo";
        std::vector<std::string> amiibos;
        utils::get_amiibos_directories(emuiiboFolder, &amiibos);
        std::string oldname;
        std::string newname;
        this->progressBar->ClearProgress();
        this->progressBar->SetY(210);
        this->progressBar->SetMaxValue(amiibos.size());
        this->progressBar->SetVisible(true);
        for(auto amiibo : amiibos){
            progress++;
            this->progressBar->SetProgress((progress/amiibos.size())*100);
            if (auto dir = opendir(amiibo.c_str())) {
                while (auto f = readdir(dir)) {
                    if (!f->d_name || f->d_name[0] == '.') continue;
                    if (f->d_type == DT_REG && f->d_name != "amiibo.png") {
                        const char* ext = ui::getExtension(f->d_name);
                        if (strcasecmp(ext, ".png")==0){
                            oldname = amiibo + "/" + f->d_name;
                            newname = amiibo+"/amiibo.png";
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
        this->progressBar->SetVisible(false);
        pu::overlay::Toast *toast = new pu::overlay::Toast("Renamed " + std::to_string(imgCount) + " images", 20, {255,255,255,255}, {0,51,102,255});
        mainapp->StartOverlayWithTimeout(toast, 1500);      
    }
    
    pu::element::Menu *ImagesLayout::GetImagesMenu()
    {
        return (this->imagesMenu);
    }
    
    void ImagesLayout::selectionChange()
    {
        switch(this->imagesMenu->GetSelectedIndex()){
            case 0:
                mainapp->SetFooterText("Search amiibo icons in AmiiSwap and emuiibo folders");
                break;
            case 1:
                mainapp->SetFooterText("Rename .png in each amiibo folder for use with AmiiSwap");
                break;
            default:
                mainapp->SetFooterText("");
                break;    
        }
    }
}
