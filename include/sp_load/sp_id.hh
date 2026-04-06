#pragma once

#include <array>
#include <cstdint>

namespace sp_load::sp_id
{
    constexpr char BASE62[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    template <size_t OutSize = 22> constexpr std::array<char, OutSize> GidToBase62(const std::array<uint8_t, 16> &gid)
    {
        std::array<uint8_t, OutSize> dst{};
        size_t i = 0;

        auto read_u32_be = [&](size_t offset) -> uint32_t
        {
            return ((uint32_t)gid[offset + 0] << 24) | ((uint32_t)gid[offset + 1] << 16) |
                   ((uint32_t)gid[offset + 2] << 8) | ((uint32_t)gid[offset + 3]);
        };

        const std::array<uint32_t, 4> parts = {read_u32_be(0), read_u32_be(4), read_u32_be(8), read_u32_be(12)};

        for (uint32_t carry : parts)
        {
            size_t j = 0;
            while (j < i)
            {
                uint64_t val = ((uint64_t)dst[j] << 32) | carry;
                dst[j] = (uint8_t)(val % 62);
                carry = (uint32_t)(val / 62);
                ++j;
            }

            while (carry > 0)
            {
                dst[i++] = (uint8_t)(carry % 62);
                carry /= 62;
            }
        }

        std::array<char, OutSize> out{};

        for (size_t k = 0; k < OutSize; ++k)
            out[k] = BASE62[(size_t)dst[OutSize - 1 - k]];

        return out;
    }
}
