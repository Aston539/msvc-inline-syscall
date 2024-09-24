#pragma once

#include <wtypes.h>

#ifdef SYSCALL_DISASSEMBLE_ROUTINES
#include "hde64.h"
#endif

#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#endif

EXTERN_C
VOID
WINAPI
__syscall(
    VOID
);

EXTERN_C
VOID
WINAPI
__set_syscall_id(
    ULONG id
);

#define SYSCALL_MAX_ID_SEARCH 32

FORCEINLINE
ULONG
SysGetSyscallIDFromRoutine(
    _In_ PVOID Routine
)
{
    if ( Routine == NULL )
    {
        return NULL;
    }

#ifndef SYSCALL_DISASSEMBLE_ROUTINES
    for ( PBYTE Opcode = ( PBYTE )Routine; Opcode < ( PBYTE )Routine + SYSCALL_MAX_ID_SEARCH; Opcode++ )
    {
        //
        // Opcode == mov eax, ?
        //
        if ( *Opcode == 0xB8 )
        {
            //
            // get 4 byte immediate from instruction
            // this being the target syscall id
            //
            return *( ULONG* )( Opcode + 1 );
        }
    }
#else
    //
    // disassemble the passed routine looking
    // for the mov eax, ? instruction
    //
    hde64s Instruction = { };
    for ( LONG Count = NULL; 
               Count < SYSCALL_MAX_ID_SEARCH && 
               hde64_disasm( ( PBYTE )Routine + Count, &Instruction ); 
               Count += Instruction.len )
    {
        //
        // Instruction.opcode == mov eax, ?
        //
        if ( Instruction.opcode == 0xB8 )
        {
            //
            // get 4 byte immediate from instruction
            // this being the target syscall id
            //
            return Instruction.imm.imm32;
        }
    }
#endif

    return NULL;
}

//
// returns a pointer to a lambda which will set the appropriate
// syscall id and return a pointer to the inline syscall function
// itself as so that the caller can cast accordingly
//
#define SYSCALL_GET( ID )\
[ ]( ) -> PVOID { \
    __set_syscall_id(ID); \
    return ( PVOID )__syscall; \
}

//
// takes in a syscall id and routine for compile time type
// information and returns a lambda which will perform the
// inline syscall
//
#define SYSCALL_BY_ID( ID, Routine )\
[ ](auto... args) -> decltype(Routine(args...)) { \
    return ( ( decltype( &Routine ) )SYSCALL_GET( ID )( ) )(args...); \
}

//
// takes in a pointer to a direct syscall routine
// such as NtCreateFile and optionally disassembles or walks
// the routine to find the syscall id then will return a lambda
// which will perform the inline syscall
//
#define SYSCALL( Routine ) \
[ ](auto... args) -> decltype(Routine(args...)) { \
    return SYSCALL_BY_ID( SysGetSyscallIDFromRoutine( ( PVOID )Routine ), Routine )( args... ); \
}