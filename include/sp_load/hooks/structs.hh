#pragma once
#include <array>
#include <cstdint>
#include <string_view>

namespace sp_load::hooks
{
    struct RawStdString
    {
      private:
        union
        {
            char *heap_ptr;
            char sso[16];
        };

        uint64_t size;
        uint64_t capacity;

      public:
        std::string_view view() const
        {
            return capacity > 15 ? std::string_view(heap_ptr, size) : std::string_view(sso, size);
        }

        std::string_view view()
        {
            return capacity > 15 ? std::string_view(heap_ptr, size) : std::string_view(sso, size);
        }
    };

    static_assert(sizeof(RawStdString) == 0x20);

    struct OptionalInt32
    {
        int32_t value;
        uint8_t has_value;
        std::array<uint8_t, 0x3> _pad;
    };
}
