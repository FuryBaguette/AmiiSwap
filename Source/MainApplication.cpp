#include <MainApplication.hpp>

MainApplication *mainapp;

AmiiboFile::AmiiboFile(std::string Name, std::string BinPath)
{
	this->Name = Name;
	this->BinPath = BinPath;
}

std::string AmiiboFile::GetName()
{
	return (this->Name);
}

std::string AmiiboFile::GetPath()
{
	return (this->BinPath);
}

AmiiboGame::AmiiboGame(std::string Name, std::string FolderPath)
{
	this->Name = Name;
	this->FolderPath = FolderPath;
}

std::string AmiiboGame::GetName()
{
	return (this->Name);
}

std::string AmiiboGame::GetPath()
{
	return (this->FolderPath);
}

std::vector<AmiiboFile*> AmiiboGame::GetBinFiles()
{
	return (this->amiiboFiles);
}

void AmiiboGame::AddAmiiboFile(AmiiboFile *file)
{
	this->amiiboFiles.push_back(file);
}

void AmiiboGame::SetAmiiboFiles(std::vector<AmiiboFile*> files)
{
	this->amiiboFiles.insert(this->amiiboFiles.end(), files.begin(), files.end());
}

void CreateDirectory(std::string Path)
{
	int res = mkdir(Path.c_str(), 777);
}

void EnsureDirectories()
{
	CreateDirectory("sdmc:/amiibos");
}

void getFiles(const std::string &path, std::function<void(const std::string &)> cb)
{
	if (auto dir = opendir(path.c_str())) {
		while (auto f = readdir(dir)) {
			if (!f->d_name || f->d_name[0] == '.') continue;
			if (f->d_type == DT_DIR) 
				getFiles(path + f->d_name + "/", cb);

			if (f->d_type == DT_REG) {
				const char* ext = getExtension(f->d_name);
				if (strcasecmp(ext, ".bin")==0)
					cb(path + f->d_name);
			}
		}
		closedir(dir);
	}
}

std::vector<std::string> get_directories(char *path)
{
    std::vector<std::string> r;
    DIR *dir = opendir(path);

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if (entry->d_type == DT_DIR)
            r.push_back(entry->d_name);

        entry = readdir(dir);
    }

    closedir(dir);
    return r;
}

void MainLayout::GetFolders()
{
	std::vector<std::string> gameFolders = get_directories("sdmc:/amiibos/");
	
	for (auto & element : gameFolders) {
		std::size_t found = element.find_last_of("/\\");
		std::string namePath = element.substr(found+1);
		
		AmiiboGame *game = new AmiiboGame(namePath, "sdmc:/amiibos/" + element + "/");
		
		std::vector<AmiiboFile*> amiiboFiles;
		
		getFiles(game->GetPath(), [&game](const std::string &path) {
			std::size_t found1 = path.find_last_of("/\\");
			std::string namePath1 = path.substr(found1+1);
			namePath1.erase(namePath1.length() - 4);
			
			AmiiboFile *file = new AmiiboFile(namePath1, path);
			
			game->AddAmiiboFile(file);
		});
		
		this->amiiboGames.push_back(game);
	}
}

MainLayout::MainLayout()
{
	EnsureDirectories();
	this->GetFolders();
    this->titleText = new pu::element::TextBlock(640, 10, "AmiiSwap");
    this->gamesMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
    this->amiiboMenu = new pu::element::Menu(0, 50, 1280, {255,255,255,255}, 70, 9);
	
	this->titleText->SetX(640 - (this->titleText->GetTextWidth() / 2));
	
	for (auto & element : this->amiiboGames) {
		pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
		item->AddOnClick(std::bind(&MainLayout::category_Click, this, element), KEY_A);
		this->gamesMenu->AddItem(item);
	}
	
	this->amiiboMenu->SetVisible(false);
	
    this->AddChild(this->titleText);
    this->AddChild(this->gamesMenu);
    this->AddChild(this->amiiboMenu);
	this->SetElementOnFocus(this->gamesMenu);
}

bool copyFile(const char *SRC, const char* DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

void MainLayout::category_Click(AmiiboGame *game)
{
	this->waitInput = true;
	this->amiiboMenu->ClearItems();
	std::vector<AmiiboFile*> files = game->GetBinFiles();
	for (auto & element : files) {
		pu::element::MenuItem *item = new pu::element::MenuItem(element->GetName());
		item->AddOnClick(std::bind(&MainLayout::item_Click, this, element), KEY_A);
		this->amiiboMenu->AddItem(item);
	}
	
	this->amiiboMenu->SetSelectedIndex(0);
	this->SetElementOnFocus(this->amiiboMenu);
	this->amiiboMenu->SetVisible(true);
	this->gamesMenu->SetVisible(false);
}

void MainLayout::item_Click(AmiiboFile *element)
{
	if (!waitInput) {
		int sopt = mainapp->CreateShowDialog("Use " + element->GetName() + " ?", "This will set the current Amiibo to " + element->GetName(), { "Yes", "No" }, true);
		if (sopt < 0 || sopt == 1) return;
		if (sopt == 0) copyFile(element->GetPath().c_str(), "sdmc:/amiibo.bin");
	} else this->waitInput = false;
}

pu::element::Menu *MainLayout::GetGamesMenu()
{
	return (this->gamesMenu);
}

pu::element::Menu *MainLayout::GetAmiiboMenu()
{
	return (this->amiiboMenu);
}

MainApplication::MainApplication()
{
    this->mainLayout = new MainLayout();
    this->LoadLayout(this->mainLayout);
	this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
    {
        if(Down & KEY_PLUS) this->Close();
		else if (Down & KEY_Y) {
			this->mainLayout->GetGamesMenu()->SetVisible(true);
			this->mainLayout->SetElementOnFocus(this->mainLayout->GetGamesMenu());
			this->mainLayout->GetAmiiboMenu()->SetVisible(false);
		}
    });
}

void SetMainApplication(MainApplication *MainApp)
{
	mainapp = MainApp;
}