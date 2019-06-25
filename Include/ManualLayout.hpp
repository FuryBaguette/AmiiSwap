#pragma once

#include <pu/Plutonium>

namespace ui
{
    class ManualLayout : public pu::Layout
    {
        public:
            ManualLayout();
            ~ManualLayout();
            void LoadFile(std::string Path);
            void Update();
            void ScrollUp();
            void ScrollDown();
            //pu::Layout *GetPreviousLayout();
        private:
            u32 loffset;
            u32 rlines;
            bool mode;
            std::string pth;
            pu::element::TextBlock *cntText;
            pu::Layout *prev;
    };
}