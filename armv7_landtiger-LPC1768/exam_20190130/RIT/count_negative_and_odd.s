				AREA asm_functions, CODE, READONLY				
                EXPORT  count_negative_and_odd
count_negative_and_odd
				; save current SP for a faster access 
				; to parameters in the stack
				MOV   r12, sp
				; save volatile registers (free them for use)
				STMFD sp!,{r4-r8,r10-r11,lr}				
				
				; - ABI standard -
				; 
				; parameters received in r0, r1, r2, r3
				; return value to be saved in r0, r1
				;
				; free to use:
				; 	r4, r5, r6, r7, r8, r10, r11
				
				
				
				; --- start here ---
				
				;r0 containes the pointer to first element of the vector
				;r1 containes the number of element
				mov		r11, #0		; number of element that are both even and odd
				
loop_in_vett	LDR		r8, [r0]	; access current element of VETT in R8
								
				; TEST NEGATIVE
				;extract MSB and test if it's 1
				ldr		r6, =0x80000000
				ands	r7, r8, r6		; extract into r7 MSB, update the flag.
				beq		prepare_next	; if presvious result is zero => positive, jump to next element
				
				; change sign of 2's complement number
				ldr		r6, = 0xFFFFFFFF
				eor		r8,	r8, r6		; perform xor with mask to flip all bits
				add		r8, #1			; add 1
				
				; TEST ODD using XOR trick (works only for unsigned number)
				lsr		r9, r8, #16
				eor 	r9, r9, r8

				lsr		r2, r9, #8
				eor 	r2, r2, r9

				lsr		r3, r2, #4
				eor 	r3, r3, r2

				lsr		r4, r3, #2
				eor 	r4, r4, r3
			
				lsr		r5, r4, #1
				eor 	r5, r5, r4
									
				; now in the LSB of r5
				; we have the XOR of all the bits of r8
				ldr		r6, =0x00000001 ; mask to extract the lsb (just by doing AND)
				
				ands	r7, r5, r6		; extract lsb of r5 in r7, if it's 1 = odd parity
				beq		prepare_next	; it's 0 => even parity, JUMP to next vett element
				add		r11, #1			; it's negative, number both negative and odd found: update counter and then go to prepare next
				
prepare_next	; go to next element of VETT
				subs	r1, #1	; if zero, we finished, all the elements scanned
				beq		exit	
				
				add		r0, #4	; move 4 byte the pointer to get address of next element
				b		loop_in_vett
				
exit			; return the number of elements in r0
				mov		r0, r11
				
				; restore volatile registers
				LDMFD sp!,{r4-r8,r10-r11,pc}	
				
				; --- end here ---
                END
				
				
				
				
				