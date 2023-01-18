#include "loader.hpp"

#include <Windows.h>
#include <MinHook.h>
#include <Hooking.Patterns.h>

#include "game/filesystem.hpp"
#include "loader/multifilesystem.hpp"
#include "loader/hookfilesystem.hpp"

static cdc::FileSystem* g_pDiskFS;

// orginal functions
static cdc::FileSystem* (*GetFS)();
static bool(__cdecl* InitPatchArchive)(const char* name);

// hooked GetFS for Legend to return our own MultiFileSystem
cdc::FileSystem* hookedGetFS()
{
	static auto pHookFS = new modloader::HookFileSystem(*(cdc::FileSystem**)g_pDiskFS);
	static auto pFS = new modloader::MultiFileSystem(GetFS(), pHookFS);

	return pFS;
}

// hooked InitPatchArchive so we can insert ourselves to the system
bool hookedInitPatchArchive(const char* name)
{
	static auto pHookFS = new modloader::HookFileSystem(*(cdc::FileSystem**)g_pDiskFS);

	auto ret = InitPatchArchive(name);
	auto pFS = reinterpret_cast<cdc::MultiFileSystem*>(GetFS());

	// add the hook filesystem to the front of the system
	pFS->Add(pHookFS, false, true);

	return ret;
}

namespace modloader
{
	Loader::Loader() noexcept
	{
		MH_Initialize();
	}

	Loader::~Loader() noexcept
	{
		MH_Uninitialize();
	}

	bool Loader::Initialize() const noexcept
	{
		auto getFS = hook::pattern("E8 ? ? ? ? 50 8D 4C 24 08 6A 00 51");

		// check if we can find the GetFS function
		if (getFS.count_hint(1).empty())
		{
			ShowError();

			return false;
		}

		GetFS = GetAddress<cdc::FileSystem*(*)()>(getFS.get_first());

		// continue by finding pDiskFS, which we can also use to check which game we are in
		auto& tr7Pattern = hook::pattern("E8 ? ? ? ? EB 02 33 C0 68 ? ? ? ? 8B C8 A3").count_hint(1);
		auto& tr8Pattern = hook::pattern("89 35 ? ? ? ? 3B CE 5E 74 09 8B 01 8B 50 38 6A 01 FF D2 C3").count_hint(1);

		if (tr7Pattern.empty() && tr8Pattern.empty())
		{
			ShowError();

			return false;
		}

		InsertLegendHooks(tr7Pattern);
		InsertUnderworldHooks(tr8Pattern);

		MH_EnableHook(MH_ALL_HOOKS);

		return true;
	}

	void Loader::InsertLegendHooks(hook::pattern& matches) const noexcept
	{
		if (matches.empty()) return;

		g_pDiskFS = *matches.get(0).get<cdc::FileSystem*>(-7);

		// hook GetFS and make sure we override GetFS too
		MH_CreateHook(GetFS, hookedGetFS, (void**)&GetFS);
	}

	void Loader::InsertUnderworldHooks(hook::pattern& matches) const noexcept
	{
		if (matches.empty()) return;

		g_pDiskFS = *matches.get(0).get<cdc::FileSystem*>(2);

		// hook InitPatchArchive so we can add ourselves to the system
		MH_CreateHook(hook::get_pattern("56 57 E8 ? ? ? ? 8B 7C 24 0C"), hookedInitPatchArchive, (void**)&InitPatchArchive);
	}

	void Loader::ShowError() const noexcept
	{
		MessageBoxA(nullptr,
			"This version is not compatible, please open an issue on GitHub if this game is listed as supported.",
			"Pattern not found", MB_OK | MB_ICONERROR);
	}

	template<typename T>
	T Loader::GetAddress(void* ptr) const noexcept
	{
		return (T)((__int32)ptr + *(__int32*)((__int32)ptr + 1) + 5);
	}
}