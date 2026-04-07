#pragma once

namespace sp_load
{
    // Static storage + placement new to avoid global constructors,
    // runtime dependencies, and static destruction ...

    template <typename T>
    T &StaticInstance()
    {
        alignas(T) static unsigned char storage[sizeof(T)];
        static T &instance = *new (storage) T();
        return instance;
    }
}
