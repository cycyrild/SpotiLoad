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
        std::array<uint8_t, 0x88> _pad_0;
        uint32_t request_type;
        std::array<uint8_t, 20> resource_id;
        PlayplayResourceKind resource_kind;
        std::array<uint8_t, 16> auxiliary_id;
        std::array<uint8_t, 7> _pad_1;
        std::array<uint8_t, 16> metadata;
        uint32_t retry_count;
        uint32_t _pad_2;
        uint64_t last_transport_status;
    };

    static_assert(offsetof(PlayplayRequestState, request_type) == 0x88);
    static_assert(offsetof(PlayplayRequestState, resource_id) == 0x8C);
    static_assert(offsetof(PlayplayRequestState, resource_kind) == 0xA0);
    static_assert(offsetof(PlayplayRequestState, auxiliary_id) == 0xA1);
    static_assert(offsetof(PlayplayRequestState, metadata) == 0xB8);
    static_assert(offsetof(PlayplayRequestState, retry_count) == 0xC8);
    static_assert(offsetof(PlayplayRequestState, last_transport_status) == 0xD0);

    // A stable address in the obfuscated algorithm where the key appears
    constexpr uintptr_t HOOK_AES_KEY_RVA = 0x2255E50;

    // Here we are just before the "obfuscated key" entry in the algorithm.
    // This is a strategic point for matching the file_id with the AES key.
    constexpr uintptr_t HOOK_HANDLE_PLAYPLAY_RVA = 0x46EC90;

    extern "C" void AesKeyCaptureTrampoline();
    extern "C" uintptr_t g_AesHookResumeAddress;
    extern "C" aes_key_t g_AesCapturedKey;

    extern "C" uint32_t g_AesHookStolenBytesSize;
    extern "C" uint32_t g_AesCaptureTrampolineSize;

    using HandlePlayplayFn = void *(*)(void *, PlayplayRequestState *, RawStdString *);

    bool Initialize(HMODULE spDll);
}
