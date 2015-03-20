*-----------------------------------------------------------
* Program    : Test
* Written by :
* Date       :
* Description:
*-----------------------------------------------------------
CR      EQU     $0D        
LF      EQU     $0A

BLACK       EQU     $00000000
MAROON      EQU     $00000080
GREEN       EQU     $00008000
OLIVE       EQU     $00008080
NAVY        EQU     $00800000
PURPLE      EQU     $00800080
TEAL        EQU     $00808000
GRAY        EQU     $00808080
RED         EQU     $000000FF
LIME        EQU     $0000FF00
YELLOW      EQU     $0000FFFF
BLUE        EQU     $00FF0000
FUCHSIA     EQU     $00FF00FF
AQUA        EQU     $00FFFF00
LTGRAY      EQU     $00C0C0C0
WHITE       EQU     $00FFFFFF

START   ORG     $1000

        BSR     get_loop_count
        LEA     MSG_HELLO,A1
       
TLOOP  BSR     print_msg
       
        SUBQ.B  #1,D7           Subtract 1 from Counter

        CMP     #0,D7           If not zero then loop
        BGT     TLOOP
     
CLOOP   MOVE.W  #200,D2         Reset Y
YLOOP   MOVE.W  #200,D1         Reset X
XLOOP   BSR pixel_drip

        SUBQ.W  #1,D1           Move left by one pixel
        CMP     #0,D1           If X is 0
        BGT     XLOOP
        
        SUBQ.W  #1,D2           Move down by one pixel
        CMP     #0,D2           If Y is 200
        BGT     YLOOP
        
        ADDQ.B  #1,D7        
        BRA     CLOOP           Endless Loop
       
        MOVE.B  #9,D0
        TRAP    #15             Halt Simulator
       
        BRA     END_PROG
*-----------------------
*       FUNCTIONS
*-----------------------
get_loop_count
        MOVEQ   #21,D0
        MOVE.L  #PURPLE,D1
        TRAP    #15

        LEA     MSG_INPUT,A1
        BSR     print_msg       Print Input Prompt
 
        MOVEQ   #21,D0
        MOVE.L  #RED,D1
        TRAP    #15
        
        MOVEQ   #4,D0
        TRAP    #15             Capture Input
        MOVE.B  D1,D7
        
        MOVEQ   #21,D0
        MOVE.L  #LIME,D1
        TRAP    #15
       
        RTS
        
pixel_drip
*____________________________If Black then Ignore
        BSR     get_pixel
        CMP     #0,D0           
        BEQ     END_DRIP
        CMP.L   #$00FFFFFF,D0
        BEQ     END_DRIP
        BSR     split_bgr
*____________________________Must draw on Black
        ADDQ.B  #1,D2
        BSR     get_pixel
        CMP     #0,D0           
        BNE     SKIP_DRIP
*____________________________
        CMP     #0,D7
        BNE     HALF_DRIP
        ADDQ.B  #1,D2
        BSR     half_bgr
        BSR     join_bgr
        BSR     set_pixel
        SUBQ.B  #1,D2
        MOVE.L  #$00FFFFFF,D6
        BSR     set_pixel
        SUBQ.B  #1,D2
        BRA     END_DRIP
*____________________________
HALF_DRIP
        BSR     half_bgr
        BSR     join_bgr
        BSR     set_pixel
SKIP_DRIP
        SUBQ.B  #1,D2
*____________________________End of Drip
END_DRIP        
        RTS
        
split_bgr
        MOVE.B  D0,D3           Blue to D3
        LSR.L   #8,D0
        MOVE.B  D0,D4           Green to D4
        LSR.L   #8,D0
        MOVE.L  D0,D5           Red to D6
        RTS

join_bgr
        LSL.L   #8,D4           Align Registers
        LSL.L   #8,D5
        LSL.L   #8,D5
        
        MOVE.L  D5,D6
        OR.W    D4,D6
        OR.B    D3,D6
        RTS

flick_bgr
        NOT.B   D3
        NOT.B   D4
        NOT.B   D5

half_bgr
        LSL.B  #1,D3
        LSL.B  #1,D4
        LSL.B  #1,D5
        RTS
        
print_msg
        MOVEQ   #14,D0          14 - Print String without /n
        TRAP    #15
        RTS
   
set_pen_col
        MOVE.B  #80,D0          Set Pen Colour to D1.L
        TRAP    #15
        RTS
        
set_fill_col
        MOVE.B  #81,D0          Set Fill Colour to D1.L
        TRAP    #15
        RTS
        
set_pixel
        MOVE.L  D1,D3           Swap Registers
        MOVE.L  D6,D1
        MOVE.B  #80,D0
        TRAP    #15             Set Pen Colour
        MOVE    D3,D1           Swap Back
        MOVE.B  #82,D0          Set Pixel
        TRAP    #15             X = D1.W  
        RTS                     Y = D2.W
        
get_pixel
        MOVE.B  #83,D0          COLOUR = D0.L
        TRAP    #15             X = D1.W  
        RTS                     Y = D2.W
        
*-----------------------
*       DATA
*-----------------------
MSG_HELLO       DC.B    'HELLO WORLD',CR,LF,0
MSG_INPUT       DC.B    'LOOP HOW MANY TIMES: ',0

END_PROG
        END     START



*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
