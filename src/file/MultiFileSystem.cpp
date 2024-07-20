#include "MultiFileSystem.h"

#include <cassert>

MultiFileSystem::MultiFileSystem() : m_fileSystems()
{
}

void MultiFileSystem::Add(cdc::FileSystem* fileSystem, ...) noexcept
{
	m_fileSystems.insert(m_fileSystems.begin(), fileSystem);
}

cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName) const noexcept
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->FileExists(fileName))
		{
			return fileSystem;
		}
	}

	return nullptr;
}

cdc::FileRequest* MultiFileSystem::RequestRead(cdc::FileReceiver* receiver, const char* fileName, unsigned int startOffset)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->RequestRead(receiver, fileName, startOffset);
}

cdc::File* MultiFileSystem::OpenFile(const char* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->OpenFile(fileName);
}

bool MultiFileSystem::FileExists(const char* fileName)
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->FileExists(fileName))
		{
			return true;
		}
	}

	return false;
}

unsigned int MultiFileSystem::GetFileSize(const char* fileName)
{
	auto fileSystem = GetBestFileSystem(fileName);

	return fileSystem->GetFileSize(fileName);
}

void MultiFileSystem::SetSpecialisationMask(unsigned int specMask)
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->SetSpecialisationMask(specMask);
	}
}

unsigned int MultiFileSystem::GetSpecialisationMask()
{
	if (m_fileSystems.empty())
	{
		return 0xFFFFFFFF;
	}

	return m_fileSystems[0]->GetSpecialisationMask();
}

cdc::FileSystem::Status MultiFileSystem::GetStatus()
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->GetStatus() == BUSY)
		{
			return BUSY;
		}
	}

	return IDLE;
}

void MultiFileSystem::Update()
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->Update();
	}
}

void MultiFileSystem::Synchronize()
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->Synchronize();
	}
}

void MultiFileSystem::Suspend()
{
	for (auto fileSystem : m_fileSystems)
	{
		fileSystem->Suspend();
	}
}

bool MultiFileSystem::Resume()
{
	for (auto fileSystem : m_fileSystems)
	{
		if (!fileSystem->Resume())
		{
			return false;
		}
	}

	return true;
}

bool MultiFileSystem::IsSuspended()
{
	for (auto fileSystem : m_fileSystems)
	{
		if (fileSystem->IsSuspended())
		{
			return true;
		}
	}

	return false;
}

char* MultiFileSystem::GetBufferPointer(cdc::FileRequest* request, unsigned int* bytesLocked)
{
	assert(!"Not implemented");

	return nullptr;
}

void MultiFileSystem::ResetBufferPointer(cdc::FileRequest* request)
{
	assert(!"Not implemented");
}