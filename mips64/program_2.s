;
;   Second lab: write at low level this algorithm
;
;   for (i = 0; i < 30; i++){
;	    v5[i] = v1[i]*v2[i];
;	    v6[i] = v2[i]/v3[i]; //assume v3 does not contains zeroes
;	    v7[i] = v1[i]+v4[i]; 
;   }
;

;----------
;   DATA
;----------
    .data

; input vectors of floating point numbers
v1: .double 3, -7.04, 1.1, -9.23, -5.11, -8.98, -1.3, -2.34, -2.9, 1.7
    .double -1.01, -2.04, 4.1, -7.23, -1.11, -12.98, -13.3, -21.34, -29.9, 14.7
    .double -4.01, -1.04, 10.1, -98.23, -58.11, -83.98, -12.3, -22.34, -21.9, 17.7

v2: .double -15.01, -79.09, 12.1, -3.23, -51.11, -3.8, -13.3, -6.34, 72.9, 1.1
    .double -12.01, -75.04, 0.1, -0.23, -50.11, -45.9, -0.3, -3.34, -2.0, 1.8
    .double -11.01, -79.04, 19.1, -89.23, -43.11, -2.98, -1.3, -2.34, -2.9, 1.0

v3: .double -20.07, -4.07, 1.1, -9.23, -5.11, -9.98, -12.3, -24.34, -2.9, 11.7
    .double -1.09, -7.09, 2.2, -9.23, -5.56, -6.98, -16.3, -0.34, -2.9, 14.7
    .double -10.01, -5.5, 3.3, -9.23, -5.21, -1.98, -13.3, -21.34, -2.9, 10.7

v4: .double -1.01, -7.04, 1.1, -9.23, -5.11, -8.98, -1.3, -2.34, -2.9, 0.79
    .double -1.01, -7.04, 1.1, -9.23, -5.11, -8.98, -1.3, -2.34, -2.9, 3.78
    .double -1.01, -7.04, 1.1, -9.23, -5.11, -8.98, -1.3, -2.34, -2.9, 1.74    

;output vectors, zero filled, of 30 elements (8B*30) each
v5: .space 240
v6: .space 240
v7: .space 240


;-----------
;   CODE
;-----------
    .text;

        ; - setup -
        daddui R1,R0,30 ;R1 contains number of iterations to do
        daddui R2,R0,0  ;R2 contains current offset (i) inside the vector

        ; - do computation -
        ; to decrease the total clock cycles, first start the FP computations and
        ; then do the needed work on the registers. It s a good way to exploit the pipelining.
        ; Values are loaded in the Fxx registers according to the best strategy (i.e. F11 and F22
        ; first because div will be the first operation)

        l.d F11, v2(R0)     ; load from memory v2[0]
        l.d F12, v3(R0)     ; load from memory v3[0]
        l.d F10, v1(R0)     ; load from memory v1[0]
        l.d F13, v4(R0)     ; load from memory v4[0]
        
LOOP:   div.d F21, F11, F12 ; F21 = v2[i]/v3[i]     ; start floating point multi-cycle computations
        mul.d F20, F10, F11 ; F20 = v1[i]*v2[i]
        add.d F22, F10, F13 ; F22 = v1[i] + v4[i]

        ;update loop control registers
        daddi R2, R2, 8     ; move to next element
        daddi R1, R1, -1    ; update number of iterations left
        
        ; load next operands, safe to do because values used by the FP computations
        ; are stored in intermediate registers of the pipeline
        l.d F11, v2(R2)     ; load from memory v2[i]
        l.d F12, v3(R2)     ; load from memory v3[i]
        l.d F13, v4(R2)     ; load from memory v4[i]
        l.d F10, v1(R2)     ; load from memory v1[i]

        ; here there will be a stall, because ROW hazards surely occurs: need to wait for FP operations results
        s.d F22, v7(R2)     ; save F22 in v7[i]
        s.d F20, v5(R2)     ; save F20 in v5[i]
        s.d F21, v6(R2)     ; save F21 in v6[i]

        ;jump control section
        bnez R1, LOOP       ; if other iterations are required, go to LOOP

        ;end program
        HALT

