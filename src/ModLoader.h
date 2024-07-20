#pragma once

class ModLoader
{
private:

	bool m_initialized = false;

	~ModLoader() noexcept;

	void ShowError() const noexcept;

public:
	static ModLoader& GetInstance() noexcept;

	void Initialize() noexcept;
	bool IsInitialized() const noexcept;
};

struct VirtualClass
{
	void** vftable;
};