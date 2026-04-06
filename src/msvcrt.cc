#include <windows.h>

extern "C" void abort(void)
{
    TerminateProcess(GetCurrentProcess(), 3);
    __builtin_unreachable();
}
