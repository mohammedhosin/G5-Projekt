#include <stdio.h>
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
int determineVelocityX(bool left, bool right);
int determineVelocityY(bool up, bool down);



SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface *imageSurface =  NULL;
SDL_Surface *sPlayer = NULL;
//SDL_Surface *windowSurface = NULL;
SDL_Texture *mField;
SDL_Texture *mPlayer = NULL;

Player player = NULL;
SDL_Rect gField;
// struct to hold the position and size of the sprite
SDL_Rect gPlayer;
#define SPEED (300); //75 is optimal, 300 for dev.
int main(int argc, const char * argv[])
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
    /*  Taken from Jonas Willén, SDL_net.zip*/
        

    // get and scale the dimensions of texture
    //SDL_QueryTexture(mPlayer, NULL, NULL, &gPlayer.w, &gPlayer.h);
    //gPlayer.w /= 10;
    //gPlayer.h /= 10;

    // start sprite in the the middle of the goal
    //float x_pos = (WINDOW_WIDTH - dest.w); //Starting x-pos for right team
    
    float x_pos = (0); //Starting x-pos for left team
    float y_pos = (WINDOW_HEIGTH - gPlayer.h) / 2;
    /*
    setPlayerPositionX(player, 0);
    setPlayerPositionY(player, (WINDOW_HEIGTH - gPlayer.h) / 2);
*/
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
     Code taken from Jonas Willén, SDL_net.zip
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
        //Update positions of the struct
        x_pos += (determineVelocityX(left, right) / 60);
        y_pos += (determineVelocityY(up, down) / 60);
       
        // set the positions in the struct
        gPlayer.y = collisionDetectionYpos(y_pos);
        gPlayer.x = collisionDetectionXpos(x_pos);
        /*  end of SDL_net.zip*/
        
        SDL_RenderClear(renderer);
        renderBackground();
        //SDL_RenderCopyEx(renderer, mPlayer, NULL, &dest, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, mPlayer, NULL, &gPlayer);
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
/*
 Determines the velocity on y-axis.
 Code taken from Jonas Willén, SDL_net.zip
 */
int determineVelocityY(bool up, bool down)
{
    int y_vel = 0;
    if (up && !down)
    {
        y_vel = -SPEED;
    }
    if (down && !up)
    {
        y_vel = SPEED;
    }
    return y_vel;
}
/*
Determines the velocity on x-axis.
Code taken from Jonas Willén, SDL_net.zip
*/
int determineVelocityX(bool left, bool right)
{
    int x_vel = 0;
    if (left && !right)
    {
        x_vel = -SPEED;
    }
    if (right && !left)
    {
        x_vel = SPEED;
    }
    return x_vel;
}
/**
 Collisiondetection for moving object on X-axis. Makes sure that the objec stays within the window
 Code taken from Jonas Willén, SDL_net.zip
 */
int collisionDetectionXpos(int x_pos)
{
    if (x_pos <= 0) x_pos = 0;
    if (x_pos >= WINDOW_WIDTH - gPlayer.w) x_pos = WINDOW_WIDTH - gPlayer.w;
    return x_pos;
}
/**
Collisiondetection for moving object on Y-axis. Makes sure that the objec stays within the window
Code taken from Jonas Willén, SDL_net.zip
*/
int collisionDetectionYpos(int y_pos)
{
    if (y_pos <= 0) y_pos = 0;
    if (y_pos >= WINDOW_HEIGTH - gPlayer.h) y_pos = WINDOW_HEIGTH - gPlayer.h;
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
    player = createPlayer(100, 100);

    
    gPlayer.x = getPlayerPositionX(player);
    gPlayer.y = getPlayerPositionY(player);
    gPlayer.h = getPlayerHeight();
    gPlayer.w =getPlayerWidth();
     
    
    
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
    
    mField = SDL_CreateTextureFromSurface(renderer, imageSurface);
    gField.x = 0; gField.y = 0; gField.h = WINDOW_HEIGTH; gField.w = WINDOW_WIDTH;
    if(NULL == imageSurface)
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
}

/**
 Init code taken from Jonas Willén.
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
