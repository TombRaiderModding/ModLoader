#include <Windows.h>
#include <MinHook.h>

#include "loader.hpp"

static bool hit = false;
static HMODULE hLibModule;

static void(WINAPI* orgGetStartupInfoW)(LPSTARTUPINFOW lpStartupInfo);

void WINAPI hookedGetStartupInfoW(LPSTARTUPINFOW lpStartupInfo)
{
	if (!hit)
	{
		hit = true;

		auto& loader = modloader::Loader::GetInstance();
		if (!loader.Initialize())
		{
			// clean up if we cannot initialize
			FreeLibraryAndExitThread(hLibModule, 0);
		}
	}

	orgGetStartupInfoW(lpStartupInfo);
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			hLibModule = hModule;

			// due to SecuROM we must hook a CRT function so we can insert our hooks
			// after the game has been done unpacking
			MH_Initialize();
			MH_CreateHookApi(L"Kernel32", "GetStartupInfoA", hookedGetStartupInfoW, (void**)&orgGetStartupInfoW);
			MH_EnableHook(MH_ALL_HOOKS);

			break;
		case DLL_PROCESS_DETACH:
			{
				auto& loader = modloader::Loader::GetInstance();
				loader.~Loader();
			}
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}