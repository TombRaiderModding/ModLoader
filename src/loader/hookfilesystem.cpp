#include "hookfilesystem.hpp"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

// so we don't need to include <Windows.h>
#define _MAX_PATH 260

namespace modloader
{
	HookFileSystem::HookFileSystem(cdc::FileSystem* pDiskFS)
	{
		m_pDiskFS = pDiskFS;
		m_specMask = 1;
	}

	// finds a file in the mods folder and rewrites the path
	bool HookFileSystem::FindFile(const char* fileName, char* path)
	{
		strcpy_s(path, _MAX_PATH, fileName);

		for (char* p = path; *p; ++p)
			*p = tolower(*p);

		// rewrite pc-w to mods folder
		if (strncmp(path, "pc-w", 4) == 0)
		{
			strncpy(path, "mods", 4);
		}

		// rewrite root to mods folder
		if (strncmp(path, "\\", 1) == 0)
		{
			// move string 4 bytes
			memmove(path + 4, path, _MAX_PATH - 4);
			strncpy(path, "mods", 4);
		}

		// check first for file on disk suffixed by our specialisation mask
		char specPath[_MAX_PATH];
		sprintf_s(specPath, "%s_%03d", path, m_specMask);

		if (m_pDiskFS->FileExists(specPath))
		{
			strcpy_s(path, _MAX_PATH, specPath);

			return true;
		}

		// check if file exists on disk, if so return the diskFS
		if (m_pDiskFS->FileExists(path))
		{
			return true;
		}

		return false;
	}

	void* HookFileSystem::RequestRead(void* receiver, const char* fileName, unsigned int startOffset)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->RequestRead(receiver, path, startOffset);
	}

	void* HookFileSystem::OpenFile(char const* fileName)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->OpenFile(path);
	}

	bool HookFileSystem::FileExists(char const* fileName)
	{
		char path[_MAX_PATH];
		return FindFile(fileName, path);
	}

	unsigned int HookFileSystem::GetFileSize(char const* fileName)
	{
		char path[_MAX_PATH];
		FindFile(fileName, path);

		return m_pDiskFS->GetFileSize(path);
	}

	void HookFileSystem::SetSpecialisationMask(unsigned int specMask)
	{
		m_pDiskFS->SetSpecialisationMask(specMask);

		// unset next generation bit and set our spec mask
		m_specMask = specMask & ~0x80000000;
	}

	unsigned int HookFileSystem::GetSpecialisationMask()
	{
		return 0;
	}

	int HookFileSystem::GetStatus()
	{
		return m_pDiskFS->GetStatus();
	}

	void HookFileSystem::Update()
	{
		m_pDiskFS->Update();
	}

	void HookFileSystem::Synchronize()
	{
		m_pDiskFS->Synchronize();
	}

	// only needed in TR8

	void HookFileSystem::Suspend()
	{
		m_pDiskFS->Suspend();
	}

	bool HookFileSystem::Resume()
	{
		return m_pDiskFS->Resume();
	}

	bool HookFileSystem::IsSuspended()
	{
		return m_pDiskFS->IsSuspended();
	}

	char* HookFileSystem::GetBufferPointer(void* request, unsigned int* bytesLocked)
	{
		return m_pDiskFS->GetBufferPointer(request, bytesLocked);
	}

	void HookFileSystem::ResetBufferPointer(int value)
	{
		return m_pDiskFS->ResetBufferPointer(value);
	}
}