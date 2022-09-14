#pragma once

#include <iostream>
#define DLL_IMPORT __declspec(dllimport)
#define DLL_EXPORT __declspec(dllexport)

#ifdef  BUILD_DLL
	DLL_EXPORT	int sum(int a, int b);
	DLL_EXPORT int minus(int a, int b);
#else
	DLL_IMPORT int sum(int a, int b);
	DLL_IMPORT int minus(int a, int b);
#endif //  BUILD_DLL

