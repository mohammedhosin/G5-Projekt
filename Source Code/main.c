#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_net.h>
#include "Player.h"


const int WINDOW_WIDTH = 960, WINDOW_HEIGTH = 540;
bool init();
void renderBackground();
bool initPlayField();
bool initMedia();
int collisionDetectionXpos(int x_pos);
int collisionDetectionYpos(int y_pos);
int determineVelocityX(bool left, bool right, float startingSpeed);
int determineVelocityY(bool up, bool down, float startingSpeed);
void speedLimit(Player p);
void colissionDetectionPlayerArena(Player p);


SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface *imageSurface =  NULL;
SDL_Surface *sPlayer = NULL;
SDL_Surface *sGoal_Left = NULL;
SDL_Surface *sGoal_Right = NULL;
//SDL_Surface *windowSurface = NULL;
SDL_Texture *mField;
SDL_Texture *mPlayer = NULL;
SDL_Texture *mGoal_Left = NULL;
SDL_Texture *mGoal_Right = NULL;

Player player = NULL;
SDL_Rect gField;
SDL_Rect gGoal_Left;
SDL_Rect gGoal_Right;
// struct to hold the position and size of the sprite
SDL_Rect gPlayer;
#define SPEED (300); //75 is optimal, 300 for dev.
#define MAX_SPEED_REVERSE -1
#define MAX_SPEED_FORWARD 8
#define TURNING_SPEED 10
#define ACCELERATION 0.1
int main(int argc, char * argv[])
{
    /**
     Implement into player object?
     Not done yet.
     */
    bool running = true;
    if(init())
    {
        printf("Initialize window and renderer successful.\n");
    }
    //Init backround here
    if(!initPlayField())
    {
        SDL_FreeSurface(imageSurface);
        //SDL_FreeSurface(windowSurface);
        //windowSurface = NULL;
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize playfield successful.\n");
    }
    //Init other graphical media.
    if(!initMedia())
    {
        SDL_FreeSurface(imageSurface);
       //SDL_FreeSurface(windowSurface);
       //windowSurface = NULL;
        imageSurface = NULL;
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(mField);
        SDL_DestroyTexture(mPlayer);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }
    else
    {
        printf("Initialize media successful.\n");
    }
    /*  Taken from Jonas Will�n, SDL_net.zip*/
        

    // get and scale the dimensions of texture
    //SDL_QueryTexture(mPlayer, NULL, NULL, &gPlayer.w, &gPlayer.h);
    //gPlayer.w /= 10;
    //gPlayer.h /= 10;

    // start sprite in the the middle of the goal
    //float x_pos = (WINDOW_WIDTH - dest.w); //Starting x-pos for right team
/*
    float x_pos = (0); //Starting x-pos for left team
    float y_pos = (WINDOW_HEIGTH - gPlayer.h) / 2;
*/
    setPlayerPositionX(player, 0);
    setPlayerDirection(player, 90);
    setPlayerPositionY(player, (WINDOW_HEIGTH - gPlayer.h) / 2);
    float x_pos = getPlayerPositionX(player);
    float y_pos = getPlayerPositionY(player);

    // keep track of which inputs are given
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
        
    /*  End of SDL_net.zip code*/
    
    
    while(running)
    {
    /**
    While loop checking if an event occured.
     Code taken from Jonas Will�n, SDL_net.zip
     */
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = true;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = true;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = true;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = true;
                        break;
                    default:
                        break;
                }
                break;
                case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W:
                    case SDL_SCANCODE_UP:
                        up = false;
                        break;
                    case SDL_SCANCODE_A:
                    case SDL_SCANCODE_LEFT:
                        left = false;
                        break;
                    case SDL_SCANCODE_S:
                    case SDL_SCANCODE_DOWN:
                        down = false;
                        break;
                    case SDL_SCANCODE_D:
                    case SDL_SCANCODE_RIGHT:
                        right = false;
                        break;
                    default:
                        break;
                }
                break;
            }
        }
        //Update attributes of the struct
        if (up == true)
            changePlayerSpeed(player, ACCELERATION);
        if (down == true)
            changePlayerSpeed(player, -ACCELERATION);
        speedLimit(player);
        if (left == true)
            changePlayerDirection(player, TURNING_SPEED - getPlayerSpeed(player));      //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
        if (right == true)
            changePlayerDirection(player, -TURNING_SPEED + getPlayerSpeed(player));     //while it's fun to always turn fast, the game feels more realistic if you cant turn as fast on high speeds
        //Update position of the struct
        updatePlayerPosition(player, 1);
        colissionDetectionPlayerArena(player);
        printf("X:%f, Y:%f, speed:%f, direction:%f\n",getPlayerPositionX(player),getPlayerPositionY(player),getPlayerSpeed(player),getPlayerDirection(player));
        /*
        x_pos += (determineVelocityX(left, right) / 60));
        y_pos += (determineVelocityY(up, down) / 60));
         */
        // set the positions in the struct
        float x_pos = getPlayerPositionX(player);
        float y_pos = getPlayerPositionY(player);
        gPlayer.y = collisionDetectionYpos(y_pos);
        gPlayer.x = collisionDetectionXpos(x_pos);
        /*  end of SDL_net.zip*/

        SDL_RenderClear(renderer);
        renderBackground();
        SDL_RenderCopyEx(renderer, mPlayer, NULL, &gPlayer, -getPlayerDirection(player), NULL, SDL_FLIP_NONE);
        //SDL_RenderCopy(renderer, mPlayer, NULL, &gPlayer);
        SDL_RenderPresent(renderer);
        
        
        SDL_Delay(1000/50);
    }
    SDL_FreeSurface(imageSurface);
    //SDL_FreeSurface(windowSurface);
    //windowSurface = NULL;
    imageSurface = NULL;
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mField);
    SDL_DestroyTexture(mPlayer);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
