#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZEL_API __declspec(dllexport)
	#else
		#define HAZEL_API __declspec(dllimport)
	#endif // HZ_BUILD_DLL
#else
#error Hazel only supports windows!
#endif // HZ_PLATFORM_WINDOWS

#ifdef HZ_ENABLE_ASSERT
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif // HZ_ENABLE_ASSERT


#define BIT(x) (1 << x)