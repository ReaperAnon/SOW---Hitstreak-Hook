#include <framework.h>

DLLHOOK OnLoad()
{
	GameHook::CreateGameHooks();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
	if(callReason == DLL_PROCESS_ATTACH)
		DisableThreadLibraryCalls(hModule);

    return TRUE;
}