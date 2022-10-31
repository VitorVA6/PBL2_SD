.global _start
_start:
	mov r0, #4
	mov r1, #3
pin0_9:
	cmp r0, #10
	bge pin10_19
	mov r2, #0
	mul r3, r1, r0 
	b end

pin10_19:
	cmp r0, #20
	bge pin20_29
	mov r2, #4
	sub r3, r0, #10
	mul r3, r1, r3
	b end

pin20_29:
	mov r2, #8
	sub r3, r0, #20
	mul r3, r1, r3
	b end

end: