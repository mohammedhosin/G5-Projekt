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
    int PLAYER_POSITION_X;
    int PLAYER_POSITION_Y;
    int speed;
};

PUBLIC Player createPlayer(int x, int y)
{
    Player p = malloc(sizeof(struct Player_type));
    p->PLAYER_POSITION_Y = y;
    p->PLAYER_POSITION_X = x;
    p->speed = 1;
    return p;
}
PUBLIC void setY(Player p, int y){
    p->PLAYER_POSITION_Y = y;
}
PUBLIC void setX(Player p, int x){
    p->PLAYER_POSITION_X = x;
}

PUBLIC void setPlayerPositionY(Player p, int y){
    p->PLAYER_POSITION_Y = y;
}

PUBLIC int getPlayerPositionX(Player p){
    return p->PLAYER_POSITION_X;
}


PUBLIC int getPlayerPositionY(Player p){
    return p->PLAYER_POSITION_Y;
}

PUBLIC int getPlayerWidth(){
    return PLAYER_WIDTH;
}

PUBLIC int getPlayerHeight(){
    return PLAYER_HEIGTH;
}
