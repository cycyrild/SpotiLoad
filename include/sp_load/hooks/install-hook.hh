#pragma once

#include <MinHook.h>
#include <cstdint>
#include <source_location>
#include <stdio.h>
#include <windows.h>

template <typename T>
bool InstallHook(HMODULE module,
                 uintptr_t rva,
                 T hook,
                 T *original,
                 const std::source_location &location = std::source_location::current())
{
    if (!module)
    {
        printf("[Hook Error]\n"
               "  File     : %s\n"
               "  Function : %s\n"
               "  Reason   : Invalid module\n",
               location.file_name(),
               location.function_name());

        return false;
    }

    auto target = (LPVOID)((uintptr_t)(module) + rva);

    MH_STATUS status = MH_CreateHook(target, (LPVOID)(hook), (LPVOID *)(original));

    if (status != MH_OK)
    {
        printf("[Hook Create Failed]\n"
               "  File     : %s\n"
               "  Function : %s\n"
               "  Error    : %s\n",
               location.file_name(),
               location.function_name(),
               MH_StatusToString(status));

        return false;
    }

    status = MH_EnableHook(target);

    if (status != MH_OK)
    {
        printf("[Hook Enable Failed]\n"
               "  File     : %s\n"
               "  Function : %s\n"
               "  Error    : %s\n",
               location.file_name(),
               location.function_name(),
               MH_StatusToString(status));

        MH_RemoveHook(target);
        return false;
    }

    return true;
}
