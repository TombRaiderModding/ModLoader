#include "filesystem.hpp"

#include <Hooking.Patterns.h>

namespace cdc
{
	void MultiFileSystem::Add(cdc::FileSystem* fs, bool reprioritize, bool insertFirst)
	{
		auto addr = hook::get_pattern<void>("80 7C 24 0C 00 8B C1 74 5A 53");
		auto func = reinterpret_cast<void(__thiscall*)(void*, cdc::FileSystem*, bool, bool)>(addr);

		func(this, fs, reprioritize, insertFirst);
	}
}