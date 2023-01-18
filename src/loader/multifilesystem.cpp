#include "multifilesystem.hpp"

namespace modloader
{
	MultiFileSystem::MultiFileSystem(cdc::FileSystem* pFS, cdc::FileSystem* pHookFS)
	{
		m_pFS = pFS;
		m_pHookFS = pHookFS;
	}

	// gets the best filesystem simply by checking the hook filesystem first
	cdc::FileSystem* MultiFileSystem::GetBestFileSystem(const char* fileName)
	{
		// first check hook filesystem, else fall back to default filesystem
		if (m_pHookFS->FileExists(fileName))
		{
			return m_pHookFS;
		}

		return m_pFS;
	}

	void* MultiFileSystem::RequestRead(void* receiver, const char* fileName, unsigned int startOffset)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->RequestRead(receiver, fileName, startOffset);
	}

	void* MultiFileSystem::OpenFile(char const* fileName)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->OpenFile(fileName);
	}

	bool MultiFileSystem::FileExists(char const* fileName)
	{
		return m_pFS->FileExists(fileName) || m_pHookFS->FileExists(fileName);
	}

	unsigned int MultiFileSystem::GetFileSize(char const* fileName)
	{
		auto pFS = GetBestFileSystem(fileName);

		return pFS->GetFileSize(fileName);
	}

	void MultiFileSystem::SetSpecialisationMask(unsigned int specMask)
	{
		m_pFS->SetSpecialisationMask(specMask);
		m_pHookFS->SetSpecialisationMask(specMask);
	}

	unsigned int MultiFileSystem::GetSpecialisationMask()
	{
		return m_pFS->GetSpecialisationMask();
	}

	// only need to call the default filesystem, both will end at the same place
	int MultiFileSystem::GetStatus()
	{
		return m_pFS->GetStatus();
	}

	void MultiFileSystem::Update()
	{
		m_pFS->Update();
	}

	void MultiFileSystem::Synchronize()
	{
		m_pFS->Synchronize();
	}
}