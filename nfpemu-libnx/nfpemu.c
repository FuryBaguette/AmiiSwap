#include "nfpemu.h"
#include <string.h>

bool emuiiboIsPresent()
{
    Handle tmph = 0;
    Result rc = smRegisterService(&tmph, "nfp:emu", false, 1);
    if(R_FAILED(rc)) return true;
    smUnregisterService("nfp:emu");
    return false;
}

static Service g_nfpEmuSrv;
static u64 g_refCnt;

Result nfpemuInitialize()
{
    if(!emuiiboIsPresent()) return MAKERESULT(Module_Libnx, LibnxError_NotFound);

    atomicIncrement64(&g_refCnt);
    if(serviceIsActive(&g_nfpEmuSrv)) return 0;
    return smGetService(&g_nfpEmuSrv, "nfp:emu");
}

void nfpemuExit()
{
    if(atomicDecrement64(&g_refCnt) == 0) serviceClose(&g_nfpEmuSrv);
}

Result nfpemuGetAmiibo(char *out)
{
    IpcCommand c;
    ipcInitialize(&c);
    ipcAddRecvBuffer(&c, out, FS_MAX_PATH, BufferType_Normal);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 0;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuSetAmiibo(const char *path)
{
    IpcCommand c;
    ipcInitialize(&c);
    char cpath[FS_MAX_PATH] = {0};
    strcpy(cpath, path);
    ipcAddSendBuffer(&c, cpath, FS_MAX_PATH, BufferType_Normal);

    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 1;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuRequestResetAmiibo()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 2;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuToggle()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 3;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuToggleOnce()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 4;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuUntoggle()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 5;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuSwapNext()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 6;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}

Result nfpemuGetToggleStatus(NfpEmuToggleStatus *out)
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 7;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
            u32 status;
        } *resp = r.Raw;

        rc = resp->result;
        if(R_SUCCEEDED(rc)) *out = (NfpEmuToggleStatus)resp->status;
    }

    return rc;
}

Result nfpemuRescanAmiibos()
{
    IpcCommand c;
    ipcInitialize(&c);
    struct {
        u64 magic;
        u64 cmd_id;
    } *raw;

    raw = ipcPrepareHeader(&c, sizeof(*raw));
    raw->magic = SFCI_MAGIC;
    raw->cmd_id = 8;
    Result rc = serviceIpcDispatch(&g_nfpEmuSrv);

    if(R_SUCCEEDED(rc))
    {
        IpcParsedCommand r;
        ipcParse(&r);

        struct {
            u64 magic;
            u64 result;
        } *resp = r.Raw;

        rc = resp->result;
    }

    return rc;
}