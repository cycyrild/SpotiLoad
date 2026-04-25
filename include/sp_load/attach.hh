#pragma once

#include <hooks/bearer_refresh.hh>
#include <hooks/build_track_ctx.hh>
#include <hooks/create_player.hh>
#include <hooks/playplay_aes_key.hh>
#include <sp_id.hh>
#include <track_info_aggregator.hh>
#include <utils.hh>
#include <windows.h>

namespace sp_load
{
    void Attach();
}
