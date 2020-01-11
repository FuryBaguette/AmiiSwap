#include <MainApplication.hpp>
extern char *fake_heap_end;
static void *ghaddr;

void Initialize()
{
    srand(time(NULL));
	if(R_FAILED(svcSetHeapSize(&ghaddr, 0x10000000))) exit(1);
	fake_heap_end = (char*)ghaddr + 0x10000000;
	if (R_FAILED(setInitialize())) exit(1);
}
void Finalize()
{
    if (utils::IsEmuiiboPresent())
		nfpemuExit();
	svcSetHeapSize(&ghaddr, ((u8*)envGetHeapOverrideAddr() + envGetHeapOverrideSize()) - (u8*)ghaddr);
}

int main()
{
	Initialize();
	auto renderer = pu::ui::render::Renderer::New(SDL_INIT_EVERYTHING, pu::ui::render::RendererInitOptions::RendererNoSound, pu::ui::render::RendererHardwareFlags);
    auto main = ui::MainApplication::New(renderer);
	ui::SetMainApplication(main);
	main->Prepare();
    main->ShowWithFadeIn();
	Finalize();
    return 0;
}
