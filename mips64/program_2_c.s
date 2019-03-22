;
;   Third lab: write at low level this algorithm
;
;   for (i = 0; i < 30; i++){
;	    v3[i] = v1[i]*v2[i];
;	    v4[i] = v3[i]/v2[i]; //assume v3 does not contains zeroes
;	    v5[i] = v4[i]+v2[i]; 
;   }
;
;   OPTIMZATION: loop unrolling (3 times)


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
        daddui R1,R0,10  ; R1 contains number of iterations to do, because the loop has been unrolled by a factor of 3, the iterations are 10
        daddi  R2,R0,-8  ; R2 contains current offset (i) inside the vector
        daddi  R3,R0,0   ; R3 points to the values of v1 and v2 for the next iterations

LOOP:   ; first unroll

        mul.d F23, F11, F12 ; v3[i] = v1[i]*v2[i];
        
        ;update loop control registers
        daddi R2, R2, 8     ; move R2 to current iteration
        daddi R3, R3, 8     ; move R3 to next iteration
        s.d F23, v3(R2)     ; store F23 in v3[i]

        div.d F24, F23, F12 ; v4[i] = v3[i]/v2[i];
        add.d F25, F24, F12 ; v5[i] = v4[i]+v2[i];

        l.d F13, v1(R3)     ; load from memory v1[i+1]
        l.d F14, v2(R3)     ; load from memory v2[i+1]
        s.d F24, v4(R2)     ; save F24 in v4[i]
        s.d F25, v5(R2)     ; save F25 in v5[i]

        ; second unroll

        mul.d F23, F13, F14 ; v3[i+1] = v1[i+1]*v2[i+1];
        
        ;update loop control registers
        daddi R2, R2, 8     ; move R2 to current iteration
        daddi R3, R3, 8     ; move to next iteration
        s.d F23, v3(R2)     ; store F23 in v3[i+1]

        div.d F24, F23, F14 ; v4[i+1] = v3[i+1]/v2[i+1];
        add.d F25, F24, F14 ; v5[i+1] = v4[i+1]+v2[i+1];

        l.d F15, v1(R3)     ; load from memory v1[i+2]
        l.d F16, v2(R3)     ; load from memory v2[i+2]
        s.d F24, v4(R2)     ; save F24 in v4[i+1]
        s.d F25, v5(R2)     ; save F25 in v5[i+1]
        
        ; third unroll

        mul.d F23, F15, F16 ; v3[i+1] = v1[i+2]*v2[i+2];
        
        ;update loop control registers
        daddi R2, R2, 8     ; move R2 to current unroll
        daddi R3, R3, 8     ; move to next iteration
        s.d F23, v3(R2)     ; store F23 in v3[i+2]

        div.d F24, F23, F16 ; v4[i+2] = v3[i+2]/v2[i+2];
        add.d F25, F24, F16 ; v5[i+2] = v4[i+2]+v2[i+2];

        l.d F15, v1(R3)     ; load from memory v1[i+3]
        l.d F16, v2(R3)     ; load from memory v2[i+3]
        s.d F24, v4(R2)     ; save F24 in v4[i+2]

        daddi R1, R1, -1    ; update number of iterations left

        ;jump control section
        bnez R1, LOOP       ; if other iterations are required, go to LOOP
        s.d F25, v5(R2)     ; save F25 in v5[i+2]

        ;end program
        HALT

