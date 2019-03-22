;
;   Second lab: write at low level this algorithm
;
;   Given a data array of 100 elements, where each element is 1B long.
;   For each element, bits 0 to 6 are data bits, the 7th bit is the parity one
;
;   Consider even parity: the parity bit is set to 1 if the number of one in the data bits is odd
;
;   Implement an algorithm that is able to set the parity bit:
;
;   for (i = 0; i < 100; i++){
;		if (parity_in [x[i]0..6 ])
;		     x[i]7 = 1;
;       else
;            x[i]7 = 0;
;   }
;

;----------
;   DATA
;----------
    .data

; input vector
x:  .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7, 6, 7
    .byte 6, 7, 6, 7, 6


;-----------
;   CODE
;-----------
    .text;

        ; -setup -
        daddui r1, r0, 100   ; number of element of the vector
        daddui  r2, r0, 0 

        ; - compute parity bit - 
LOOP:   lb r10, x(r2)       ; load i-th element in r10

        ; perform a XOR of all bits of the element, to do so
        ; shift by decreasing power of 2 xoring the result, so that
        ; in the LSB we obtain the XOR of all the bits, i.e.:
        ; 
        ; given "abcdfghi":
        ; abcdfghi >> 4 = 0000abcd
        ; abcdfghi XOR 0000abcd = abcd af bg ch di   <- notation: af = a XOR f
        ; ...
        ;
        ; continuing like this in the end we obtain: a ab abc abcd ... abcdefgh <- bingo
        ; if the LSB is 1, then there are an odd number of 1 in the byte
        ;
        ; credits: https://stackoverflow.com/a/21618038/7204198

        dsrl r11,r10,4      ; shift i-th byte 4 bit to right
        xor r11,r11,r10     ; do xor of i-th byte and the shifted one

        dsrl r12,r11,2      ; shift result of previous operation 2 bit to right
        xor r12,r12,r11     ; do xor of previous result and the newly shifted one

        dsrl r13,r12,1      ; shift result of previous operation 1 bit to right
        xor r13,r13,r12     ; do xor of previous result and the newly shifted one

        ; thus we obtain in the LSB of r13 the XOR of all the bits of the i-th byte
        daddui r19, r0, 0x01    ; mask to extract the LSB bit = 0x01
        and r20, r13, r19       ; perform an AND to extract the LSB in r20

        ;if LSB=1 => the byte has an odd number of one
        bnez r20, ODD


        ;even number of one => set MSB to 0
EVEN:   daddui r17, r0, 0x7f ; mask to set MSB to 0
        and r10,r10,r17 ; flip MSB to 0
        sb r10, x(r2) ; save updated byte with parity  
        
        daddi r2, r2, 1     ; point to next element
        daddi R1,R1,-1    ; decrement numer of bytes left
        bnez R1, LOOP ; next byte
        j EXIT


        ;odd number of one => set MSB to 1
ODD:    daddui r18, r0, 0x80 ; mask to set MSB to 1
        or r10,r10,r18 ; flip MSB to 1
        sb r10, x(r2) ; save updated byte with parity
        
        daddi r2, r2, 1     ; point to next element
        daddi R1,R1,-1    ; decrement numer of bytes left
        bnez R1, LOOP ; next byte
        j EXIT
        
        
EXIT:
        ;program end
        HALT

