.syntax unified
.thumb

.global _LDREX
.type   _LDREX, %function

.global _STREX
.type   _STREX, %function

.global _DMB
.type   _DMB, %function

.global _CLREX
.type   _CLREX, %function

.global _DISABLE_INTR
.type   _DISABLE_INTR, %function

.global _ENABLE_INTR
.type   _ENABLE_INTR, %function


_LDREX:
    LDREX   R0, [R0]        
    BX      LR              

_STREX:
    STREX R2, R1, [R0]    
    MOV R0, R2         
    BX  LR

_DMB:
    DMB SY
    BX LR


_CLREX:
    CLREX
    BX LR


_DISABLE_INTR:
    CPSID i
    BX LR   

_ENABLE_INTR:
    CPSIE i
    BX LR   
