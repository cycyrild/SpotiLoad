#include <attach.hh>

namespace sp_load
{
    void Attach()
    {
        MH_STATUS status;
        HMODULE spDll;

        spDll = LoadLibraryW(L"Spotify.dll");

        if (!spDll)
        {
            printf("Failed to load Spotify.dll: %lu\n", GetLastError());
            return;
        }

        status = MH_Initialize();
        if (status != MH_OK)
        {
            printf("MinHook init failed: %s\n", MH_StatusToString(status));
            return;
        }

        printf("Spotify.dll loaded at: %p\n", (void *)spDll);

        MyTrackInfoAggregator.OnTrackReady = [](const TrackInfoAggregator::TrackInfo &info)
        {
            auto fileIdHex = utils::BytesToHex(info.fileId);
            auto aesKeyHex = utils::BytesToHex(info.aesKey);
            auto b62gid = sp_id::GidToBase62(info.trackGid);

            printf("Track ready:\n");
            printf("  Track URI: spotify:track:%.*s\n", (int)b62gid.size(), b62gid.data());
            printf("  File ID: %.*s\n", (int)fileIdHex.size(), fileIdHex.data());
            printf("  Bitrate: %d\n", info.bitrate);
            printf("  AES Key: %.*s\n", (int)aesKeyHex.size(), aesKeyHex.data());
        };

        sp_load::hooks::playplay_aes_key::Initialize(spDll);
        sp_load::hooks::build_track_ctx::Initialize(spDll);
        sp_load::hooks::bearer_refresh::Initialize(spDll);
        sp_load::hooks::create_player::Initialize(spDll);
    }
}
