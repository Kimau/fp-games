*-----------------------------------------------------------
* Program    : Falling Bircks
* Written by : Claire Blackshaw
* Date       :
* Description: A simple test game
*-----------------------------------------------------------
START   ORG     $1000

*------------------------------------
*       MAIN
*------------------------------------
        LEA     BLOCK_START,A1
        LEA     PLAY_START,A3
        
        MOVE.L  #RED,D1
        BSR set_pen_col
        BSR mode_2buffer

LOOP
        BSR get_time            * Get Current Time
        MOVE.L  D1,D7
        
        BSR swap_buffers        * Swap and Clear
        BSR clear_screen
    
        BSR Do_Cubes            * Handle Cubes
        BSR Do_Player           * Handle Players

Time_Kill                       * Kill Time to stop OverRun
        BSR get_time
        SUB.L   D7,D1
        CMP.W   #5,D1
        BLT     Time_Kill
        
        BRA LOOP
        
        BRA END_OF_PROG
*------------------------------------
*       FUNCTIONS
*------------------------------------
Do_Cubes
        MOVE.W  A1,A2
        DBLOOP  D6 = #3                 * Loop Four Times
        BSR     Update_Cube
        UNLESS
        
        RTS

Update_Cube
        MOVEM.W (A2),D1-D2
        ADD.W   #1,D2
        MOVEM.W D1-D2,(A2)
        ADD.W   #4,A2
        BSR     Draw_Cube
        RTS

Draw_Cube
        MOVE.W  D1,D3
        MOVE.W  D2,D4
        SUB.W   #4,D1
        SUB.W   #4,D2
        ADD.W   #4,D3
        ADD.W   #4,D4
        MOVE.B  #91,D0
        TRAP    #15
        RTS

Do_Player
        MOVE.W  A3,A4
        MOVE.W  (A4),D2
        
        BSR     Get_Input
        MOVE.W  D2,(A4)+
        MOVE.W  D2,D1
        BSR     Draw_Player        
        BSR     Update_Bullet
        RTS
        
Get_Input
        MOVE.W  #0,D5
        MOVE.B  #19,D0
        MOVE.L  #$1B202527,D1
        trap    #15
        
        CMP.L   #$00000000,D1
        BEQ     Skip_ESC
        
        CMP.B   #$00,D1
        BEQ     Skip_RIGHT
        ADD.W   #6,D2        
Skip_RIGHT
        LSR.L   #8,D1
        CMP.B   #$00,D1
        BEQ     Skip_LEFT
        SUB.W   #6,D2
Skip_LEFT
        LSR.L   #8,D1
        CMP.B   #$00,D1
        BEQ     Skip_SPACE
        BSR     Shoot
Skip_SPACE
        LSR.L   #8,D1
        CMP.B   #$00,D1
        BEQ     Skip_ESC
        BRA     END_OF_PROG        
Skip_ESC
        RTS

Draw_Player
        MOVE.W  D1,D3
        MOVE.W  #190,D2
        MOVE.W  #200,D4
        SUB.W   #8,D1
        ADD.W   #8,D3
        MOVE.B  #90,D0
        TRAP    #15
        RTS    
        
Shoot   
        MOVE.W  D2,D5
        RTS
        
Update_Bullet
        MOVEM.W (A4),D1-D2              * Load Bullet
        
        CMP.W   #$A000,D1
        BEQ     End_of_Bullet_List      * End of Bullet Marker
        
        SUB.W   #4,D2                   * Move Bullet
        BSR     Collide_Bullet          * Check Collisions
       
        BRA     Update_Bullet           * Goto next Bullet
End_of_Bullet_List        
        CMP.W   #0,D5                   * If Shoot has been Pressed
        BEQ     End_of_Bullet
        
        MOVE.L  A4,A5                   * Check maxium shots
        SUB.L   A3,A5
        CMP.L   #78,A5
        BGE     End_of_Bullet
        
        MOVE.W  D5,D1                   * Make new Bullet
        MOVE.W  #200,D2
        MOVE.W  #$A000,D3
        MOVE.W  #$A000,D4
        MOVEM.W D1-D4,(A4)              * Save new Bullet
End_of_Bullet
        RTS
        
Collide_Bullet
        CMP.W   #0,D2
        BGT     Valid_Pos
        
        MOVE.L  A4,A5                   * Save Mem Marker
        BRA     Delete_Bullet           
Done_Del
        BRA     Exit_CB
        
Valid_Pos
        MOVEM.W D1-D2,(A4)              * Save Bullet
        BSR     Draw_Bullet             * Draw Bullet
        
Exit_CB        
        ADD.L   #4,A4                   * Move Mem Marker
        RTS

Delete_Bullet
        MOVEM.W (A5),D1-D4              * Load Bullets
        MOVEM.W D3-D4,(A5)
        CMP.W   #$A000,D3               * Bullet is EOL
        BEQ     Done_Del                * Exit List
        ADD.L   #4,A5
        BRA     Delete_Bullet

Draw_Bullet
        MOVE.W  D1,D3
        MOVE.W  D2,D4
        SUB.W   #1,D1
        SUB.W   #2,D2
        ADD.W   #1,D3
        ADD.W   #2,D4
        MOVE.B  #90,D0
        TRAP    #15
        RTS

*------------------------------------
*          DATA
*------------------------------------
BLOCK_START     DC.W    10,1,250,7,40,1,75,1
PLAY_START      DC.W    100
                DCB.W   40,$A000

*------------------------------------
*        INCLUDES
*------------------------------------
        INCLUDE "BasicIO.x68"

END_OF_PROG
        MOVE.B  #9,D0
        TRAP    #15             Halt Simulator
        END     START



*~Font name~Courier New~
*~Font size~10~
*~Tab type~1~
*~Tab size~8~
