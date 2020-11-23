#include <iostream>
#include <cmath>
using namespace std;
#include "BearLibTerminal_0.15.7\Include\C\BearLibTerminal.h"
#include "gooseEscapeUtil.hpp"
#include "gooseEscapeActors.hpp"
#include "gooseEscapeConsole.hpp"
#include "gooseEscapeGamePlay.hpp"
#include "point.hpp"

//Static variables to store teleporter things:
static int teleportX1 = 0, teleportX2 = 0, teleportY1 = 0, teleportY2 = 0;

extern Console out;
/*
With graphics, screens are given an x,y coordinate system with the origin
in the upper left corner.  So it means the coordinate axes are:
---------------->   x direction
|
|
|
|
|
V

y direction 
*/
	
/*
    Print the game world
    
    The functions should draw characters to present features of the game
    board, e.g. win location, obstacles, power ups
*/

// print the game board function
void printGameBoard(int xChar, int yChar, int charToPut)
{
	int x_location_on_board = xChar;
	int y_location_on_board = yChar;
    terminal_put(x_location_on_board,y_location_on_board, charToPut);

}

void sendGameBoardCoordinates(int teleX1, int teleY1, int teleX2, int teleY2)
{
    teleportX1 = teleX1;
    teleportY1 = teleY1;
    teleportX2 = teleX2;
    teleportY2 = teleY2;
}

/*
    Do something when the goose captures the player
    
    If you want to attack or something else, this is the function you 
    need to change.  For example, maybe the two touch each other and
    then fight.  You could add a health to the Actor class that is
    updated.  Run, use weapons, it's up to you!
*/

bool captured(Actor const & player, Actor const & monster)
{
    return (player.get_x() == monster.get_x() 
         && player.get_y() == monster.get_y());
}

/*
    Move the player to a new location based on the user input.  You may want
    to modify this if there are extra controls you want to add.
    
    All key presses start with "TK_" then the character.  So "TK_A" is the a
    key being pressed.
    
    A look-up table might be useful.
    You could decide to learn about switch statements and use them here.
*/

void movePlayer(int key, Actor & player, int gameBoard[NUM_BOARD_Y][NUM_BOARD_X])
{
    int yMove = 0, xMove = 0;
    if (key == TK_UP)
        yMove = -1;
    else if (key == TK_DOWN)
        yMove = 1;
    else if (key == TK_LEFT)
        xMove = -1;
    else if (key == TK_RIGHT)
        xMove = 1;
        
    int playerX = player.get_x();
    int playerY = player.get_y();
       
    if (player.can_move(xMove, yMove) 
      && gameBoard[playerY+yMove][playerX+xMove] != SHALL_NOT_PASS)
      {
        //if(isTeleportable)
        //{
        //    teleportActor(player);
        //}
        //else
        //{
            updatePlayerLocation(player, xMove, yMove);
        //}
        
	  }
        
}

void updatePlayerLocation(Actor &player, int xMove, int yMove)
{
    int playerX = player.get_x();
    int playerY = player.get_y();
    player.update_location(xMove, yMove);
    terminal_put(playerX,playerY, BLANK_CHAR);
    terminal_refresh();
}

bool isTeleportable(int gameBoard[NUM_BOARD_Y][NUM_BOARD_X], Actor &player)
{
    return gameBoard[player.get_y()][player.get_x()] == TELEPORT;
}

void teleportActor(Actor &player)
{
    int playerX = player.get_x();
    int playerY = player.get_y();

    if(playerX == teleportX1)
    {
        player.setLocation(teleportX2, teleportY2);
    }
    else
    {
        player.setLocation(teleportX1, teleportY1);
    }

    updatePlayerLocation(player, 0, 0);
}

bool won(Actor &player, int gameBoard[NUM_SCREEN_Y][NUM_SCREEN_X])
{
    int playerX = player.get_x();
    int playerY = player.get_y();

    return (gameBoard[playerY][playerX] == WINNER);
}

void chasePlayer(Actor & monster, Actor const & player, int gameBoard[NUM_BOARD_Y][NUM_BOARD_X])
{
    int playerX = player.get_x();
    int playerY = player.get_y();

    int monsterX = monster.get_x();
    int monsterY = monster.get_y();

    int deltaX = 0;
    int deltaY = 0;
    // Moves the monster one unit in both X and Y towards player
    // If there is a wall in the way the monster will jump over unless wall is more then 1 unit thick

    //Player is Right of Monster
    if(playerX > monsterX)
    {
        deltaX++;
        if(gameBoard[monsterY][monsterX + 1] == SHALL_NOT_PASS && gameBoard[monsterY][monsterX + 2] != SHALL_NOT_PASS)
        {
            deltaX++;
        }
        else if(gameBoard[monsterY][monsterX + 1] == SHALL_NOT_PASS && gameBoard[monsterY][monsterX + 2] == SHALL_NOT_PASS)
        {
            deltaX--;
        }
    }
    //Player is Left of Monster
    else if(playerX < monsterX)
    {
        deltaX--;
        if(gameBoard[monsterY][monsterX - 1] == SHALL_NOT_PASS && gameBoard[monsterY][monsterX - 2] != SHALL_NOT_PASS)
        {
            deltaX--;
        }
        else if(gameBoard[monsterY][monsterX - 1] == SHALL_NOT_PASS && gameBoard[monsterY][monsterX - 2] == SHALL_NOT_PASS)
        {
            deltaX++;
        }
    }
    //Player is Below Monster
    if(playerY > monsterY)
    {
        deltaY++;
        if(gameBoard[monsterY + 1][monsterX] == SHALL_NOT_PASS && gameBoard[monsterY + 2][monsterX] != SHALL_NOT_PASS)
        {
            deltaY++;
        }
        else if(gameBoard[monsterY + 1][monsterX] == SHALL_NOT_PASS && gameBoard[monsterY + 2][monsterX] == SHALL_NOT_PASS)
        {
            deltaY--;
        }

    }
    //Player is Above Monster
    else if(playerY < monsterY)
    {
        deltaY--;
        if(gameBoard[monsterY - 1][monsterX] == SHALL_NOT_PASS && gameBoard[monsterY - 2][monsterX] != SHALL_NOT_PASS)
        {
            deltaY--;
        }
        else if(gameBoard[monsterY - 1][monsterX] == SHALL_NOT_PASS && gameBoard[monsterY - 2][monsterX] == SHALL_NOT_PASS)
        {
            deltaY++;
        }
    }
    monster.update_location(deltaX, deltaY);
}


/*
    What other functions do you need to make the game work?  What can you
    add to the basic functionality to make it more fun to play?
*/
