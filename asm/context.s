.syntax unified
.thumb

.global PendSV_Handler
.type   PendSV_Handler, %function

PendSV_Handler:
    CPSID   I
    MRS     R0, PSP
    STMDB   R0!, {R4-R11}
    LDR     R1, =current_task
    LDR     R1, [R1]
    STR     R0, [R1]
    LDR     R2, =next_task
    LDR     R2, [R2]
    LDR     R0, [R2]
    LDR     R3, =current_task
    STR     R2, [R3]
    LDMIA   R0!, {R4-R11}
    MSR     PSP, R0
    CPSIE   I
    LDR     LR, =0xFFFFFFFD
    BX      LR
    