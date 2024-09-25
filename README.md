msvc-inline-syscall
Overview

msvc-inline-syscall is a Microsoft Visual Studio project designed to perform inline system calls (syscalls) in Windows using assembly. The project provides mechanisms to dynamically set and retrieve syscall IDs for specific routines, offering flexible syscall execution without relying on traditional APIs. This can be useful for low-level Windows development and bypassing certain API-layer restrictions.
Features

    Inline Syscalls: Ability to execute syscalls directly in user-mode with __syscall and __set_syscall_id.
    Syscall ID Retrieval: Automatically disassemble a routine to find its associated syscall ID with SysGetSyscallIDFromRoutine.
    Lambda-Based Execution: Use macros like SYSCALL, SYSCALL_GET, and SYSCALL_BY_ID to create and invoke syscalls in a flexible and type-safe manner.
    Disassembler Integration (Optional): Optionally integrate with hde64 to disassemble routines and extract the syscall ID.


    Example: Allocating Virtual Memory

cpp

PVOID Base = NULL;
SIZE_T RegionSize = 50;
Status = SYSCALL( NtAllocateVirtualMemory )( GetCurrentProcess( ), &Base, NULL, &RegionSize, MEM_COMMIT | MEM_R
