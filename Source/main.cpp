#include <MainApplication.hpp>

int main()
{
    ui::MainApplication *amain = new ui::MainApplication();
	ui::SetMainApplication(amain);
    amain->Show();
    delete amain;
    return 0;
}
