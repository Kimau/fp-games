*------------------------------------
*       DEFINES
*------------------------------------
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

*------------------------------------
*       FUNCTIONS
*------------------------------------
print_msg
        MOVEQ   #14,D0          14 - Print String without /n
        TRAP    #15
        RTS
        
get_char
        MOVEQ   #5,D0           Store Char in D1.B
        TRAP    #15
        RTS
        
get_num
        MOVEQ   #4,D0           Store Number in D1.L
        TRAP    #15
        RTS        
       
get_string
        MOVEQ   #2,D0           Store String in A1
        TRAP    #15             Store Lenth in D1.W
        RTS           
        
set_text
        MOVEQ   #21,D0          Set Text Colour to D1.L
        TRAP    #15             D2.L [Font//Size//S/U/I/B]
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
        MOVE.B  #82,D0          Set Pixel
        TRAP    #15             X = D1.W  
        RTS                     Y = D2.W
        
get_pixel
        MOVE.B  #83,D0          COLOUR = D0.L
        TRAP    #15             X = D1.W  
        RTS                     Y = D2.W

mode_2buffer
        MOVE.B  #92,D0
        MOVE.B  #17,D1
        TRAP    #15
        RTS
        
swap_buffers
        MOVE.B  #94,D0          Swaps off screen Buffer
        TRAP    #15
        RTS

clear_screen
        MOVE.B  #11,D0           Clears the Screen with Pos Cursor
        MOVE.W  #$FF00,D1        
        TRAP    #15
        RTS

get_time 
        MOVEQ           #8,d0    Get Time in 1/100ths of a Second
        TRAP            #15
        RTS
*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
