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
Stack_Mem       SPACE   Stack_Size
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
                LDR     R0, =Reset_Handler
				
				; ----------------------------------------------------------
				; Lab 2 exercise 1 solution
				; ----------------------------------------------------------
				
				; ----------------------------------------------------------
				; Part 1 - order the Price_list
				; ----------------------------------------------------------				
	
				; 1) first copy Price_list in heap memory (R/W)
				
				LDR		R1, =Price_list 	; load address of Price_list in R1 (Act as pointer)
				LDR		R2, =Heap_Mem
				MOV		R3, #56

copy_loop		LDR		R4, [R1]
				STR		R4, [R2]				

				ADD		R1, #4
				ADD		R2, #4
				SUBS	R3, #1

				BNE		copy_loop	
				
				; 2) Sort the copy in the heap using Selection Sort
				LDR		R2, =Heap_Mem	; list to sort
				MOV		R3,	#0			; minVal
				MOV		R4,	#0			; i
				MOV		R5, #0			; minIndex
				MOV		R6, #216 		; len, number of bytes occupied by the collection of blocks of PID+Price = 8B

external_loop	MOV		R7, R2			; R7 = Heap_Mem + i
				ADD		R7, R4

				LDR 	R3, [R7]		; R3 = minVal
				;ADD		R5, R4			; minIndex = i
				MOV		R5, R4

				MOV		R8, R4 			; R8 = j = i+8
				ADD		R8, #8


				; for each element from "j=i+1" to "len-1"
internal_loop	MOV		R9, R2			; R9 = [heap_mem+j]
				ADD		R9, R8		
				LDR 	R10, [R9]		; load j-th element
		
				CMP		R10, R3			; j-th element lower then minimum?

				;if yes, update mininum found
				MOVLO	R3, R10			; minValue = j-th element									
				MOVLO   R5, R8			; minIndex = j

				;move to next iteration in inernal_loop
				CMP		R8, R6			; do until end internal loop
				ADDNE 	R8, #8			; j = j+8
				BNE		internal_loop
				

				;continue external loop, swap elements: where elements = PID+Price
				MOV 	R11, R2
				ADD 	R11, R5 		; R11 = heap_mem + minimumIndex

				; swap pid
				LDR		R12, [R7]		; R12 = [heap_mem+i], PID of i-th element
				STR		R12, [R11]		; store PID of i-th element in [heap_mem+minIndex]
				STR		R3, [R7]		; store the minimum in [heap_mem+i]

				; swap price
				ADD		R7, #4 			; make R7 = heap_mem+i+4
				ADD		R11, #4		    ; make R11 = heap_mem + minimumIndex +4
				LDR		R3, [R11]		; load in R3 the price of minValue 
				LDR		R12, [R7]		; R12 = [heap_mem+i+4], price of i-th element
				STR		R12, [R11]		; store price i-th element in [heap_mem+minIndex+4]
				STR		R3, [R7]		; store the minimum price in [heap_mem+i+4]

				ADD 	R4, #8
				CMP 	R4, R6			; if i!=len jump to external_loop
				BNE		external_loop


				; ----------------------------------------------------------
				; Part 2 - find the total amount to pay
				; ----------------------------------------------------------

				;flush registers
				LDR		R0, =0x00000000
				LDR		R1, =0x00000000
				LDR		R2, =0x00000000
				LDR		R3, =0x00000000
				LDR		R4, =0x00000000
				LDR		R5, =0x00000000
				LDR		R6, =0x00000000
				LDR		R7, =0x00000000
				LDR		R8, =0x00000000
				LDR		R9, =0x00000000
				LDR		R10, =0x00000000
				LDR		R11, =0x00000000
				LDR		R12, =0x00000000
				LDR		R13, =0x00000000

				LDR		R0, Item_num		; number of item to search		

				LDR		R1, =Item_list 		; load address of Item_list in R1 (Act as pointer)

				LDR		R12, =Heap_Mem 	; load address of Price_list in R12 (Act as pointer)			

				LDR 	R2, [R1]			; get first product ID to search
				LDR		R3, [R1, #4]		; get first number of product ID to buy

Compute_total	; search the price of the current product into Price_list, using binary search

				LDR		R4, =0x00000001	; first = 1
				LDR 	R5, =0x0000001C	; last = 29	(hex=1B)

test			CMP 	R4, R5    			; while first <= last
				LDRHS	R10, =0x00000000  	; if not, output is zero
				BHS		InfLoop				; and go to end of program		
				
				; compute offset of middle current middle element
				ADD		R7, R4, R5			; R7 = first + last
				LDR		R6, =0x00000002
				UDIV	R9, R7, R6			; R9 = middle = (first+last)/2
				LDR		R7, =0x00000008
				MUL		R6, R9, R7			; R6 = middle*8, offset inside Price_list
				LDR		R7, =0x00000000
				ADD		R7, R12, R6		 	; R7 = R12 + middleOffset
				LDR		R8, [r7]			; R8 = [R12 + middleOffset], the middle productID
				
				; test if ProductID == PriceList[middle]	
				CMP		R2, R8
				
				;if key == table[middle], element found
				LDREQ		R11, [r7, #4] 	; get price in R11 and exit
				BEQ			exit_loop

				;else update and loop again
				MOVLO		R5, R9			; last = middle - 1
				SUBLO		r5, #1
				BLO			test		    ; go to next iteration
					;else
				MOVHS		R4, R9			; first = middle + 1
				ADDHS		R4, #1			
				B test 						; go to next iteration

exit_loop		; update the total price
				MUL		R11, R11, R3		; productPrice*productQuantity
				ADD		R10, R10, R11		; total

				; test R5 (Item_num and jump)
				SUBS	R0, R0, #1		  	; decrease R5 updating the flags (we'll watch for Zero flag set)

				; update product ID and number of product (if zero flag set we don't need to, we'll exit)
				ADDNE	R1, R1, #8			; update Item_list pointer	
				LDRNE	R2, [R1] 			; get next product ID
				LDRNE	R3, [R1, #4]		; get price of next product

				BNE		Compute_total			

			
InfLoop         B      	InfLoop

; Wholesaler price list, composed of two int value
; product ID (4B), and price (4B)
; ordered with respect to the ID
Price_list		DCD 	0x00A, 245,  0x004, 120,  0x006, 315,  0x007, 1210 
				DCD 	0x010, 228,  0x012, 7,    0x016, 722,  0x017, 1217 
				DCD 	0x018, 138,  0x01A, 2222, 0x01B, 34,   0x01E, 11
				DCD 	0x022, 223,  0x023, 1249, 0x025, 240,  0x027, 112
				DCD 	0x02C, 2245, 0x02D, 410,  0x031, 840,  0x033, 945
				DCD 	0x036, 3211, 0x039, 112,  0x03C, 719,  0x03E, 661
				DCD 	0x042, 230,  0x045, 1112, 0x047, 2627, 0x04A, 265
			    ALIGN	4
		

; List of item to buy
; product ID, quantity
; not ordered
Item_list		DCD 	0x022, 14, 0x006, 431, 0x03E, 1210, 0x017, 56342 
				ALIGN 	4

; The number of total product to buy
Item_num		DCB 	4
				ALIGN	4 ; ensure alignment to 4B = 32bits
		

				; ----------------------------------------------------------
				; END PROGRAM
				; ----------------------------------------------------------
				
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
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
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
