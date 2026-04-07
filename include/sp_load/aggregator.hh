#pragma once

#include <instance.hh>
#include <track-info-aggregator.hh>

namespace sp_load
{
    inline TrackInfoAggregator &GetTrackInfoAggregator()
    {
        return StaticInstance<TrackInfoAggregator>();
    }
}
