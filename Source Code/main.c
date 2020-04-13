#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

const int WIDTH = 960, HEIGTH = 540;
bool init();
void renderBackground();
bool initPlayField();
bool initMedia();


SDL_Window *window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface *imageSurface =  NULL;
SDL_Surface *player = NULL;
//SDL_Surface *windowSurface = NULL;
SDL_Texture *mField;
SDL_Texture *mPlayer = NULL;

SDL_Rect gField;
SDL_Rect gPlayer;
int main(int argc, const char * argv[])
{
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    /**
     Implement into player object?
     Not done yet
     */
    SDL_Rect possition;
    possition.y = 64;
    possition.x = 110;
    possition.h = 16;
    possition.w = 16;
    int frame = 6;
    
    
    SDL_Event e;
    bool running = true;
    if(init())
    {
        printf("Initialize successful.");
    }
    //Init backround here
    if(initPlayField())
    {
        printf("Initialize play field successful");
    }
    //Init other graphical media.
    if(initMedia())
    {
        printf("Initialize media successful");
    }
    while(running)
    {
        /**
        While loop checking if an event occured.
         */
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }

            else if( e.type == SDL_KEYDOWN )
            {
                //Select surfaces based on key press
                switch( e.key.keysym.sym )
                {
                    case SDLK_UP:
                        possition.y -= 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 4)
                            frame = 5;
                        else
                            frame = 4;
                        break;
                    case SDLK_DOWN:
                        possition.y += 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 0)
                            frame = 1;
                        else
                            frame = 0;
                        break;
                    case SDLK_LEFT:
                        possition.x -= 2;
                        flip = SDL_FLIP_HORIZONTAL;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    case SDLK_RIGHT:
                        possition.x += 2;
                        flip = SDL_FLIP_NONE;
                        if(frame == 2)
                            frame = 3;
                        else
                            frame = 2;
                        break;
                    default:
                        
                        break;
                }
            }
            
    }
       
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        renderBackground();
        SDL_RenderCopyEx(renderer, mPlayer, &gPlayer, &possition, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(imageSurface);
    //SDL_FreeSurface(windowSurface);
    //windowSurface = NULL;
    imageSurface = NULL;
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(mField);
    SDL_Quit();
    return 0;
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
    player = IMG_Load("images/Player1.png");
    
    mPlayer = SDL_CreateTextureFromSurface(renderer, player);
    gPlayer.x = 100; gPlayer.y = 100; gPlayer.h = 30; gPlayer.w = 30;
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
    gField.x = 0; gField.y = 0; gField.h = HEIGTH; gField.w = WIDTH;
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
    window = SDL_CreateWindow("Under production", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGTH, SDL_WINDOW_SHOWN);
    
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
