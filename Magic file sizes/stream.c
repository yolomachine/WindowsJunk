#include <windows.h>
#include <stdio.h>

void main(int argc, char* argv[]) {
    HANDLE hStream;
    DWORD dwRet;

    char* str = malloc(524288000 + 1);
    memset(str, 'a', 524288000);
    str[524288000] = '\0';

    hStream = CreateFile("zerobyte:stream", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, 0, NULL);
    if( hStream == INVALID_HANDLE_VALUE )
        printf("Cannot open testfile:stream\n");
    else 
        WriteFile(hStream, str, 524288000, &dwRet, NULL);
}