#pragma once

#include <aggregator.hh>
#include <array>
#include <hooks/install-hook.hh>
#include <hooks/playplay-aes-key.hh>
#include <hooks/structs.hh>
#include <sp-types.hh>
#include <utils.hh>
#include <windows.h>

namespace sp_load::hooks::build_track_ctx
{
    // RVA 0x13FB200 Mappings
    enum class SpotifyLinkType : uint32_t
    {
        Empty = 0,
        Artist = 1,
        Album = 2,
        Search = 3,
        Track = 4
    };

    struct TrackSelectionCtx
    {
        std::array<uint8_t, 0x70> _pad_0;
        SpotifyLinkType link_type;
        std::array<uint8_t, 0x04> _pad_1;
        std::array<uint8_t, 0x10> link_id; // WARNING: Only valid for track GID (16 bytes)
        std::array<uint8_t, 0x18> _pad_2;
        RawStdString selected_source;
        RawStdString selected_identifier;
        RawStdString selected_aux_id;
        RawStdString selected_kind;
        std::array<uint8_t, 0x2C> _pad2;
        uint32_t nominal_bitrate;
    };

    static_assert(offsetof(TrackSelectionCtx, link_type) == 0x70);
    static_assert(offsetof(TrackSelectionCtx, link_id) == 0x78);
    static_assert(offsetof(TrackSelectionCtx, selected_source) == 0xA0);
    static_assert(offsetof(TrackSelectionCtx, selected_identifier) == 0xC0);
    static_assert(offsetof(TrackSelectionCtx, selected_aux_id) == 0xE0);
    static_assert(offsetof(TrackSelectionCtx, selected_kind) == 0x100);
    static_assert(offsetof(TrackSelectionCtx, nominal_bitrate) == 0x14C);

    using BuildTrackCtxFn = void *(*)(void *, TrackSelectionCtx *, int);

    constexpr uintptr_t BUILD_TRACK_FILE_SELECTION_CONTEXT_RVA = 0x2D44C4;

    bool Initialize(HMODULE spDll);
}
