#pragma once

#include "cdc/FileSystem.h"

#include <stdlib.h>

class HookFileSystem : public cdc::FileSystem
{
private:
	cdc::FileSystem* m_diskFileSystem;
	unsigned int m_specMask;
	unsigned int m_language;

	char m_path[_MAX_PATH];

	bool FindFile(const char* fileName, char* outPath) noexcept;

public:
	HookFileSystem(cdc::FileSystem* diskFileSystem);

	cdc::FileRequest* RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset);
	cdc::File* OpenFile(const char* fileName);
	bool FileExists(const char* fileName);
	unsigned int GetFileSize(const char* fileName);
	void SetSpecialisationMask(unsigned int specMask);
	unsigned int GetSpecialisationMask();
	Status GetStatus();
	void Update();
	void Synchronize();

	void Suspend();
	bool Resume();
	bool IsSuspended();
	char* GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked);
	void ResetBufferPointer(cdc::FileRequest* request);
};