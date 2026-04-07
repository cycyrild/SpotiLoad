#pragma once

#include <cstdint>
#include <etl/vector.h>
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
            auto &entry = GetOrInsert(fileId);

            entry.trackGid = gid;
            entry.bitrate = bitrate;
            entry.mask |= HasBitrate;

            TryFinalize(&entry);
        }

        void OnAesKey(const audio_file_id_t &fileId, const aes_key_t &key)
        {
            auto &entry = GetOrInsert(fileId);

            entry.aesKey = key;
            entry.mask |= HasAesKey;

            TryFinalize(&entry);
        }

      private:
        TrackInfo *Find(const audio_file_id_t &fileId)
        {
            for (auto &e : Cache)
                if (e.fileId == fileId)
                    return &e;
            return nullptr;
        }

        TrackInfo &GetOrInsert(const audio_file_id_t &fileId)
        {
            if (auto *existing = Find(fileId))
                return *existing;

            if (Cache.full())
                Cache.erase(Cache.begin());

            auto &entry = Cache.emplace_back();
            entry.fileId = fileId;

            return entry;
        }

        void TryFinalize(TrackInfo *entry)
        {
            if (!entry || !entry->IsComplete())
                return;

            if (OnTrackReady)
                OnTrackReady(*entry);

            auto it = Cache.begin() + (entry - Cache.data());
            Cache.erase(it);
        }

      private:
        etl::vector<TrackInfo, 2> Cache;
    };
}
