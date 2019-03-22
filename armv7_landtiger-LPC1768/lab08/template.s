;            Computer Architectures - 02LSEOV 02LSEOQ            ;
; author: 		Paolo BERNARDI - Politecnico di Torino           ;
; creation: 	11 November 2018								 ;
; last update:  13 November 2018								 ;
; functionalities:												 ;
;		nothing but bringing to the reset handler				 ;

; *------- <<< Use Configuration Wizard in Context Menu >>> ------------------

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
				SPACE   Stack_Size/2
Stack_Mem       SPACE   Stack_Size/2 ; user process stack
__initial_sp




; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 16: Watchdog Timer
                DCD     TIMER0_IRQHandler         ; 17: Timer0
                DCD     TIMER1_IRQHandler         ; 18: Timer1
                DCD     TIMER2_IRQHandler         ; 19: Timer2
                DCD     TIMER3_IRQHandler         ; 20: Timer3
                DCD     UART0_IRQHandler          ; 21: UART0
                DCD     UART1_IRQHandler          ; 22: UART1
                DCD     UART2_IRQHandler          ; 23: UART2
                DCD     UART3_IRQHandler          ; 24: UART3
                DCD     PWM1_IRQHandler           ; 25: PWM1
                DCD     I2C0_IRQHandler           ; 26: I2C0
                DCD     I2C1_IRQHandler           ; 27: I2C1
                DCD     I2C2_IRQHandler           ; 28: I2C2
                DCD     SPI_IRQHandler            ; 29: SPI
                DCD     SSP0_IRQHandler           ; 30: SSP0
                DCD     SSP1_IRQHandler           ; 31: SSP1
                DCD     PLL0_IRQHandler           ; 32: PLL0 Lock (Main PLL)
                DCD     RTC_IRQHandler            ; 33: Real Time Clock
                DCD     EINT0_IRQHandler          ; 34: External Interrupt 0
                DCD     EINT1_IRQHandler          ; 35: External Interrupt 1
                DCD     EINT2_IRQHandler          ; 36: External Interrupt 2
                DCD     EINT3_IRQHandler          ; 37: External Interrupt 3
                DCD     ADC_IRQHandler            ; 38: A/D Converter
                DCD     BOD_IRQHandler            ; 39: Brown-Out Detect
                DCD     USB_IRQHandler            ; 40: USB
                DCD     CAN_IRQHandler            ; 41: CAN
                DCD     DMA_IRQHandler            ; 42: General Purpose DMA
                DCD     I2S_IRQHandler            ; 43: I2S
                DCD     ENET_IRQHandler           ; 44: Ethernet
                DCD     RIT_IRQHandler            ; 45: Repetitive Interrupt Timer
                DCD     MCPWM_IRQHandler          ; 46: Motor Control PWM
                DCD     QEI_IRQHandler            ; 47: Quadrature Encoder Interface
                DCD     PLL1_IRQHandler           ; 48: PLL1 Lock (USB PLL)
                DCD     USBActivity_IRQHandler    ; 49: USB Activity interrupt to wakeup
                DCD     CANActivity_IRQHandler    ; 50: CAN Activity interrupt to wakeup


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]                                            
				
				;------
				; Lab8
				;------
				
				MOV		R0, #3          
				MSR		CONTROL, R0             ; move to unpriliged mode, thread mode. a.k.a. User level.
				LDR		SP, =Stack_Mem          ; use the user process stack
				nop
				
				; load register for debug
				LDR		R0, =0xDEADBEEF
				LDR		R1, =0x00000005
				LDR		R2, =0xDEADBEEF
				LDR		R3, =0xDEADBEEF
				LDR		R4, =0xBBBBBBBB
				LDR		R5, =0x00000006
				LDR		R6, =0xBBBBBBBB
				LDR		R7, =0xDEADBEEF
				LDR		R8, =0xDEADBEEF
				LDR		R9, =0xDEADBEEF
				LDR		R10, =0xDEADBEEF
				LDR		R11, =0xDEADBEEF
				LDR		R12, =0xDEADBEEF
				
				; push r0 to r12 to the stack, r0 to r3 and r12 are redundant,
				; but i push them so that I can access the register values from the register number
				STMFD 	SP!, {R0-R12}	

                ;raise software interrupt for calling SysCall 0x10
				SVC		0xE9
				
				; pop updated version of r0 to r12 to the stack
				LDMFD 	SP!, {R0-R12}

				
