*-----------------------------------------------------------
* Program    : Space Invaders
* Written by : Claire Blackshaw
* Date       :
* Description: A space invaders game
*-----------------------------------------------------------
START   ORG     $1000

bottom  EQU     250             * Height of Play
width   EQU     200             * Playfield Width
move_sp EQU     6               * Movement Speed
cooldwn EQU     3               * Cooldown between shots
*------------------------------------
*       START GAME
*------------------------------------
        LEA     FOE_START,A1
        LEA     FOE_END,A2
        LEA     BULL_START,A3
        LEA     BULL_END,A4
        
        BSR mode_2buffer
*------------------------------------
*       GAME LOOP
*------------------------------------
LOOP                       *---------- Main Loop
        BSR get_time
        MOVE.L  D1,D7
        
        BSR swap_buffers        * Swap and Clear
        BSR clear_screen
        
        BSR Get_Input
        BSR Update_Foes
        BSR Update_Bullets
        BSR Update_Score
        BSR Draw_Score
        BSR Draw_Cooldown
    
Time_Kill                     *------ Time Loop
        BSR get_time
        SUB.L   D7,D1           * Kill Time to stop OverRun
        CMP.W   #5,D1
        BLT     Time_Kill     *------
        
        BRA LOOP           *-----------
        
        BRA END_OF_PROG

*================================================================================
*================================================================================
        
*------------------------------------
*       Get Input
*------------------------------------
Get_Input
        MOVE.L  #$1B202527,D1   * Check [ESC|SPACE|<|>]
        BSR     check_key
        
        CMP.L   #$00000000,D1   * No Key Pushed
        BEQ     Skip_ESC
        
        CMP.B   #$00,D1         * -> Key not Pushed?
        BEQ     Skip_RIGHT
        ADD.W   #move_sp,PLAYER * Move Right
        CMP.W   #width,PLAYER  * Check if out of Bounds
        BLE     Skip_RIGHT
        MOVE.W  #width,PLAYER  * Correct
        
Skip_RIGHT
        LSR.L   #8,D1           * Shift
        CMP.B   #$00,D1         * <- Key not Pushed?
        BEQ     Skip_LEFT     
        SUB.W   #move_sp,PLAYER * Move Left
        CMP.W   #0,PLAYER       * Check if out of Bounds
        BGE     Skip_LEFT
        MOVE.W  #0,PLAYER       * Correct
        
Skip_LEFT
        LSR.L   #8,D1           * Shift
        CMP.B   #$00,D1         * SPACE key not Pushed?
        BEQ     Skip_SPACE
        BSR     Shoot           * Shoot
Skip_SPACE
        LSR.L   #8,D1           * Shift
        CMP.B   #$00,D1         * ESC key not Pushed?
        BEQ     Skip_ESC
        BRA     END_OF_PROG     * Exit Prog
Skip_ESC
        
        BSR Draw_Player
        RTS
        
*------------------------------------
*       Shoot
*------------------------------------
Shoot
        MOVE.B  COOLDOWN,D0     * Check Cooldown
        CMP.B   #0,D0           *  If cool then go ahead
        BGT     Shoot_Exit
        MOVE.B  #cooldwn,COOLDOWN
        
        MOVE.L  A3,A6          * Load Start of Bullets
Shoot_Next
        CMP     A6,A4           * Check if End of LIST
        BLT     Shoot_Exit      *---------- DEBUG!!
        
        MOVE.W  (A6)+,D3        
        CMP.W   #0,D3
        BNE     Shoot_Next
        SUB.L   #2,A6
        MOVE.W  PLAYER,D0
        MOVE.B  D0,(A6)+
        MOVE.B  #bottom,(A6)+     
        
Shoot_Exit
        RTS

*================================================================================
*================================================================================

*------------------------------------
*       Update Enemies
*------------------------------------
Update_Foes
        MOVE.L  A1,A6
Foe_Loop                      *----------- Foe Loop
        MOVEM.W (A6),D3-D4              * Load Foe
        
        CMP.W   #bottom,D4                 * Past Margin
        BLE Foe_Valid
        BSR     Rand_Gen
        MOVE.B  #0,D4
        MOVE.W  #0,SCORE
        
Foe_Valid        
        ADD.W   #1,D4
        MOVEM.W D3-D4,(A6)              * Save Foe
        ADD.L   #4,A6
        
        BSR     Draw_Foe                * Draw Foe

        CMP     A6,A2                   * Check if End of LIST
        BGT     Foe_Loop      *----------
        
        RTS

*------------------------------------
*       Update Bullets
*------------------------------------
Update_Bullets
        MOVE.L  A3,A6
        CMP.B   #0,COOLDOWN
        BEQ     Bull_Loop
        SUB.B   #1,COOLDOWN
