.extern XYZ
    .entry MAIN

KINITIALVALUE: sub *r4   ,    *r3

MAIN:   mov *r3, LENGTH

LOOP:   jmp L1

    macr M1
sub *r1, *r9
    bne END
    endmacr%%%

    .entry LOOP
prn #-5
bne LOOP

XYZ: mov *r4, *r2

M1

L1:     inc K
bne LOOP

stop

STR:    .string "666"abcdef"
LENGTH: .data 6, -9, 15
K:      .data 4    ,  ,  -55,4,4,4,6
.data ,,5,8,9
    macr M1
mov r1, val
add r2, r1
    endmacr %
M1
ABC: mov XYZ, *r3
reg1: .data 6,5,-555,66
reg2: .data 6,5,-555,66
val: .string "asfas   %%dfjk"