#pragma once

#include <pu/Plutonium>

namespace ui
{
	class AmiiboDetailsLayout : public pu::Layout
	{
	    public:
	        AmiiboDetailsLayout();
			~AmiiboDetailsLayout();
			void LoadAmiibo(std::string path);
		private:
			pu::element::Image *amiiboImage;
			pu::element::TextBlock *amiiboName;
			pu::element::TextBlock *amiiboPath;
			pu::element::TextBlock *amiiboRandom;
			pu::element::TextBlock *firstWrite;
			pu::element::TextBlock *lastWrite;
			bool randomuuid = false;
	};
}