void speedLimit(Player p)   //how fast do the cars go forward and in reverse? if a player goes beyond the cars limits, this sets the car back to it's limits
{
    if (getPlayerSpeed(p)>MAX_SPEED_FORWARD)
        setPlayerSpeed(p, MAX_SPEED_FORWARD);
    if (getPlayerSpeed(p)<MAX_SPEED_REVERSE)
        setPlayerSpeed(p, MAX_SPEED_REVERSE);
}
void colissionDetectionPlayerArena(Player p)    //keeping the abstract version of the car on the arena prevents you getting visually stuck in the corner while lost far off the map
{
    float slow = 0.96;                          //slow factor for colliding with arena walls helps changing direction when you messed up bad, and in theory slows down people who ram it but it's very neglible for that purpose
    if (getPlayerPositionX(p) < 0)
    {
        setPlayerPositionX(p, 0);
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionY(p) < 0)
    {
        setPlayerPositionY(p, 0);
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionX(p) > WINDOW_WIDTH - getPlayerHeight())
    {
        setPlayerPositionX(p, WINDOW_WIDTH - getPlayerWidth());
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
    if (getPlayerPositionY(p) > WINDOW_HEIGTH - getPlayerHeight())
    {
        setPlayerPositionY(p, WINDOW_HEIGTH - getPlayerHeight());
        setPlayerSpeed(player, getPlayerSpeed(player)*slow);
    }
}
/*
 Determines the velocity on y-axis.
 Code taken from Jonas Will�n, SDL_net.zip
 */
int determineVelocityY(bool up, bool down, float startingSpeed)
{
    if (up && !down)
    {
        startingSpeed =- SPEED;
    }
    if (down && !up)
    {
        startingSpeed =+ SPEED;
    }
    return startingSpeed;
}
/*
Determines the velocity on x-axis.
Code taken from Jonas Will�n, SDL_net.zip
*/
int determineVelocityX(bool left, bool right, float startingSpeed)
{
    if (left && !right)
    {
        startingSpeed =- SPEED;
    }
    if (right && !left)
    {
        startingSpeed =+ SPEED;
    }
    return startingSpeed;
}
/**
 Collisiondetection for moving object on X-axis. Makes sure that the objec stays within the window
 Code taken from Jonas Will�n, SDL_net.zip
 */

int collisionDetectionXpos(int x_pos)
{
    if (x_pos <= 0) x_pos = 0;
    if (x_pos >= WINDOW_WIDTH - getPlayerHeight()) x_pos = WINDOW_WIDTH - getPlayerWidth();
    return x_pos;
}
/**
Collisiondetection for moving object on Y-axis. Makes sure that the objec stays within the window
Code taken from Jonas Will�n, SDL_net.zip
*/
int collisionDetectionYpos(int y_pos)
{
    if (y_pos <= 0) y_pos = 0;
    if (y_pos >= WINDOW_HEIGTH - getPlayerHeight()) y_pos = WINDOW_HEIGTH - getPlayerWidth();
    return y_pos;
}
/**
 Init other media
 */
bool initMedia()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    sPlayer = IMG_Load("images/Player1.png");
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, sPlayer);
    player = createPlayer(0, 0);

    
    gPlayer.x = getPlayerPositionX(player);
    gPlayer.y = getPlayerPositionY(player);
    gPlayer.h = getPlayerHeight();
    gPlayer.w = getPlayerWidth();
     
    
    
    //gPlayer.x = 100; gPlayer.y = 100; gPlayer.h = 30; gPlayer.w = 30;
    if(NULL == imageSurface)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    return flag;
}
/**
 Init background football field.
 Returns false if image could not be loaded or if SDL_Image could not be initialized.
 */
