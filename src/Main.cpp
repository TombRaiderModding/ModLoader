#include <Windows.h>
#include <MinHook.h>

#include "ModLoader.h"

static VOID(WINAPI* s_GetStartupInfoA)(LPSTARTUPINFOA);

static VOID WINAPI StartupInfoW(LPSTARTUPINFOA lpStartupInfo)
{
	auto& loader = ModLoader::GetInstance();

	if (!loader.IsInitialized())
	{
		loader.Initialize();
	}

	return s_GetStartupInfoA(lpStartupInfo);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		MH_Initialize();

		// Insert an early hook on GetStartupInfoA which will be called when the executable is done unpacking
		// Some of the executables are packed, so we can't immediately insert our hooks
		MH_CreateHookApi(L"kernel32", "GetStartupInfoA", StartupInfoW, (void**)&s_GetStartupInfoA);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	return TRUE;
}