#include <hooks/bearer-refresh.hh>

// This will be useful if we need to make authenticated requests

namespace sp_load::hooks::bearer_refresh
{
    MaterializeFn Original = nullptr;

    OAuthTokenResponseMarshaled *Hook(void *field_specs, OAuthTokenResponseMarshaled *out_response, void *json_cursor)
    {
        auto ret = Original(field_specs, out_response, json_cursor);

        if (!out_response)
            return ret;

        auto access = out_response->access_token.view();
        auto refresh = out_response->refresh_token.view();

        if (access.size() > 0)
            printf("[Access Token] %.*s\n", (int)access.size(), access.data());

        if (refresh.size() > 0)
            printf("[Refresh Token] %.*s\n", (int)refresh.size(), refresh.data());

        return ret;
    }

    bool Initialize(HMODULE spDll)
    {
        return InstallHook(spDll, MATERIALIZE_RVA, &Hook, &Original);
    }
}
