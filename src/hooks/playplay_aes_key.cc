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
        if (g_AesHookStolenBytesSize != g_AesCaptureTrampolineSize)
        {
            printf("Aes hook stolen bytes size (%u) does not match trampoline size (%u)\n",
                   g_AesHookStolenBytesSize,
                   g_AesCaptureTrampolineSize);
            return;
        }

        uintptr_t moduleBase = (uintptr_t)spDll;
        uintptr_t hookAddress = moduleBase + HOOK_AES_KEY_RVA;
        uint8_t *dst = (uint8_t *)hookAddress;

        g_AesHookResumeAddress = hookAddress + g_AesHookStolenBytesSize;

        DWORD oldProtect = 0;
        VirtualProtect(dst, g_AesHookStolenBytesSize, PAGE_EXECUTE_READWRITE, &oldProtect);

        memcpy(dst, AesKeyCaptureTrampoline, g_AesHookStolenBytesSize);

        VirtualProtect(dst, g_AesHookStolenBytesSize, oldProtect, &oldProtect);
        FlushInstructionCache(GetCurrentProcess(), dst, g_AesHookStolenBytesSize);
    }

    bool Initialize(HMODULE spDll)
    {
        return InstallHook(spDll, HOOK_HANDLE_PLAYPLAY_RVA, &Hook, &Original) && (InitializeAesHook(spDll), true);
    }
}
