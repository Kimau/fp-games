#ifndef PLAYER_H
#define PLAYER_H

#include "pawn.h"
#include "footsteps.h"
#include <SDL.h>
#include <memory.h>

/* Player 
 *
 * The player object defines the player object and all of it's abilities and stats etc.
 * It also handles all movement and logic of the player object
 * Player uses a special KeyboardController to control the player instead of the usual AI controller
 *
 * Player uses the singleton design pattern to insure that no more than one player can ever be created. 
 *
 * Pawn is inherited as a protected base in order to restrict access to the controller (since only one type of controller
 * may be assigned to Player: KeyboardController)
 */

class Player : protected Pawn
{
protected:  
  /* KeyboardController
   * 
   * Allows a pawn (commonly the player) to be controlled by the keyboard instead of through an AI controller
   * Currently only for use by Player
   */
  class KeyboardController : public Controller
  {
  protected:
    enum ControllerKeys
    {
      LEFT = 0,
      RIGHT,
      UP,
      DOWN,
      USE,
      NKEYS
    };
    bool keymap[5]; //if key in keymap == true then the key is pressed
  public:
    /* constructor
     */
    KeyboardController();
    
    /* execute
     * 
     * Read keystrokes and perform actions based on those
     */
    virtual void execute(Pawn* pawn);
    
    /* keyPressed
     *  
     * returns true if keypress was handled
     */
    bool keyPressed(SDLKey key);
    
    /* keyUp
     *  
     * returns true if keyup was handled
     */
    bool keyUp(SDLKey key);
  };

protected:
  static Player instance;
  uint texture;
  uint gold;

  /* constructors:
   * 
   * Hidden to prevent construction
   */
  Player();
  Player(Player&);
  Player& operator=(Player&);
  Footsteps prints;
public:
  static Player* getInstance() { return &instance; }
  bool biteHim();

  //void  setPosition(float posX, float posY, float posZ = 0.0f) { Pawn::setPosition(posX,posY,posZ); }
  KeyboardController* getController() const { return (KeyboardController*)Pawn::getController(); } 
  bool checkCollide(Pawn& mover);
  void update(Time currentTime) { Pawn::update(currentTime); }
  __inline bool moveTo(pointXYZ point)  { return Pawn::moveTo(point); }	  
  __inline Vector<float,2> getWorldPos()  { return Pawn::getWorldPos(); }	  
  __inline pointXYZ getGridPos()  { return Pawn::getGridPos(); }	  
  __inline Footsteps* getFootsteps() { return &prints; }
  bool takeStep(Time currentTime)
  { 
    if(Pawn::takeStep(currentTime))
    {
      prints.takeStep(getGridPos());
      return true;
    }
    return false;
  }	  
  
  /* anihilateMe
   */
  virtual void anihilateMe();

  /* draw
   */
  void draw();

  /* addGold
   */
  void addGold(uint value);

  /* reload
   *
   * Reload resources.
   */
  bool reload();
  
  /* unload
   *
	 * Clean up all resources in use (i.e. delete textures etc.)
	 */
  void unload();

  /* openDoors
   *
   * Open all doors around the player (in the adjacent cells)
   */
  void openDoors();
};

#endif

