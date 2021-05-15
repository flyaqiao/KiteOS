                PRESERVE8
                THUMB
                AREA    |.text|, CODE, READONLY
                IMPORT  OsReadNowSp
                IMPORT  OsSaveNowSp
                IMPORT  OsFindActTask
                IMPORT  OsInitTick 

OsHighStart     PROC                                  ; 启动最高优先级任务
                EXPORT  OsHighStart
                CPSID   I
                BL      OsInitTick      
                BL      OsFindActTask 
                MSR		  MSP, R0
                POP     {R8 - R11, R4 - R7}                     ;   Pop new task's R8-R11 (into R0-R3), R4-R7
                ADD     SP, #0x10
                POP     {R0 - R3}                     ;   Pop new task's R12, PC, LR, PSR into (R0, R1, R2, R3, respectively)
                MOV     R12, R0
                MOV     LR,  R1
                MSR     PSR, R3
                PUSH    {R2}                          ;   Save PC
                SUB     SP, #0x1C                 
                POP     {R0 - R3}                     ;   Pop new task's R0-R3
                ADD     SP, #0x0C
                CPSIE   I                             ;   Enable interrupts
                POP    {PC}                           ;   Pop new task's PC
                ENDP
OsIntSwitch     PROC                                  ; 切换到最高优先级任务
                EXPORT  OsIntSwitch
                CPSID   I
                SUB     SP, #0x10                     ; 空出四个DWORD空间,将存放R12,LR,PC,PSR
                PUSH    {R0 - R3}
                
                ADD     SP, #0x20
                MRS     R3, PSR
                MOV		  R2, LR
                MOV		  R1, LR
                MOV     R0, R12
                PUSH    {R0 - R3}
                
                SUB     SP, #0x10
                MOV 	  R0, R8
                MOV 	  R1, R9
                MOV 	  R2, R10
                MOV 	  R3, R11
                PUSH    {R0 - R7}   
                MRS		  R0, MSP
                BL		  OsSaveNowSp
                BL		  OsFindActTask
                MSR		  MSP, R0
                
                POP    {R8 - R11, R4 - R7}                      ;   Pop new task's R8-R11 (into R0-R3), R4-R7
                
                ADD     SP, #0x10
                POP    {R0 - R3}                      ;   Pop new task's R12, PC, LR, PSR into (R0, R1, R2, R3, respectively)
                MOV     R12, R0
                MOV     LR,  R1
                MSR     PSR, R3
                
                PUSH   {R2}                           ;   Save PC
                
                SUB     SP, #0x1C                 
                POP    {R0 - R3}                      ;   Pop new task's R0-R3
                ADD     SP, #0x0C 
                CPSIE   I                             ;   Enable interrupts
                POP    {PC} 
                ENDP
OsTaskSwitch    PROC                                  ; 切换到最高优先级任务
                EXPORT  OsTaskSwitch
                CPSID   I
                SUB     SP, #0x10
                PUSH    {R0 - R3}
                
                ADD     SP, #0x20
                MRS     R3, PSR
                MOV		  R2, LR
                MOV		  R1, LR
                MOV     R0, R12
                PUSH    {R0 - R3}
                
                SUB     SP, #0x10
                MOV 	  R0, R8
                MOV 	  R1, R9
                MOV 	  R2, R10
                MOV 	  R3, R11
                PUSH    {R0 - R7}   
                MRS		  R0, MSP
                BL		  OsSaveNowSp
                BL		  OsFindActTask
                MSR		  MSP, R0
                
                POP    {R8 - R11, R4 - R7}                      ;   Pop new task's R8-R11 (into R0-R3), R4-R7
                
                ADD     SP, #0x10
                POP    {R0 - R3}                      ;   Pop new task's R12, PC, LR, PSR into (R0, R1, R2, R3, respectively)
                MOV     R12, R0
                MOV     LR,  R1
                MSR     PSR, R3
                
                PUSH   {R2}                           ;   Save PC
                
                SUB     SP, #0x1C                 
                POP    {R0 - R3}                      ;   Pop new task's R0-R3
                ADD     SP, #0x0C 
                CPSIE   I                             ;   Enable interrupts
                POP    {PC} 
                ENDP
OsEnterCritical PROC                      ; 关中断
                EXPORT  OsEnterCritical
                CPSID   I
                MOVS    R0, #0
                BX      LR
                ENDP
OsExitCritical  PROC                      ; 开中断
                EXPORT  OsExitCritical
                CPSIE   I
                MOVS    R0, #0
                BX      LR
                ENDP

                ALIGN
                END
