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

        const auto &access = out_response->access_token;
        const auto &refresh = out_response->refresh_token;

        if (out_response->access_token.view().size() > 0)
            printf("[Access Token] %.*s\n", (int)access.view().size(), access.view().data());

        if (refresh.view().size() > 0)
            printf("[Refresh Token] %.*s\n", (int)refresh.view().size(), refresh.view().data());

        return ret;
    }

    bool Initialize(HMODULE spDll)
    {
        return InstallHook(spDll, MATERIALIZE_RVA, &Hook, &Original);
    }
}
