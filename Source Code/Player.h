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
float getPlayerPositionX(Player p);
float getPlayerPositionY(Player p);
Player createPlayer(float x, float y);
void setPlayerPositionY(Player p, float y);
void setPlayerPositionX(Player p, float x);
int getPlayerWidth();
int getPlayerHeight();

#endif /* Player_h */
