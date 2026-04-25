#include <hooks/create_player.hh>

// I'll leave this here for now... it might be useful

namespace sp_load::hooks::create_player
{
    CreatePlayerFn Original = nullptr;

    void *Hook(
        void *self,
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
        void *options)
    {
        void *ret;
        ret = Original(
            self,
            out_slot,
            track,
            speed,
            normalization,
            urgency,
            track_flag,
            opaque_ctx,
            stream_type,
            start_pos,
            seek_timestamp,
            post_event,
            options);
        // printf("CreatePlayer called with track: %p\n", track);
        return ret;
    }

    bool Initialize(HMODULE module)
    {
        return InstallHook(module, CREATE_PLAYER_RVA, &Hook, &Original);
    }
}
