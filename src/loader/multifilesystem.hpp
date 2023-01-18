#pragma once

#include "../game/filesystem.hpp"

// very simple version of cdc::MultiFileSystem since it does not exist in Legend and Anniversary
// the real MultiFileSystem can dynamically add and re-order systems

namespace modloader
{
	class MultiFileSystem : public cdc::FileSystem
	{
	private:
		cdc::FileSystem* m_pFS;
		cdc::FileSystem* m_pHookFS;

		cdc::FileSystem* GetBestFileSystem(const char* fileName);
	public:
		MultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pHookFS);

		virtual void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset);
		virtual void* OpenFile(char const* fileName);
		virtual bool FileExists(char const* fileName);
		virtual unsigned int GetFileSize(char const* fileName);
		virtual void SetSpecialisationMask(unsigned int specMask);
		virtual unsigned int GetSpecialisationMask();
		virtual int GetStatus();
		virtual void Update();
		virtual void Synchronize();

		// unused since this filesystem will never run in TR8
		virtual void Suspend() { }
		virtual bool Resume() { return false; }
		virtual bool IsSuspended() { return false; }
	};
}