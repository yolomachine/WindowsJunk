#pragma once

#ifdef MEMMAPDLL_EXPORTS
#define MEMMAPDLL_API __declspec(dllexport) 
#else
#define MEMMAPDLL_API __declspec(dllimport) 
#endif

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <windows.h>

namespace MemoryMapping {
    class SortGenerateUtil {
    public:
        static MEMMAPDLL_API void generateFile(LPCWSTR path, INT n);

        static MEMMAPDLL_API void sortFile(LPCWSTR path);
    };
}