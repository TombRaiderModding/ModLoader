#include <Windows.h>
#include "loader.hpp"

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	auto& loader = modloader::Loader::GetInstance();

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			if (!loader.Initialize())
			{
				return FALSE;
			}

			break;
		case DLL_PROCESS_DETACH:
			loader.~Loader();

			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
	}

	return TRUE;
}