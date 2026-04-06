#pragma once

#include <array>
#include <cstdint>

namespace sp_load
{
    using aes_key_t = std::array<uint8_t, 16>;
    using track_gid_t = std::array<uint8_t, 16>;
    using audio_file_id_t = std::array<uint8_t, 20>;

    // string types (with NULL terminators)
    using aes_key_str_t = std::array<char, sizeof(aes_key_t) * 2 + 1>;
    using file_id_str_t = std::array<char, sizeof(audio_file_id_t) * 2 + 1>;
    using b62_gid_str_t = std::array<char, 22 + 1>;
    using track_uri_str_t = std::array<char, sizeof("spotify:track:") + sizeof(b62_gid_str_t) - 1>;
}
