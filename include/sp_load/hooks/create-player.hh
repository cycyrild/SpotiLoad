#pragma once

#include <aggregator.hh>
#include <cstdint>
#include <hooks/install-hook.hh>
#include <optional>
#include <windows.h>

namespace sp_load::hooks::create_player
{
    using CreatePlayerFn = void *(*)(void *self,
                                     void *out_slot,
                                     void *track,
                                     double speed,
                                     uint32_t normalization,
                                     uint32_t urgency,
                                     uint32_t track_flag,
                                     void *opaque_ctx,
                                     uint32_t stream_type,
                                     void *start_pos,
                                     void *seek_timestamp,
                                     std::optional<uint32_t> post_event,
                                     void *options);

    constexpr uintptr_t CREATE_PLAYER_RVA = 0x2AA878;

    bool Initialize(HMODULE module);
}
