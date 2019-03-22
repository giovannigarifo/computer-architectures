;
;   First lab: find maximum in a vector of 100 signed integers
;

;----------
;   DATA
;----------
    .data

; one hundred numbers, max is 200, minimum is -100
vector: .word -100, -27, 51, -49, -55, -48, -95, -23, -25, 0
        .word 200, 27, 51, -49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, 23, 25, 50
        .word 100, 27, 51, 49, 55, 48, 95, -23, 25, 50

; eight bytes reserved in memory for result
result: .space 8    

;-----------
;   CODE
;-----------
    .text;

MAIN:   daddui  R1,R0,100   ; R1 contains the number of elements of the array

        daddi   R2,R0,1     ; R2 is used to contain the current maximum.
        dsll    R2,R2,31    ; Initally, it is the minimum value on 64bit for
        dsll    R2,R2,31    ; a two s complement number, that is 0x8000 0000 0000 0000
        dsll    R2,R2,1     ; MIPS64 DSLL instruction allows only from 0 to 31 bit shift 
                            ; (5 bit reserved in the istruction), so I shifted it three times 

        daddu   R3,R0,R0    ; R3 contains the offset of the current element of the vector

LOOP:   ld      R4,vector(R3)   ; get current element in R4
        slt     R5,R4,R2        ; if R4(current element) < R2(current maximum) then R5=1 else R5=0. 
        movz    R2,R4,R5        ; if R5=0 update current max

        daddi   R3,R3,8     ; increment offset to new element, distance is 8 byte because is a vector of word
        daddi   R1,R1,-1    ; decrement numer of iterations left

        bnez    R1,LOOP     ; if R1!=0, go to LOOP
        ;LOOP END

        sd      R2,result(R0)   ;save the maximum in result
        
        HALT
