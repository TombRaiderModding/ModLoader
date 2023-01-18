#pragma once

#include <Hooking.Patterns.h>

namespace modloader
{
	class Loader
	{
	private:
		Loader() noexcept;

		void InsertLegendHooks(hook::pattern& matches) const noexcept;
		void InsertUnderworldHooks(hook::pattern& matches) const noexcept;

		void ShowError() const noexcept;

		template<typename T>
		T GetAddress(void* ptr) const noexcept;

	public:
		static Loader& GetInstance()
		{
			static Loader loader;
			return loader;
		}

		~Loader() noexcept;

		bool Initialize() const noexcept;
	};
}