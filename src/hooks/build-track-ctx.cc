#include <hooks/build-track-ctx.hh>

// We obtain: the track URI, its file_id, the track bitrate
// (linked to the file_id)

namespace sp_load::hooks::build_track_ctx
{
    BuildTrackCtxFn Original = nullptr;

    std::optional<std::tuple<track_gid_t, audio_file_id_t>> LastTrackGidFileId;

    void *Hook(void *self, TrackSelectionCtx *outCtx, int mask)
    {
        auto ret = Original(self, outCtx, mask);

        if (!(mask & 2))
            return ret;

        if (outCtx->link_type != SpotifyLinkType::Track)
            return ret; // Not a track link, skip logging

        auto kind = outCtx->selected_kind.view();
        auto identifier = outCtx->selected_identifier.view();

        if (kind != "fileid" ||
            identifier.size() != sizeof(audio_file_id_t) * 2) // Not a file ID, or invalid identifier size
            return ret;

        std::array<char, sizeof(audio_file_id_t) * 2> fileIdStr;

        audio_file_id_t fileIdBytes;
        std::ranges::copy(identifier, fileIdStr.begin());
        fileIdBytes = utils::HexToBytes(fileIdStr);

        auto spLink = std::make_tuple(outCtx->link_id, fileIdBytes);

        if (LastTrackGidFileId.has_value() && LastTrackGidFileId.value() == spLink)
            return ret; // Already logged this track GID and file ID combination, skip logging

        LastTrackGidFileId = spLink; // Update last logged track GID and file ID

        TrackInfo::get().OnTrackInfo(outCtx->link_id, fileIdBytes, outCtx->nominal_bitrate);
        return ret;
    }

    bool Initialize(HMODULE spDll)
    {
        return InstallHook(spDll, BUILD_TRACK_FILE_SELECTION_CONTEXT_RVA, &Hook, &Original);
    }
}
