#include <MainApplication.hpp>

namespace ui
{
    extern MainApplication *mainapp;

    ManualLayout::ManualLayout()
    {
        this->cntText = new pu::element::TextBlock(20, 120, "");
        this->cntText->SetColor({0,51,102,255});
        this->Add(this->cntText);
        this->loffset = 0;

        this->SetOnInput([&](u64 Down, u64 Up, u64 Held, bool Touch)
        {
            if (Down & KEY_B){
                mainapp->SetHelpText("A: Select ");
                mainapp->GetMainLayout()->GetMainMenu()->SetVisible(true);
                mainapp->GetMainLayout()->SetElementOnFocus(mainapp->GetMainLayout()->GetMainMenu());
                mainapp->GetMainLayout()->selectionChange();
                mainapp->LoadLayout(mainapp->GetMainLayout());
            }
            else if((Down & KEY_DDOWN) || (Down & KEY_LSTICK_DOWN) || (Held & KEY_RSTICK_DOWN)) this->ScrollDown();
            else if((Down & KEY_DUP) || (Down & KEY_LSTICK_UP) || (Held & KEY_RSTICK_UP)) this->ScrollUp();
        });
    }

    ManualLayout::~ManualLayout()
    {
        delete this->cntText;
    }

    void ManualLayout::LoadFile(std::string Path)
    {
        this->pth = Path;
        this->loffset = 0;
        this->Update();
    }

    void ManualLayout::Update()
    {
        std::vector<std::string> lines;
        lines = utils::ReadFileLines(this->pth, this->loffset, 19);
        if(lines.empty())
        {
            this->loffset--;
            return;
        }
        this->rlines = lines.size();
        std::string alines;
        if(this->rlines > 0) for(u32 i = 0; i < this->rlines; i++)
        {
            if(i > 0) alines += "\n";
            alines += lines[i];
        }
        this->cntText->SetText(alines);
        lines.clear();
    }

    void ManualLayout::ScrollUp()
    {
        if(this->loffset > 0)
        {
            this->loffset--;
            this->Update();
        }
    }

    void ManualLayout::ScrollDown()
    {
        this->loffset++;
        this->Update();
    }
}