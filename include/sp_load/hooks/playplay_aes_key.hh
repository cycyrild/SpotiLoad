#pragma once

#include <aggregator.hh>
#include <array>
#include <hooks/install_hook.hh>
#include <hooks/structs.hh>
#include <sp_types.hh>
#include <stdint.h>
#include <utils.hh>
#include <windows.h>

namespace sp_load::hooks::playplay_aes_key
{
    enum class PlayplayResourceKind : uint8_t
    {
        ResourceId20 = 0,
        ResourceId16 = 1,
    };

    struct PlayplayRequestState
    {
        std::array<uint8_t, 0xA0> _pad_0;
        uint8_t _pad_2;
        std::array<uint8_t, 7> _pad3;
        uint64_t _pad4;
        uint64_t _pad5;
        uint32_t _pad_6;
        std::array<uint8_t, 20> resource_id;
        PlayplayResourceKind resource_kind;
    };

    static_assert(offsetof(PlayplayRequestState, resource_id) == 0xBC);
    static_assert(offsetof(PlayplayRequestState, resource_kind) == 0xD0);

    // A stable address in the obfuscated algorithm where the key appears
    constexpr uintptr_t HOOK_AES_KEY_RVA = 0x20233F1;

    // Here we are just before the "obfuscated key" entry in the algorithm.
    // This is a strategic point for matching the file_id with the AES key.
    constexpr uintptr_t HOOK_HANDLE_PLAYPLAY_RVA = 0x2E62CC;

    extern "C" void AesKeyCaptureHookStub();
    extern "C" uintptr_t g_AesHookResumeAddress;
    extern "C" aes_key_t g_AesCapturedKey;
    extern "C" uint32_t g_AesHookStolenBytes;

    using HandlePlayplayFn = void *(*)(void *, PlayplayRequestState *, RawStdString *);

    bool Initialize(HMODULE spDll);
}
