.DATA

	PUBLIC __syscall
	PUBLIC __set_syscall_id

	__syscall_id DWORD 0

.CODE

	ALIGN 16
	__syscall PROC

		mov r10, rcx

		mov eax, DWORD PTR[ __syscall_id ]

		syscall
		
		ret

	__syscall ENDP

	__set_syscall_id PROC

		mov __syscall_id, ecx

		ret

	__set_syscall_id ENDP

END