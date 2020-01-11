#pragma once

#include <pu/Plutonium>

namespace ui
{
    class ManualLayout : public pu::ui::Layout
    {
        public:
            ManualLayout();
            PU_SMART_CTOR(ManualLayout);
            void LoadFile(std::string Path);
            void Update();
            void ScrollUp();
            void ScrollDown();
			void manual_Input(u64 Down, u64 Up, u64 Held);
        private:
            u32 loffset;
            u32 rlines;
            bool mode;
            std::string pth;
            pu::ui::elm::TextBlock::Ref cntText;
            pu::ui::Layout::Ref prev;
    };
}