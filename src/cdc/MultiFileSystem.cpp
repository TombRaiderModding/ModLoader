#include "MultiFileSystem.h"

#include "util/Hooking.h"

#include <Hooking.Patterns.h>

void cdc::MultiFileSystem::Add(FileSystem* fileSystem, bool bReprioritize, bool bAddToFront)
{
	auto func = hook::get_pattern<void>("80 7C 24 0C 00 8B C1 74 5A 53");

	Hooking::ThisCall(func, this, fileSystem, bReprioritize, bAddToFront);
}