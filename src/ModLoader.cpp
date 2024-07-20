#include "ModLoader.h"

#include <Windows.h>
#include <MinHook.h>
#include <Hooking.Patterns.h>

#include "cdc/FileSystem.h"
#include "cdc/MultiFileSystem.h"
#include "file/MultiFileSystem.h"
#include "file/HookFileSystem.h"
#include "util/Hooking.h"

static void* s_getFS = nullptr;
static void* s_target = nullptr;

static cdc::FileSystem** s_diskFS = nullptr;

// Gets the current file system
static cdc::FileSystem* GetFS() noexcept
{
	return Hooking::CallReturn<cdc::FileSystem*>(s_getFS);
}

// Patch the virtual function table in Legend to switch the newly added methods with the destructor
static void PatchVFTable(cdc::FileSystem* fileSystem)
{
	auto raw = (VirtualClass*)fileSystem;

	DWORD oldProtect;
	VirtualProtect(raw->vftable, 15 * sizeof(void*), PAGE_READWRITE, &oldProtect);

	// Swap 'Suspend' for '~FileSystem'
	raw->vftable[9] = raw->vftable[14];

	VirtualProtect(raw->vftable, 15 * sizeof(void*), oldProtect, &oldProtect);
}

// Overrides the default file system used by the game
static void SetFileSystem(cdc::FileSystem* multiFileSystem)
{
	// A1 8C 88 83 00	mov eax, g_pFS
	auto fileSystem = *(cdc::FileSystem***)((char*)s_getFS + 1);

	*fileSystem = multiFileSystem;
}

// Insert the hook file system and remove the current hook
template<typename FileSystem>
static void InsertAndUnhook() noexcept
{
	if (std::is_same<FileSystem, MultiFileSystem>())
	{
		// Create new multi file system if it doesn't exist in current game
		auto multiFileSystem = new MultiFileSystem();
		multiFileSystem->Add(GetFS());

		PatchVFTable(multiFileSystem);

		// Override the default file system with ours
		SetFileSystem(multiFileSystem);
	}

	// Create the hook file system
	auto fileSystem = new HookFileSystem(*s_diskFS);

	// Add it to the system
	auto multiFileSystem = (FileSystem*)GetFS();
	multiFileSystem->Add(fileSystem, false, true);

	// Now GTFO so we don't trigger any DRM which might be hanging around
	MH_RemoveHook(s_target);
}

// Hooks
static void(*s_InitArchive)();
static bool(*s_InitPatchArchive)(const char*);
static void(__thiscall* s_InitDlcSystem)(void*);

// TR7 and TRA
static void InitArchive()
{
	s_InitArchive();

	InsertAndUnhook<MultiFileSystem>();
}

// TR8
static bool InitPatchArchive(const char* name)
{
	auto ret = s_InitPatchArchive(name);

	InsertAndUnhook<cdc::MultiFileSystem>();

	return ret;
}

// LCGOL
static void __fastcall InitDlcSystem(void* _this)
{
	s_InitDlcSystem(_this);

	InsertAndUnhook<cdc::MultiFileSystem>();
}

ModLoader::~ModLoader() noexcept
{
	MH_Uninitialize();
}

ModLoader& ModLoader::GetInstance() noexcept
{
	static ModLoader instance;
	return instance;
}

void ModLoader::Initialize() noexcept
{
	m_initialized = true;

	// Scan for all patterns
	auto getFS = hook::pattern("E8 ? ? ? ? 50 8D 4C 24 08 6A 00 51").count_hint(1);

	auto initArchive = hook::pattern("84 C0 75 0B 68 ? ? ? ? E8 ? ? ? ? 59 C3").count_hint(1);
	auto initPatch = hook::pattern("56 57 E8 ? ? ? ? 8B 7C 24 0C 8B").count_hint(1);
	auto initDlc = hook::pattern("8B 0E 6A 00 6A 00 52 E8 ? ? ? ? A1").count_hint(1);

	if (getFS.empty() || !(initArchive.empty() || initPatch.empty() || initDlc.empty()))
	{
		ShowError();

		return;
	}

	s_getFS = Hooking::GetAddress(getFS.get_first());

	// Insert hooks
	if (!initArchive.empty())
	{
		s_target = initArchive.get_first(-68);

		MH_CreateHook(s_target, InitArchive, (void**)&s_InitArchive);
	}

	if (!initPatch.empty())
	{
		s_target = initPatch.get_first();

		MH_CreateHook(s_target, InitPatchArchive, (void**)&s_InitPatchArchive);
	}

	if (!initDlc.empty())
	{
		s_target = initDlc.get_first(-88);

		MH_CreateHook(s_target, InitDlcSystem, (void**)&s_InitDlcSystem);
	}

	// Find the disk file system
	auto legend = hook::pattern("8B 15 ? ? ? ? 33 C9 89 4C 24 01").count_hint(1);
	auto underworld = hook::pattern("8B 0D ? ? ? ? 89 35 ? ? ? ? 3B CE").count_hint(1);

	if (legend.empty() && underworld.empty())
	{
		ShowError();

		return;
	}

	if (!legend.empty())
	{
		s_diskFS = *legend.get_first<cdc::FileSystem**>(2);
	}

	if (!underworld.empty())
	{
		s_diskFS = *underworld.get_first<cdc::FileSystem**>(8);
	}

	MH_EnableHook(MH_ALL_HOOKS);
}

void ModLoader::ShowError() const noexcept
{
	MessageBoxA(
		NULL,
		"This version is not compatible, please open an issue on GitHub if this game is listed as supported.",
		"Gane not compatible",
		MB_OK | MB_ICONERROR);
}

bool ModLoader::IsInitialized() const noexcept
{
	return m_initialized;
}