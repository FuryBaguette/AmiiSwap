#include <MainApplication.hpp>

int main()
{
    ui::Initialize();
    atexit(ui::Finalize);
    ui::MainApplication *amain = new ui::MainApplication();
	ui::SetMainApplication(amain);
    amain->Show();
    delete amain;
    return 0;
}
