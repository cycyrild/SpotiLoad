#include <std-out.hh>

HANDLE hConsole;

void InitializeConsole()
{
    AllocConsole();
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

extern "C" void putchar_(char c)
{
    DWORD written;

    WriteFile(hConsole, &c, 1, &written, nullptr);
}
