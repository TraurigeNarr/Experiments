#pragma once

namespace Templates
	{
	template <typename T>
	struct Test
		{
		template <typename... Args>
		static T RetFunc(Args... args);
		};

	template <>
	struct Test < float >
		{
		template <typename... Args>
		static float RetFunc(Args... args)
			{
			return 1.f;
			}
		};

	template <typename T>
	T ReadType(char* ip_addr, size_t i_offset)
	{
		return *reinterpret_cast<T*>(ip_addr + i_offset);
	}

	void TestFunction();

	} // Templates