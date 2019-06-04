
#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

bool emuiiboIsPresent();

Result nfpemuInitialize();
void nfpemuExit();

typedef enum {
    NfpEmuToggleStatus_Off = 0,
    NfpEmuToggleStatus_On = 1,
    NfpEmuToggleStatus_Once = 2,
} NfpEmuToggleStatus;

Result nfpemuGetAmiibo(char *out);
Result nfpemuSetAmiibo(const char *path);
Result nfpemuResetAmiibo();
Result nfpemuToggle();
Result nfpemuToggleOnce();
Result nfpemuUntoggle();
Result nfpemuSwapNext();
Result nfpemuGetToggleStatus(NfpEmuToggleStatus *out);
Result nfpemuRescanAmiibos();

#ifdef __cplusplus
}
#endif