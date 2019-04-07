#include <MainApplication.hpp>

int main()
{
    MainApplication *amain = new MainApplication();
	SetMainApplication(amain);
    amain->Show();
    delete amain;
    return 0;
}