InfLoop         B      	InfLoop
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP

; ---------------------
; - START SVC HANDLER -
; ---------------------
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
				
				;STMFD 	SP!, {R0-R12, LR}  		; save all registers to the system master stack
				MRS 	R1, PSP                 ; get into R1 the user process stack pointer
				LDR	 	R0, [R1, #24]			; load into R0 the PC value of the user process
				LDR 	R0, [R0,#-4]			; take into R0 the code instruction of the SVC, so the last executed instruction
				BIC 	R0, #0xFF000000			; bit clear MSB
				LSR		R0, #16					; R0 now contains only the SVC number
				
				;-----------------
				; test SVC number
				;-----------------
				
				; 0 to 12: reset register Rx, where x=[0,12]
				CMP 	R0, #13
				BLO		reset_register			; branch if lower
				
				; 12 to 63: nop
				CMP 	R0, #64
				BLO		just_nop				; branch if lower then 64, 0-12 excluded by previous cmp
				
				; 64 to 255: module operation
				AND		R2, R0, 0x00000060		; select only 5th and 6th bit
				LSR		R2, R2, #5				; put them into bit 0 and 1
				
				; now test the value of the 5th and 6th bit, take path accordingly
				CMP		R2, #0					; reset registers R_ID1 and R_ID2
				BEQ		reset_rid
				
				CMP		R2, #1					; just do a nop
				BEQ		just_nop
				
				CMP		R2, #3					; do the module operation
				BEQ		module_op
			
				B 		exit_svchandler
				
				
				; case 1 - reset the content of the Rx register
reset_register	LDR		R2, =0x00000000			; load R2 with zeroes, R1 contains the PSP
				MOV		R3, R1					
				ADD		R3, #36					; R3 is a pointer to the first register saved into the stack
				LDR		R5, =0x00000004			; load the multiplier used to obtain the offset
				MUL		R4, R0, R5				; the R0 register number, multiplied by four, becomes the offset of the register in the stack
				ADD 	R3, R4, R3				; put together the two offsets to obtain the pointer 
				STR		R2, [R3]				; reset the value of the register stored into the user stack and pointed by R3
				B		exit_svchandler


				; case 2 - just do a nop and exit
just_nop		nop
				B		exit_svchandler


				; case 3 - reset the R_ID1 and R_ID2
				; PLEASE NOTE: we'll never end in this case because the SVC[6] and SVC[5] equal to 0 case
				; 			   is captures by case 2, the nop operation...something wrong in the lab track?
reset_rid		AND		R2, R0, 0x00000007		; extract R_ID_2 and reset the corresponding register
				MOV		R3, R1					; copy stack pointer (user)
				ADD		R3, #36					; R3 is a pointer to the first register saved into the stack
				LDR		R5, =0x00000004			; load the multiplier used to obtain the offset
				MUL		R4, R2, R5				; the R_ID_2 (R2) register number, multiplied by four, becomes the offset of the register in the stack
				ADD 	R3, R4, R3				; put together the two offsets to obtain the pointer 
				STR		R2, [R3]				; reset the value of the register stored into the user stack and pointed by R3
				
				AND		R2, R0, 0x00000038		; extract R_ID_1 and reset the corresponding register
				LSR		R2, R2, #3
				MOV		R3, R1					; copy stack pointer (user)
				ADD		R3, #36					; R3 is a pointer to the first register saved into the stack
				LDR		R5, =0x00000004			; load the multiplier used to obtain the offset
				MUL		R4, R2, R5				; the R_ID_1 (R2) register number, multiplied by four, becomes the offset of the register in the stack
				ADD 	R3, R4, R3				; put together the two offsets to obtain the pointer 
				STR		R2, [R3]				; reset the value of the register stored into the user stack and pointed by R3
				
				B		exit_svchandler

				; case 4 - compute the module operation
				;		   implementation of pseudo code found at: 
				;		   https://stackoverflow.com/questions/938038/assembly-mod-algorithm-on-processor-with-no-division-operator
module_op		
				; get into R5 the divisor (b, R_ID_2 from the stack)
				AND		R5, R0, 0x00000007
				MOV		R3, R1					; copy stack pointer (user)
				ADD		R3, #36					; R3 is a pointer to the first register saved into the stack
				LDR		R10, =0x00000004		; load the multiplier used to obtain the offset
				MUL		R4, R5, R10				; the R_ID_2 register number, multiplied by four, becomes the offset of the register in the stack
				ADD 	R3, R4, R3				; put together the two offsets to obtain the pointer 
				LDR		R5, [R3]				; get the register value
				
				; get into R6 the dividend (a, R_ID_1 from the stack)
				AND		R6, R0, 0x00000038
				LSR		R6, R6, #3
				MOV		R3, R1					; copy stack pointer (user)
				ADD		R3, #36					; R3 is a pointer to the first register saved into the stack
				LDR		R10, =0x00000004		; load the multiplier used to obtain the offset
				MUL		R4, R6, R10				; the R_ID_1 register number, multiplied by four, becomes the offset of the register in the stack
				ADD 	R3, R4, R3				; put together the two offsets to obtain the pointer 
				LDR		R6, [R3]				; get the register value
				
				; if divisor == 0 -> error, exit safely
				CMP 	R5, #0
				BEQ		exit_svchandler
				
				; set remainder=dividend and next_multiple=divisor
				MOV		R7, R6 ; remainder, initially equal to dividend
				MOV 	R8, R5 ; next_multiple, initially equal to divisor
				MOV		R9, #0 ; multiple
				
				; calculate the remainder by iterating
search_multiple	MOV		R9, R8 ; multiple = next_multiple
				LSL		R8, R9, #1 ; next_multiple = left_shift(multiple, 1)
				
				CMP		R8, R7     ; while next_multiple <= remainder && next_multiple > multiple
				BLS		second_test
				B		go_on
				
second_test		CMP		R8, R9
				BHI		search_multiple
				B		go_on
				
go_on			CMP		R9, R5 	; while multiple >= divisor
				BHS		update_rem					
				B		mod_completed
				
update_rem		CMP		R9, R7		; if multiple <= remainder,
				SUBLS	R7, R7, R9	;	 remainder = remainder - multiple
				LSR		R9, R9, #1  ; multiple = right_shift(multiple, 1)
				B		go_on

mod_completed	; save the result (remainder) in r0
				MOV		R3, R1					
				ADD		R3, #36					; R3 is a pointer to R0, saved into the stack
				STR		R7, [R3]				; save the remainder into the stack, overwrite R0
				B		exit_svchandler



				; exit SVC handler
exit_svchandler	;LDMFD 	SP!, {R0-R12, LR}         ; reload registers
				BX 		LR                        ; return to unprivileged state and process stack pointer
				
                ENDP
; -------------------
; - END SVC HANDLER -
; -------------------

DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  I2C0_IRQHandler           [WEAK]
                EXPORT  I2C1_IRQHandler           [WEAK]
                EXPORT  I2C2_IRQHandler           [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]
                EXPORT  SSP0_IRQHandler           [WEAK]
                EXPORT  SSP1_IRQHandler           [WEAK]
                EXPORT  PLL0_IRQHandler           [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  EINT0_IRQHandler          [WEAK]
                EXPORT  EINT1_IRQHandler          [WEAK]
                EXPORT  EINT2_IRQHandler          [WEAK]
                EXPORT  EINT3_IRQHandler          [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  BOD_IRQHandler            [WEAK]
                EXPORT  USB_IRQHandler            [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  I2S_IRQHandler            [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  RIT_IRQHandler            [WEAK]
                EXPORT  MCPWM_IRQHandler          [WEAK]
                EXPORT  QEI_IRQHandler            [WEAK]
                EXPORT  PLL1_IRQHandler           [WEAK]
                EXPORT  USBActivity_IRQHandler    [WEAK]
                EXPORT  CANActivity_IRQHandler    [WEAK]

WDT_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
PWM1_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
I2C2_IRQHandler
SPI_IRQHandler
SSP0_IRQHandler
SSP1_IRQHandler
PLL0_IRQHandler
RTC_IRQHandler
EINT0_IRQHandler
EINT1_IRQHandler
EINT2_IRQHandler
EINT3_IRQHandler
ADC_IRQHandler
BOD_IRQHandler
USB_IRQHandler
CAN_IRQHandler
DMA_IRQHandler
I2S_IRQHandler
ENET_IRQHandler
RIT_IRQHandler
MCPWM_IRQHandler
QEI_IRQHandler
PLL1_IRQHandler
USBActivity_IRQHandler
CANActivity_IRQHandler

                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit                

                END
