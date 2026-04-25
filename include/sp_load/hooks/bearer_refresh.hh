#pragma once

#include <aggregator.hh>
#include <hooks/install_hook.hh>
#include <hooks/structs.hh>
#include <windows.h>

namespace sp_load::hooks::bearer_refresh
{
    struct OAuthTokenResponseMarshaled
    {
        RawStdString access_token;
        RawStdString token_type;
        uint64_t unknown;
        RawStdString refresh_token;
    };

    using MaterializeFn = OAuthTokenResponseMarshaled *(*)(void *field_specs,
                                                           OAuthTokenResponseMarshaled *out_response,
                                                           void *json_cursor);

    constexpr uintptr_t MATERIALIZE_RVA = 0xE8269C;

    bool Initialize(HMODULE spDll);
}
