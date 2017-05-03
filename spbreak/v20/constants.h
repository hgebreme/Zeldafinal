/* Contains all of the global variables for the program */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen Dimensions
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// counters
extern int movecount;
extern int endgamecount;

// Movement speed
extern const int SPEED;

// The window we'll be rendering to
extern SDL_Window* gWindow;

// the window renderer
extern SDL_Renderer* gRenderer;

// usable space for game (not hud)
extern SDL_Rect fullScreen;

// heart info
extern bool dropHeart;
extern SDL_Rect treasureSpace;


#endif
