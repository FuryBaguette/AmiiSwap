#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AmiiboDetailsLayout : public pu::ui::Layout
	{
	    public:
	        AmiiboDetailsLayout();
			PU_SMART_CTOR(AmiiboDetailsLayout);
			void LoadAmiibo(std::string path);
			void amiiboDetails_Input(u64 Down, u64 Up, u64 Held);
		private:
			pu::ui::elm::Image::Ref amiiboImage;
			pu::ui::elm::TextBlock::Ref amiiboName;
			pu::ui::elm::TextBlock::Ref amiiboPath;
			pu::ui::elm::TextBlock::Ref amiiboRandom;
			pu::ui::elm::TextBlock::Ref firstWrite;
			pu::ui::elm::TextBlock::Ref lastWrite;
			bool randomuuid = false;
	};
}