Bull_Loop                      *----------- Bullet Loop
        CMP.L   A6,A4                   * Check if End of LIST
        BLE     Bull_Exit
        MOVE.W (A6),D3                  * Load Bullet
        MOVE.B  D3,D4
        LSR     #8,D3
        
        CMP.B   #4,D4                   * Check if Bullet is valid
        BHI     Bull_Move               * Move Bullet
        MOVE.W  #0,(A6)+
        BRA     Bull_Loop        
        
Bull_Move
        SUB.B   #3,D4
        
        BSR     Collision_Check
        
        MOVE.B  D3,D0
        LSL     #8,D0
        MOVE.B  D4,D0
        MOVE.W  D0,(A6)+                * Move and Save
        BSR     Draw_Bullet
        BRA     Bull_Loop        
        
Bull_Exit        
        RTS
        
*------------------------------------
*       Update Score
*------------------------------------
Update_Score
        MOVEM   D0-D4,-(SP)
        MOVE    A0,-(SP)

        MOVE.W  SCORE,D4        ** Load Score and Str **
        LEA     SCORE_TAIL,A0
        CLR.L   D3

_usb_loop
        DIVU    #10,D4
        SWAP    D4
        MOVE.B  D4,D0 
        
        ADD.B   #'0',D0         ** Add current Digit to Str **
        MOVE.B  D0,-(A0)
        CLR.W   D4
        
        SWAP    D4
        BNE     _usb_loop

        MOVE.L  #5,D2
_usc_loop
        MOVE.B  #'0',-(A0)
        DBRA    D2,_usc_loop
     
        MOVE    (SP)+,A0
        MOVEM   (SP)+,D0-D4
        RTS

*================================================================================
*================================================================================

*------------------------------------
*       Collision Check
*  Assumes D3,D4 holds Bullet Pos
*------------------------------------
Collision_Check
        MOVEM.L D0-D2,-(SP)     * Preserve Data Registers

        MOVE.L  A1,A5
Col_Loop                        * Start fetching Foes
        CMP     A5,A2           
        BLT     Col_Exit        * End Loop if end of loop
        
        CLR.L   D1              * Clear Reg
        CLR.L   D2
        
        MOVEM.W (A5)+,D1-D2     * Load Foe
        
        CMP.W   #bottom,D4         * Past Margin
        BGT     Col_Loop        * Check next Foe
        
        ** Do Collision Check **
        SUB.W   D3,D1           * Get Difference
        BCC     _Col_X_Skip_Neg
        NEG.W   D1
_Col_X_Skip_Neg
        
        SUB.W   D4,D2
        BCC     _Col_Y_Skip_Neg
        NEG.W   D2
_Col_Y_Skip_Neg
        
        CMP     #8,D1
        BHI     Col_Loop        * No Collision
        CMP     #5,D2
        BHI     Col_Loop        * No Collision
        
        ** Collision Detected **
        CLR.L   D3
        CLR.L   D4
        
        SUB     #2,A5
        MOVE    #0,(A5)         * Reset Foe
        ADD.W   #1,SCORE
        
Col_Exit        
        MOVEM.L (SP)+,D0-D2     * Restore Data Registers
        
        RTS

*================================================================================
*================================================================================

*------------------------------------
*       Draw Space Invader
*
* 01111111110   03FE
* 01000000010   ,$0202
* 01001010010   ,$0252
* 10100000101   ,$0505
* 10001010001   ,$0451
* 01110101110   ,$03AE
*------------------------------------
FOE_SPRITE      DC.W    $03FE,$0202,$0252,$0505,$0451,$03AE,$0BAD
        
Draw_Foe     
        MOVE.L  #$00FF6060,D1
        BSR     set_pen_col
        MOVE.W  D4,D2
        MOVE.W  D3,D4
        SUB.W   #4,D2
        SUB.W   #5,D4
        
        LEA     FOE_SPRITE,A0        * Load Sprite
        BSR     Draw_Sprite
        RTS

*------------------------------------
*       Draw Player
* 
* 0000000110000000      0x0180
* 0000000110000000      0x0180
* 0001010110101000      0x15A8
* 0010101111010100      0x2BD4
* 0101011001101010      0x566A
* 1011100000011101      0xB81D
* 1111000110001111      0xF18F
* 0111111111111110      0x7FFE
*------------------------------------
PLAYER_SPRITE   DC.W    $0180,$0180,$15A8,$2BD4,$566A,$B81D,$F18F,$7FFE,$0BAD

Draw_Player
        MOVE.L  #$0000FF00,D1
        BSR     set_pen_col
        MOVE.W  #bottom,D2                 * D1,D2 = X,Y
        SUB.W   #5,D2
        MOVE.W  PLAYER,D4
        SUB.W   #8,D4
        
        LEA     PLAYER_SPRITE,A0        * Load Sprite
        BSR     Draw_Sprite
        RTS
        
