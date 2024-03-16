#pragma once

#include "../game/filesystem.hpp"

// filesystem which will look for files in the mods folder if a file is found
// it will pass the request to the MSFileSystem and rewrite the path to the mods folder

namespace modloader
{
	class HookFileSystem : public cdc::FileSystem
	{
	private:
		cdc::FileSystem* m_pDiskFS;
		unsigned int m_specMask;

		bool FindFile(const char* fileName, char* path);
	public:
		HookFileSystem(cdc::FileSystem* pDiskFS);

		void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset);
		void* OpenFile(char const* fileName);
		bool FileExists(char const* fileName);
		unsigned int GetFileSize(char const* fileName);
		void SetSpecialisationMask(unsigned int specMask);
		unsigned int GetSpecialisationMask();
		int GetStatus();
		void Update();
		void Synchronize();

		void Suspend();
		bool Resume();
		bool IsSuspended();
		char* GetBufferPointer(void* request, unsigned int* bytesLocked);
		void ResetBufferPointer(int value);
	};
}