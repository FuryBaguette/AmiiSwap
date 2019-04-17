
#include <switch.h>

#ifdef __cplusplus
extern "C" {
#endif

bool emuiiboIsPresent();

Result nfpemuInitialize();
void nfpemuExit();

Result nfpemuGetAmiiboCount(u32 *out);
Result nfpemuGetCurrentAmiibo(u32 *idx_out);
Result nfpemuRequestUseCustomAmiibo(const char *path);
Result nfpemuRequestResetCustomAmiibo();
Result nfpemuToggle();
Result nfpemuToggleOnce();
Result nfpemuSwapNext();

#ifdef __cplusplus
}
#endif