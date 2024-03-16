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

		void* RequestRead(void* receiver, const char* fileName, unsigned int startOffset);
		void* OpenFile(char const* fileName);
		bool FileExists(char const* fileName);
		unsigned int GetFileSize(char const* fileName);
		void SetSpecialisationMask(unsigned int specMask);
		unsigned int GetSpecialisationMask();
		int GetStatus();
		void Update();
		void Synchronize();

		// unused since this filesystem will never run in TR8
		void Suspend() { }
		bool Resume() { return false; }
		bool IsSuspended() { return false; }
		char* GetBufferPointer(void* request, unsigned int* bytesLocked) { return nullptr; }
		void ResetBufferPointer(int value) { }
	};
}