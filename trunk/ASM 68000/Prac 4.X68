*-----------------------------------------------------------
* Program    : 
* Written by : Claire Blackshaw
* Date       :
* Description:
*-----------------------------------------------------------
CR      EQU     $0D        
LF      EQU     $0A

*-----------------------
*               START OF PROGRAM
*-----------------------
START   ORG     $400400

        MOVE.L  #$12345678,D0   
        LEA.L   data2,A5        
        ADD.W   (A5),D0         Add D0 & 5678(low word of data2)
        SUBA.L  #2,A5
        EXT.L   D0
        NEG.L   D0
        CLR.B   D0
        ASR.W   #3,D0
        LSL.B   #1,D0
        ROXR.W  #2,D0
        OR.W    (A5),D0
        EOR.W   #$FFFF,(A5)
        OR.W    (A5),D0         Logical OR
     
        BRA     END_PROG
*-----------------------
*       FUNCTIONS
*-----------------------
print_msg
        MOVE.B  #14,D0          14 - Print String without /n
        TRAP    #15
        RTS
                
get_input
        MOVE.B  #4,D0
        TRAP    #15             Capture Input
                RTS
                
*-----------------------
*       DATA
*-----------------------
data1   DC.B 20,44
data2   DC.W $10FF,$5ABC

END_PROG
        END     START
*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