*------------------------------------
*       Draw Bullet
*------------------------------------
BULLET_SPRITE   DC.W    2,5,5,5,5,7,0,5,2,5,2,$0BAD

Draw_Bullet
        MOVE.L  #$000000FF,D1
        BSR     set_pen_col
        MOVE.W  D4,D2
        MOVE.W  D3,D4
        SUB.W   #4,D2
        SUB.W   #1,D4
        
        LEA     BULLET_SPRITE,A0        * Load Sprite
        BSR     Draw_Sprite
        RTS
        
*------------------------------------
*       Draw Sprite
*       D1 = Top Border
*       D4 = Left Border
*       Sprite Terminated by $0BAD
*------------------------------------
Draw_Sprite
        MOVE.W  #82,D0
_DSPR_Loop
        MOVE.W  (A0)+,D3                * Load Line into Register
        MOVE.W  D4,D1                   * Reset X
        ADD.W   #1,D2                   * Move down a line
_DSPR_Roll
        LSR.W   #1,D3                   * Get Bit
        BCC     _DSPR_NoPixel            * If 1 then
        TRAP    #15                     * -- Draw Pixel
_DSPR_NoPixel                    
        ADD.W   #1,D1                   * Move right one pixel

        CMP     #0,D3                   * If any pixels left carry on rolling
        BNE     _DSPR_Roll
        CMP     #$0BAD,(A0)             * If next line is "BAD" then end of sprite
        BNE     _DSPR_Loop
        
        RTS
*------------------------------------
*       Draw Score
*------------------------------------
Draw_Score
        MOVE    A1,-(SP)        ** Preserve Reg **
        MOVEM   D0-D2,-(SP)
        
        MOVE.L  #$00333333,D1   ** Setup Background **
        MOVE.W  #81,D0
        TRAP    #15     
        
        LEA     SCORE_STR,A1    ** Display Score **
        MOVE.L  #95,D0
        MOVE.L  #5,D1
        MOVE.L  #bottom,D2
        ADD.L   #5,D2
        TRAP    #15
        
        MOVEM   (SP)+,D0-D2     ** Restore Reg **
        MOVE    (SP)+,A1
        RTS
        
*------------------------------------        
*       Draw Cooldown Bar
*------------------------------------
Draw_Cooldown
        MOVEM   D0-D6,-(SP)
        CLR     D6              ** Setup Loop Varible **
        MOVE.B  COOLDOWN,D6
        CMP     #0,D6           * Check anything to be drawn
        BEQ     _drc_exit       
        SUB.B   #1,D6           * Comp for -1 exit
        
                                ** Setup Colours
        MOVE.L  #$00FF00FF,D1
        MOVE.W  #80,D0
        TRAP    #15
        
        MOVE.L  #$00990099,D1
        MOVE.W  #81,D0
        TRAP    #15        
        
        MOVE.L  #bottom,D2      ** Setup Counter Square **
        ADD.W   #15,D2
        MOVE.L  #width,D3
        MOVE.L  D2,D4
        ADD.W   #10,D4
        MOVE.L  D3,D1
        SUB.W   #10,D1
        MOVE.W  #87,D0
        
_drc_loop
        TRAP    #15
        SUB.W   #1,D1
        MOVE.W  D1,D3
        SUB.W   #10,D1
        DBRA    D6,_drc_loop
        
_drc_exit
        MOVEM   (SP)+,D0-D6       
        RTS
        
*------------------------------------
*       Random Generation
*------------------------------------
Rand_Gen
        MOVEM   D0-D2,-(SP)
        MOVE.W  SEED,D0
        DIVS    #2836,D0

        MOVE.W  D0,D1
        MULS    #839,D1

        SWAP    D0
        MOVE.W  D0,D2
        MULS    #277,D2

        ADD.W   D1,D2
        MOVE    D2,SEED 
        MOVEM   (SP)+,D0-D2
        RTS

*------------------------------------
*          DATA
*------------------------------------
FOE_START       DC.W    10,0,35,0,60,0,85,0,110,0,135,0,160,0,185,0
*                DC.W    20,10,50,10,90,10,120,10,150,10,180,10
                DC.W    30,20,70,20,130,20,170,20
*                DC.W    50,30,150,30        ** X,Y **
        
FOE_END         DC.W    $FFFF

BULL_START      DCB.W   60,$0000
BULL_END        DC.W    $FFFF
COOLDOWN        DC.B    $00

PLAYER          DC.W    0             * Y Pos of Player
SCORE           DC.W    0,0
SEED            DC.W    $ABCD

SCORE_STR       DC.B    'Score: 000000000'
SCORE_TAIL      DC.B    0
        
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
