#pragma once

#include <Hooking.Patterns.h>

namespace modloader
{
	class Loader
	{
	private:
		void InsertLegendHooks(hook::pattern matches);
		void InsertUnderworldHooks(hook::pattern matches);

		void ShowError();

		template<typename T>
		T GetAddress(void* ptr);

	public:
		static Loader& GetInstance()
		{
			static Loader loader;
			return loader;
		}

		Loader();
		~Loader();

		bool Initialize();
	};
}