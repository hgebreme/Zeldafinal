/* Andrew Nemecek, 19 March 2017
 * SDL2 tutorial code */

//using SDL and standard IO
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] ){
    // the window we'll be rendering to
    SDL_Window* window = NULL;

    // the surface contained by the window
    SDL_Surface* screenSurface = NULL;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }else{
        // Create Window
        window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL ){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }else{
            // Get Window surface
            screenSurface = SDL_GetWindowSurface( window );

            // Fill the surface white
            SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
            
            // Update the surface
            SDL_UpdateWindowSurface( window );

            // Wait 2 seconds
            SDL_Delay( 2000 );
        }
    }        

    // Destroy Window
    SDL_DestroyWindow( window );

    // Quit SDL subsystems
    SDL_Quit();

    return 0;

}


/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
