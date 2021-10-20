#include <framework.h>

namespace GameHook
{
    // Multi Skill Equip
    const SHORT bytePattern[] = { 0x45, 0x33, 0xC0, 0x49, 0x8B, 0xCA, 0xE8, 0x8B, 0xB3 };

    const SHORT patternLength = 9;

    const SHORT patternOffset = 0;

    // Name of the module in which to scan for byte patterns.
    const WCHAR moduleName[] = L"ShadowOfWar.exe";

    AOBScanner pScanner;

    void Set(const BYTE* writeAddress, const std::vector<BYTE>& newBytes)
    {
        MEMORY_BASIC_INFORMATION mbi{};
        if (!VirtualQuery(writeAddress, &mbi, sizeof(mbi)) || mbi.State != MEM_COMMIT || mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD))
            return;

        DWORD oldProtect;
        if(!VirtualProtect((LPVOID)writeAddress, newBytes.size(), PAGE_READWRITE, &oldProtect))
            return;

        WriteProcessMemory(GetCurrentProcess(), (LPVOID)writeAddress, newBytes.data(), newBytes.size(), nullptr);
        VirtualProtect((LPVOID)writeAddress, newBytes.size(), oldProtect, &oldProtect);
    }

    // Injections and scanned addresses.
    extern "C"
    {
        UINT_PTR hitStreakJmpAddress = 0;
        EXPORT BYTE* hitStreakAddress = nullptr;

        BYTE shouldSkip = 0;
        INSTRUCTIONSET HitStreakHook() // Doesn't really work how I thought it would.
        {
            __asm__
            (
                "push %rax\n\t"
                "mov shouldSkip, %al\n\t"
                "test %al, %al\n\t"
                "je 1f\n\t"
                "xor %r8d, %r8d\n\t"
                "mov $0, %al\n\t"
                "mov %al, shouldSkip\n\t"
                "jmp 2f\n\t"
                "1:\n\t"
                "mov $1, %al\n\t"
                "mov %al, shouldSkip\n\t"
                "2:\n\t"
                "pop %rax\n\t"
                "mov %r10, %rcx\n\t"
                "jmp *hitStreakJmpAddress"
            );
        }
    }

    void CreateHitStreakHook()
    {
        hitStreakAddress = pScanner.PerformModuleScan(bytePattern, patternLength, patternOffset, moduleName);
        hitStreakJmpAddress = (UINT_PTR)hitStreakAddress + 6;
        // MH_CreateHook(hitStreakAddress, (LPVOID)HitStreakHook, nullptr);
    }

    // Hook startup.
    void CreateGameHooks()
    {
        CreateHitStreakHook();
        Set(hitStreakAddress, {0x90, 0x90, 0x90});
        // MH_EnableHook(MH_ALL_HOOKS);
    }
}