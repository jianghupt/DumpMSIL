#include<stdio.h>
#include<Windows.h>
//#include "corjithost.h"
//#include "jithost.h"

typedef void (*pFunc)();
typedef const char (*buildstr)[];
typedef void (*jitStartup)(void* jitHost);
typedef void (*dumpILRange)(const BYTE* const codeAddr, unsigned codeSize);
typedef int  (*compare)(int a, int b);




int main(int argc, char** argv)
{
    HMODULE h = LoadLibraryExA((LPCSTR)"E:\\Visual Studio Project\\Test_\\x64\\Debug\\DaoChu.dll", NULL, 0);
    pFunc pc = (pFunc)GetProcAddress(h, "ABCD");
    char* p = (char*)pc;
    p = p - 0x69;
    compare ce = (compare)p;
    ce(1, 2);



    HMODULE h1 = LoadLibraryExA((LPCSTR)"G:\\runtime9_pre\\runtime\\artifacts\\bin\\coreclr\\windows.x64.Debug\\clrjit.dll", NULL, 0);
    //jitStartup* p1 = (jitStartup*)GetProcAddress(h1, "jitStartup");

    //(*p1)(JitHost::getJitHost());

    char* p1 = (char*)(jitStartup)GetProcAddress(h1, "jitStartup");
    p1 = p1 + 0x2B6380;
    dumpILRange dumpil = (dumpILRange)p1;
    //const char ar[] = { '28','3f', '00', '00', '06' };
    const BYTE  bt[5] = { 0x28,0x3f,0x00,0x00,0x06 };


    //JitTls ts = new JitTls(NULL);



    dumpil(bt, 6);
}