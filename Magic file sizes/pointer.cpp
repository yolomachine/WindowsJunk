#include <windows.h>
#include <string>
#include <iostream>

HANDLE CreateSparseFile(LPCTSTR lpSparseFileName) {
    DWORD dwTemp;

    HANDLE hSparseFile = CreateFile(lpSparseFileName, 
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hSparseFile == INVALID_HANDLE_VALUE)
        return hSparseFile;

    DeviceIoControl(hSparseFile,
        FSCTL_SET_SPARSE,
        NULL,
        0,
        NULL,
        0,
        &dwTemp,
        NULL);
    return hSparseFile;
}

DWORD SetSparseRange(HANDLE hSparseFile, LONGLONG start, ULONGLONG size) {
    FILE_ZERO_DATA_INFORMATION fzdi;
    fzdi.FileOffset.QuadPart = start;
    fzdi.BeyondFinalZero.QuadPart = start + size;

    DWORD dwTemp;
    SetLastError(0);
    BOOL bStatus = DeviceIoControl(hSparseFile,
        FSCTL_SET_ZERO_DATA,
        &fzdi,
        sizeof(fzdi),
        NULL,
        0,
        &dwTemp,
        NULL);
    if (bStatus) 
        return 0;
    else {
        DWORD e = GetLastError();
        return(e);
    }
}

int main(int argc, char* argv[]) {
    try {
        ULONGLONG size = (argc >= 3)? std::stoull(argv[2]) : 1844674407370955161;
        HANDLE h = CreateSparseFile((argc >= 2)? argv[1] : "sparse");
        if (h == INVALID_HANDLE_VALUE) {
            std::cerr << "Unable to create file" << std::endl;
            return 1;
        }
        if (SetSparseRange(h, 0, size) != 0) {
            std::cerr << "Unable to set sparse range" << std::endl;
            return 1;
        }
        ULARGE_INTEGER seek;
        seek.QuadPart = size;
        if (!SetFilePointerEx(h, seek, 0, 0)) {
            std::cerr << "Unable to seek to desired offset" << std::endl;
            return 1;
        }
        SetEndOfFile(h);
        CloseHandle(h);
    } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }

    return 0;
}