bool initPlayField()
{
    bool flag = true;
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("\nCould not initialize SDL_Image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    imageSurface = IMG_Load("images/SoccerField.png");
    sGoal_Left = IMG_Load("images/Goal.png");
    sGoal_Right = IMG_Load("images/Goal.png");
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);
    mGoal_Left = SDL_CreateTextureFromSurface(renderer, sGoal_Left);
    mGoal_Right = SDL_CreateTextureFromSurface(renderer, sGoal_Right);
    //Left goal rect init position and define width and heigth
    gGoal_Left.h=370;
    gGoal_Left.w = 50;
    gGoal_Left.x = 0;
    gGoal_Left.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;
    
    //Right goal rect init position and define width and heigth
    gGoal_Right.h=370;
    gGoal_Right.w = 50;
    gGoal_Right.x = WINDOW_WIDTH-gGoal_Right.w;
    gGoal_Right.y = WINDOW_HEIGTH/2 - gGoal_Left.h/2;
    //Field Rect init positions and define width and heigth
    
    gField.x = 0; gField.y = 0; gField.h = WINDOW_HEIGTH; gField.w = WINDOW_WIDTH;
    if(NULL == imageSurface && NULL == sGoal_Left && NULL == sGoal_Right)
    {
        printf("\nCould not load image. Error: %s",SDL_GetError());
        printf("\n");
        flag = false;
    }
    return flag;
}
/**
 Rendering the background, AKA. Football field.
 */
void renderBackground()
{
    /*
    SDL_BlitSurface(imageSurface, NULL,windowSurface,NULL);
    SDL_UpdateWindowSurface(window);
     */
    SDL_RenderCopy(renderer, mField, NULL, &gField);
    SDL_RenderCopy(renderer,mGoal_Left, NULL, &gGoal_Left);
    SDL_RenderCopy(renderer, mGoal_Right, NULL, &gGoal_Right);
}

/**
 Init code taken from Jonas Will�n.
 Creates window and a renderer.
 Gets windowSurface from window to present background (play field).
 Returns false if init failed.
 */
bool init()
{
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Under production", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_SHOWN);
    
    //windowSurface = SDL_GetWindowSurface(window);
    
    if(window == NULL)
    {
        printf("Could not create window. Error: %s ",SDL_GetError());
        printf("\n");
        test = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL)
    {
        printf("Could not create renderer. Error: %s",SDL_GetError());
        printf("\n");
        test = false;
    }
    return test;
}