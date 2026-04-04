.syntax unified
.thumb

.extern current_task

.global sched_start
.type   sched_start, %function

sched_start:
    LDR     R0, =current_task       
    LDR     R0, [R0]                
    LDR     R0, [R0]                
    ADD     R0, R0, #32
    MSR     PSP, R0
    MOV     R0, #0x03
    MSR     CONTROL, R0
    ISB                        
    POP     {R0-R3, R12, LR}         
    POP     {PC}                    

