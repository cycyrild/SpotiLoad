#pragma once

#include <algorithm>
#include <boost/container/static_vector.hpp>
#include <cstdint>
#include <sp-types.hh>

namespace sp_load
{
    class TrackInfoAggregator
    {
      public:
        enum FieldMask : uint8_t
        {
            HasNone = 0,
            HasBitrate = 1 << 0,
            HasAesKey = 1 << 1,
            HasAll = HasBitrate | HasAesKey
        };

        struct TrackInfo
        {
            track_gid_t trackGid;
            audio_file_id_t fileId;
            aes_key_t aesKey;
            int bitrate;

            uint8_t mask{HasNone};

            bool IsComplete() const
            {
                return mask == HasAll;
            }
        };

        using OnTrackReadyFn = void (*)(const TrackInfo &);

      public:
        OnTrackReadyFn OnTrackReady = nullptr;

      public:
        void OnTrackInfo(const track_gid_t &gid, const audio_file_id_t &fileId, int bitrate)
        {
            auto &entry = InsertOrGet(fileId);

            entry.trackGid = gid;
            entry.fileId = fileId;
            entry.bitrate = bitrate;
            entry.mask |= HasBitrate;

            TryFinalize(entry);
        }

        void OnAesKey(const audio_file_id_t &fileId, const aes_key_t &key)
        {
            auto &entry = InsertOrGet(fileId);

            entry.aesKey = key;
            entry.mask |= HasAesKey;

            TryFinalize(entry);
        }

      private:
        TrackInfo *Find(const audio_file_id_t &fileId)
        {
            for (auto &entry : Cache)
                if (entry.fileId == fileId)
                    return &entry;

            return nullptr;
        }

        TrackInfo &InsertOrGet(const audio_file_id_t &fileId)
        {
            if (auto *existing = Find(fileId))
                return *existing;

            if (Cache.size() == Cache.capacity())
                Cache.erase(Cache.begin());

            Cache.emplace_back();
            Cache.back().fileId = fileId;

            return Cache.back();
        }

        void TryFinalize(TrackInfo &info)
        {
            if (!info.IsComplete())
                return;

            if (OnTrackReady)
                OnTrackReady(info);

            auto it =
                std::find_if(Cache.begin(), Cache.end(), [&](const TrackInfo &t) { return t.fileId == info.fileId; });

            if (it != Cache.end())
                Cache.erase(it);
        }

      private:
        boost::container::static_vector<TrackInfo, 2> Cache;
    };
}
