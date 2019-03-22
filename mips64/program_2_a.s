;
;   Third lab: write at low level this algorithm
;
;   for (i = 0; i < 30; i++){
;	    v3[i] = v1[i]*v2[i];
;	    v4[i] = v3[i]/v2[i]; //assume v3 does not contains zeroes
;	    v5[i] = v4[i]+v2[i]; 
;   }
;
;   OPTIMIZATION: some instruction have been reordered to reduce the number of stalls

;----------
;   DATA
;----------
    .data

; input vectors of floating point numbers
v1: .double 1, 2, 3, 4, 5, -8.98, -1.3, -2.34, -2.9, 1.7
    .double -1.01, -2.04, 4.1, -7.23, -1.11, -12.98, -13.3, -21.34, -29.9, 14.7
    .double -4.01, -1.04, 10.1, -98.23, -58.11, -83.98, -12.3, -22.34, -21.9, 17.7

v2: .double 1, 2, 3, 4, 5, -3.8, -13.3, -6.34, 72.9, 1.1
    .double -12.01, -75.04, 0.1, -0.23, -50.11, -45.9, -0.3, -3.34, -2.0, 1.8
    .double -11.01, -79.04, 19.1, -89.23, -43.11, -2.98, -1.3, -2.34, -2.9, 1.0

v3: .space 240

v4: .space 240

v5: .space 240

;-----------
;   CODE
;-----------
    .text;

        l.d F11, v1(R0)     ; load from memory v1[0]
        l.d F12, v2(R0)     ; load from memory v2[0]

        ; - setup -
        daddui R1,R0,30 ;R1 contains number of iterations to do
        daddi R2,R0,-8  ;R2 contains current offset (i) inside the vector
        daddi R3,R0,0   ;R3 contains R2+8, it is an index to values of next iteration

LOOP:   mul.d F23, F11, F12 ; v3[i] = v1[i]*v2[i];
        
        ;update loop control registers
        daddi R2, R2, 8     ; move to current offset
        daddi R3, R3, 8     ; point to next iteration
        daddi R1, R1, -1    ; update number of iterations left
        
        s.d F23, v3(R2)     ; store F23 in v3[i]

        div.d F24, F23, F12 ; v4[i] = v3[i]/v2[i];
        add.d F25, F24, F12 ; v5[i] = v4[i]+v2[i];

        l.d F11, v1(R3)     ; load from memory v1[i+1]
        l.d F12, v2(R3)     ; load from memory v2[i+1]
        
        s.d F24, v4(R2)     ; save F24 in v4[i]
        s.d F25, v5(R2)     ; save F25 in v5[i]

        ; load next operands, safe to do because values used by the FP computations
        ; are stored in intermediate registers of the pipeline

        ;jump control section
        bnez R1, LOOP       ; if other iterations are required, go to LOOP

        ;end program
        HALT

