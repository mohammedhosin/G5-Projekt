//
//  player.c
//  G5_Projekt
//
//  Created by Ernst on 2020-04-17.
//  Copyright © 2020 Ernst. All rights reserved.
//
#include <stdlib.h>
#include "Player.h"

#define PUBLIC
#define PRIVATE static

PRIVATE int PLAYER_WIDTH = 30;
PRIVATE int PLAYER_HEIGTH = 30;

struct Player_type
{
    float PLAYER_POSITION_X;
    float PLAYER_POSITION_Y;
    int speed;
};

PUBLIC Player createPlayer(float x, float y)
{
    Player p = malloc(sizeof(struct Player_type));
    p->PLAYER_POSITION_Y = y;
    p->PLAYER_POSITION_X = x;
    p->speed = 1;
    return p;
}
PUBLIC void setPlayerPositionX(Player p, float x){
    p->PLAYER_POSITION_X = x;
}

PUBLIC void setPlayerPositionY(Player p, float y){
    p->PLAYER_POSITION_Y = y;
}

PUBLIC float getPlayerPositionX(Player p){
    return p->PLAYER_POSITION_X;
}


PUBLIC float getPlayerPositionY(Player p){
    return p->PLAYER_POSITION_Y;
}

PUBLIC int getPlayerWidth(){
    return PLAYER_WIDTH;
}

PUBLIC int getPlayerHeight(){
    return PLAYER_HEIGTH;
}
