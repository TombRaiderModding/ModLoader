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

		virtual void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset);
		virtual void* OpenFile(char const* fileName);
		virtual bool FileExists(char const* fileName);
		virtual unsigned int GetFileSize(char const* fileName);
		virtual void SetSpecialisationMask(unsigned int specMask);
		virtual unsigned int GetSpecialisationMask();
		virtual int GetStatus();
		virtual void Update();
		virtual void Synchronize();

		virtual void Suspend();
		virtual bool Resume();
		virtual bool IsSuspended();
	};
}