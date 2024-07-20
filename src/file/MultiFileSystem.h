#pragma once

#include <vector>
#include "cdc/FileSystem.h"

class MultiFileSystem : public cdc::FileSystem
{
private:
	std::vector<cdc::FileSystem*> m_fileSystems;

	cdc::FileSystem* GetBestFileSystem(const char* fileName) const noexcept;

public:
	MultiFileSystem();

	// ... to be compatible with function definition of base game
	void Add(cdc::FileSystem* fileSystem, ...) noexcept;

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