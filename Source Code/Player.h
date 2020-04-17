//
//  Player.h
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright © 2020 Ernst. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <stdio.h>
typedef struct Player_type *Player;
int getPlayerPositionX(Player p);
int getPlayerPositionY(Player p);
Player createPlayer(int x, int y);
void setPlayerPositionY(Player p, int y);
void setY(Player p, int y);
void setX(Player p, int x);
int getPlayerWidth();
int getPlayerHeight();

#endif /* Player_h */
