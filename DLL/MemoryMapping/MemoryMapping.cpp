#include "stdafx.h"
#include "MemoryMapping.h"

#define EXCLUSIVE_ACCESS 0

namespace MemoryMapping {

    MEMMAPDLL_API void SortGenerateUtil::generateFile(LPCWSTR path, INT n = 1000) {
        std::ofstream o(path, std::ios_base::binary);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100);

        for (size_t i = 0; i < n; ++i) {
            const auto value = dist(gen);
            o.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }

        o.close();
    }

    MEMMAPDLL_API void SortGenerateUtil::sortFile(LPCWSTR path) {
        HANDLE hFile = CreateFile(path, GENERIC_READ | GENERIC_WRITE, EXCLUSIVE_ACCESS, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == NULL) {
            std::cerr << "hFile is NULL: " << GetLastError();
            return;
        }

        size_t fileSize = GetFileSize(hFile, NULL);

        HANDLE hFileMappingObject = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, path);
        if (hFileMappingObject == NULL) {
            std::cerr << "hFileMappingObject is NULL: " << GetLastError();
            return;
        }

        LPVOID lpMapAddress = MapViewOfFile(hFileMappingObject, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (lpMapAddress == NULL) {
            std::cerr << "lpMapAddress NULL: " << GetLastError();
            return;
        }

        LPINT lpMappedData = reinterpret_cast<LPINT>(lpMapAddress);

        std::qsort(lpMappedData,
                   fileSize / sizeof(*lpMappedData),
                   sizeof(*lpMappedData),
                   [](const void* a, const void* b) {
            int arg1 = *static_cast<const int*>(a);
            int arg2 = *static_cast<const int*>(b);

            if (arg1 < arg2) return -1;
            if (arg2 < arg1) return 1;

            return 0;
        });

        UnmapViewOfFile(lpMapAddress);
        CloseHandle(hFileMappingObject);
        CloseHandle(hFile);
    }

}