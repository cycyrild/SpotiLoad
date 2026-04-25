#include <hooks/playplay_aes_key.hh>

// We obtain the AES key linked to the file_id of the track

namespace sp_load::hooks::playplay_aes_key
{
    HandlePlayplayFn Original = nullptr;

    extern "C" uintptr_t g_AesHookResumeAddress{};
    extern "C" aes_key_t g_AesCapturedKey{};

    void *Hook(void *self, PlayplayRequestState *req, RawStdString *playplayRawAnswer)
    {
        void *ret;

        ret = Original(self, req, playplayRawAnswer);

        if (req->resource_kind != PlayplayResourceKind::ResourceId20)
            return ret;

        TrackInfo::get().OnAesKey(req->resource_id, g_AesCapturedKey);
        return ret;
    }

    // We install a small stub that captures the 16 byte key and then matches it to the file_id in Hook()
    void InitializeAesHook(HMODULE spDll)
    {
        uintptr_t moduleBase = (uintptr_t)spDll;
        uintptr_t hookAddress = moduleBase + HOOK_AES_KEY_RVA;
        uint8_t *dst = (uint8_t *)hookAddress;

        g_AesHookResumeAddress = hookAddress + g_AesHookStolenBytes;

        DWORD oldProtect = 0;
        VirtualProtect(dst, g_AesHookStolenBytes, PAGE_EXECUTE_READWRITE, &oldProtect);

        memset(dst, 0x90, g_AesHookStolenBytes);

        dst[0] = 0x49;
        dst[1] = 0xBB;
        *(uintptr_t *)(dst + 2) = (uintptr_t)AesKeyCaptureHookStub;
        dst[10] = 0x41;
        dst[11] = 0xFF;
        dst[12] = 0xE3;

        VirtualProtect(dst, g_AesHookStolenBytes, oldProtect, &oldProtect);
        FlushInstructionCache(GetCurrentProcess(), dst, g_AesHookStolenBytes);
    }

    bool Initialize(HMODULE spDll)
    {
        return InstallHook(spDll, HOOK_HANDLE_PLAYPLAY_RVA, &Hook, &Original) && (InitializeAesHook(spDll), true);
    }
}
