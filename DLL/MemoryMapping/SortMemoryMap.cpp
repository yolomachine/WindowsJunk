#include "stdafx.h"
#include "MemoryMapping.h"

int main() {
    MemoryMapping::SortGenerateUtil::generateFile(L"test", 100);
    MemoryMapping::SortGenerateUtil::sortFile(L"test");
}

