#pragma once

#include "FileSystem.h"

namespace cdc
{
	class MultiFileSystem
	{
	public:
		void Add(FileSystem* fileSystem, bool bReprioritize, bool bAddToFront) const noexcept;
	};
}