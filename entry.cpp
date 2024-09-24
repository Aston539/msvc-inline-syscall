#include <iostream>
#include <windows.h>

#define SYSCALL_DISASSEMBLE_ROUTINES

#include "syscall.h"

#pragma comment( lib, "ntdll" )

EXTERN_C
NTSTATUS
WINAPI
NtQueryPerformanceCounter(
    _Out_     PLARGE_INTEGER PerformanceCounter,
    _Out_opt_ PLARGE_INTEGER PerformanceFrequency
);

EXTERN_C
NTSTATUS
NtAllocateVirtualMemory(
    HANDLE ProcessHandle,
    PVOID* BaseAddress,
    ULONG_PTR ZeroBits,
    PSIZE_T RegionSize,
    ULONG AllocationType, 
    ULONG Protect
);

int main()
{
    LARGE_INTEGER PerformanceCounter = { };
    LARGE_INTEGER PerformanceFrequency = { };
    NTSTATUS      Status = NULL;

    Status = SYSCALL( NtQueryPerformanceCounter )( &PerformanceCounter, &PerformanceFrequency );

    printf( "[ SYSCALL ] NtQueryPerformanceCount\n\tPerformanceCounter: %llu\n\tPerformanceFrequency: %llu\n\tStatus: %lX\n\n",
        PerformanceCounter.QuadPart, PerformanceFrequency.QuadPart, Status );

    PVOID Base = NULL;
    SIZE_T RegionSize = 50;
    Status = SYSCALL( NtAllocateVirtualMemory )( GetCurrentProcess( ), &Base, NULL, &RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

    printf( "[ SYSCALL ] NtAllocateVirtualMemory\n\tBaseAddress: %p\n\tRegion Size: %p\n\tStatus: %lX\n\n",
        Base, RegionSize, Status );

    return 0;
}