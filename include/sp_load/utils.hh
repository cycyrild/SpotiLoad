#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace sp_load::utils
{
    template <size_t N> constexpr auto BytesToHex(const std::array<uint8_t, N> &bytes) -> std::array<char, N * 2>
    {
        constexpr std::array hex{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

        std::array<char, N * 2> out{};

        for (size_t i = 0; i < N; ++i)
        {
            const auto b = bytes[i];
            out[i * 2] = hex[b >> 4];
            out[i * 2 + 1] = hex[b & 0x0F];
        }

        return out;
    }

    constexpr uint8_t HexToNibble(char c)
    {
        if (c >= '0' && c <= '9')
            return static_cast<uint8_t>(c - '0');

        if (c >= 'a' && c <= 'f')
            return static_cast<uint8_t>(c - 'a' + 10);

        if (c >= 'A' && c <= 'F')
            return static_cast<uint8_t>(c - 'A' + 10);

        return 0;
    }

    template <size_t N> constexpr auto HexToBytes(const std::array<char, N> &hex) -> std::array<uint8_t, N / 2>
    {
        static_assert(N % 2 == 0, "Hex string size must be even");

        std::array<uint8_t, N / 2> out{};

        for (size_t i = 0; i < N / 2; ++i)
        {
            const auto hi = HexToNibble(hex[i * 2]);
            const auto lo = HexToNibble(hex[i * 2 + 1]);

            out[i] = static_cast<uint8_t>((hi << 4) | lo);
        }

        return out;
    }
}
