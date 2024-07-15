#pragma once

// Helpers for calling functions
class Hooking
{
public:
	// Calls a function
	template<typename... Args>
	static inline void Call(void* address, Args... args) noexcept
	{
		reinterpret_cast<void(*)(Args...)>(address)(args...);
	}

	// Calls a function with a return value
	template<typename T, typename... Args>
	static inline T CallReturn(void* address, Args... args) noexcept
	{
		return reinterpret_cast<T(*)(Args...)>(address)(args...);
	}

	// Calls a class function
	template<typename... Args>
	static inline void ThisCall(void* address, Args... args) noexcept
	{
		reinterpret_cast<void(__thiscall*)(Args...)>(address)(args...);
	}

	// Calls a class function with a return value
	template<typename T, typename... Args>
	static inline T ThisCallReturn(void* address, Args... args) noexcept
	{
		return reinterpret_cast<T(__thiscall*)(Args...)>(address)(args...);
	}

	// Gets an address at a call instruction
	template<typename T = void*>
	static inline T GetAddress(void* ptr) noexcept
	{
		return (T)((__int32)ptr + *(__int32*)((__int32)ptr + 1) + 5);
	}
};