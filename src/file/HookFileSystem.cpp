#include "HookFileSystem.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <cassert>

HookFileSystem::HookFileSystem(cdc::FileSystem* diskFileSystem) : m_path()
{
	assert(diskFileSystem != nullptr);

	m_diskFileSystem = diskFileSystem;
	m_specMask = 1;
	m_language = 1;
}

bool HookFileSystem::FindFile(const char* fileName, char* outPath)
{
	strcpy_s(outPath, _MAX_PATH, fileName);

	for (char* p = outPath; *p; ++p)
		*p = tolower(*p);

	// Rewrite pc-w to mods folder
	if (strncmp(outPath, "pc-w", 4) == 0)
	{
		strncpy(outPath, "mods", 4);
	}

	// Rewrite root to mods folder
	if (strncmp(outPath, "\\", 1) == 0)
	{
		// Move string 4 bytes
		memmove(outPath + 4, outPath, _MAX_PATH - 4);
		strncpy(outPath, "mods", 4);
	}

	// Check first for file on disk suffixed by our specialisation mask
	char specPath[_MAX_PATH];
	sprintf_s(specPath, "%s_%03d", outPath, m_language);

	if (m_diskFileSystem->FileExists(specPath))
	{
		strcpy_s(outPath, _MAX_PATH, specPath);

		return true;
	}

	// Check if file exists on disk, if so return the diskFS
	if (m_diskFileSystem->FileExists(outPath))
	{
		return true;
	}

	return false;
}

cdc::FileRequest* HookFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	FindFile(fileName, m_path);

	return m_diskFileSystem->RequestRead(receiver, m_path, startOffset);
}

cdc::File* HookFileSystem::OpenFile(const char* fileName)
{
	FindFile(fileName, m_path);

	return m_diskFileSystem->OpenFile(m_path);
}

bool HookFileSystem::FileExists(const char* fileName)
{
	return FindFile(fileName, m_path);
}

unsigned int HookFileSystem::GetFileSize(const char* fileName)
{
	FindFile(fileName, m_path);

	return m_diskFileSystem->GetFileSize(m_path);
}

void HookFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	m_specMask = specMask;

	//  Unset the next generation bit
	m_language = specMask & ~0x80000000;
}

unsigned int HookFileSystem::GetSpecialisationMask()
{
	return m_specMask;
}

cdc::FileSystem::Status HookFileSystem::GetStatus()
{
	return m_diskFileSystem->GetStatus();
}

void HookFileSystem::Update()
{
	m_diskFileSystem->Update();
}

void HookFileSystem::Synchronize()
{
	m_diskFileSystem->Synchronize();
}

void HookFileSystem::Suspend()
{
	m_diskFileSystem->Suspend();
}

bool HookFileSystem::Resume()
{
	return m_diskFileSystem->Resume();
}

bool HookFileSystem::IsSuspended()
{
	return m_diskFileSystem->IsSuspended();
}

char* HookFileSystem::GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked)
{
	return m_diskFileSystem->GetBufferPointer(request, bytesLocked);
}

void HookFileSystem::ResetBufferPointer(cdc::FileRequest* request)
{
	m_diskFileSystem->ResetBufferPointer(request);
}