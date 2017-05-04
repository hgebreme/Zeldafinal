/*Andrew Nemecek
 * Main program. Plays zelda */

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<stdio.h>
#include<string>
#include<time.h>
#include"character.h"
#include"link.h"
#include"enemy.h"
#include"boss.h"
#include"constants.h"
#include"ltexture.h"

using namespace std;

// Screen dimensions constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 5;

// counters
int movecount = 0; // determines when different moves happen
int endgamecount = 0; // determines how long the last room is shown before showing the endgame screen

// Determine which square link is in
int num = 0;
int prevnum = 0;

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Start menu
LTexture gStart; // texture for start menu
SDL_Rect entireScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

// Game over (loss)
LTexture gGameOver; // texture for game over
// uses entireScreen to expand to fit the screen

// Game over (win)
LTexture gWin; // texture for win image
SDL_Rect gWinSection; // size initialized in loadMedia()
// uses fullScreen to expand to fit the screen and leave the hud

// Character sprite
SDL_Rect linkSprites[12]; // link
SDL_Rect enemies[10]; // enemy sprites (size may need to be increased if more enemies are added

// Heart
LTexture gTreasureTexture; 	// texture for heart
SDL_Rect treasure[2]; // hearts (full and half)
SDL_Rect treasureSpace = {0, 0, 20, 20}; // heart location
bool dropHeart = false;

// Initialize Link class character
Link link(6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, "empty", 12, NULL, 1, 1, true, 0, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, 40, 40, 6);

// initialize enemies
Enemy e0(1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "empty", 10, NULL, 1, 1, true, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 40, 40, 1);
Enemy e1(1, 3*SCREEN_WIDTH / 4 - 10, 3*SCREEN_HEIGHT / 4 - 10, 3*SCREEN_WIDTH / 4 - 10, 3*SCREEN_HEIGHT / 4 - 10, "empty", 10, NULL, 1, 1, true, 0, 3*SCREEN_WIDTH / 4 - 10, 3*SCREEN_HEIGHT / 4 - 10, 40, 40, 1);
Enemy e2(1, SCREEN_WIDTH / 4, 3*SCREEN_HEIGHT / 4, SCREEN_WIDTH / 4, 3*SCREEN_HEIGHT / 4, "empty", 10, NULL, 1, 1, true, 0, SCREEN_WIDTH / 4, 3*SCREEN_HEIGHT / 4, 40, 40, 1);
Boss b(6, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, "empty", 10, NULL, 1, 1, false, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 60, 60, 6);

// Scene sprites
SDL_Rect gSpriteClips[4]; // select which region of map to load
LTexture gSpriteSheetTexture; // full map texture loaded from png image
SDL_Rect fullScreen = {0, SCREEN_HEIGHT / 4, SCREEN_WIDTH, 3 * SCREEN_HEIGHT / 4}; // allows stretching to fill full screen
SDL_Rect gDungeonClip[2]; // select which region of dungeon to load
LTexture gDungeonTexture; // full dungeon texture loaded from png image

// HUD info
SDL_Rect blackRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 4}; // black rectange that HUD is displayed on
SDL_Rect greyRect = {SCREEN_WIDTH / 16, SCREEN_HEIGHT / 32, SCREEN_WIDTH / 4, 3* SCREEN_HEIGHT / 16}; // grey rectangle representing entire map
SDL_Rect greenLocation = {SCREEN_WIDTH / 8, 5 * SCREEN_HEIGHT / 64, SCREEN_WIDTH / 128, SCREEN_HEIGHT / 128}; // green rectangle showing location on map
SDL_Rect inventory = {256 / 3, 10, 256 / 3, 224 / 4 - 10}; // shows number of rupees, etc.
LTexture gInvText; // texture for image that shows number of rupees, etc.
SDL_Rect invStretch = {SCREEN_WIDTH/3, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4}; // stretches inventory to correct size
SDL_Rect heart1 = {3*SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8, 30, 30}; // first heart in life meter
SDL_Rect heart2 = {3*SCREEN_WIDTH / 4 + 45, SCREEN_HEIGHT / 8, 30, 30}; // second heart in life meter
SDL_Rect heart3 = {3*SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT / 8, 30, 30}; // third heart in life meter
SDL_Rect lifeFont = {2*256/3, 10, 256/3, 224 / 8 - 10}; // "-LIFE-"
SDL_Rect lifeStretch = {2*SCREEN_WIDTH / 3 + 20, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8}; // stretches "-LIFE-" to correct size

// music that will be played
Mix_Music *gMusic1 = NULL; // intro
Mix_Music *gMusic2 = NULL; // game over win
Mix_Music *gMusic3 = NULL; // overworld

void reset(); // reset the game after a gameover

bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }else{
        // Set Texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1") ){
            printf("Warning: Linear texture filtering not enabled!");
        }        

        // Create window
        gWindow = SDL_CreateWindow( "The Legend of Zelda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL ){
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }else{
            // Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL){
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
                success = false;
            }else{
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init(imgFlags) & imgFlags ) ){
                    printf("SDL_image could not initialize! SDL_image_Error: %s\n", IMG_GetError() );
                    success = false;
                }

                // Initialize SDL_mixer
                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0){
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}        

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load music
    if(Mix_VolumeMusic(-1) == 0){
        printf("error \n");
        success = false;
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME);

    gMusic1 = Mix_LoadMUS("intro.wav"); // load title screen song
    if(gMusic1 == NULL){
        printf("Failed to load title music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    
    gMusic2 = Mix_LoadMUS("fairyfountain.wav"); // load end game (win) song
    if(gMusic2 == NULL){
        printf("Failed to load end music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    gMusic3 = Mix_LoadMUS("overworld.wav"); // load main song
    if(gMusic3 == NULL){
        printf("Failed to load overworld music1 SDL_Eror: %s\n", Mix_GetError());
        success = false;
    }

    // Load start screen texture
    if( !gStart.loadFromFile("LOZHeader.png")){
        printf("Failed to load start screen image\n");
        success = false;
    }

    // Load game over texture
    if( !gGameOver.loadFromFile("maxresdefault.png")){
        printf("Failed to load game over (loss) image\n");
        success = false;
    }

    // Load win texture
    if( !gWin.loadFromFile("zelda+17.png")){
        printf("Failed to load win image\n");
        success = false;
    }else{
        gWinSection.w = gWin.getWidth();
        gWinSection.h = 3*gWin.getHeight() / 4;
        gWinSection.x = 0;
        gWinSection.y = gWin.getHeight() / 4;
    }

    // Load character texture
    if( !link.setLTexture("zelda-sprites-link.png")){
        printf("Failed to load Link texture image!\n");
        success = false;
    }else{    
        // walking down 1
        linkSprites[0].w = 20;
        linkSprites[0].h = 20;
        linkSprites[0].x = 0;
        linkSprites[0].y = 0;

        // walking down 2
        linkSprites[1].w = 20;
        linkSprites[1].h = 20;
        linkSprites[1].x = 0;
        linkSprites[1].y = 30;

        // walking up 1
        linkSprites[2].w = 20;
        linkSprites[2].h = 20;
        linkSprites[2].x = 60;
        linkSprites[2].y = 0;

        // walking up 2
        linkSprites[3].w = 20;
        linkSprites[3].h = 20;
        linkSprites[3].x = 60;
        linkSprites[3].y = 30;

        // walking left 1
        linkSprites[4].w = 20;
        linkSprites[4].h = 20;
        linkSprites[4].x = 30;
        linkSprites[4].y = 0;

        // walking left 2
        linkSprites[5].w = 20;
        linkSprites[5].h = 20;
        linkSprites[5].x = 30;
        linkSprites[5].y = 30;

        // walking right 1
        linkSprites[6].w = 20;
        linkSprites[6].h = 20;
        linkSprites[6].x = 90;
        linkSprites[6].y = 0;

        // walking right 2
        linkSprites[7].w = 20;
        linkSprites[7].h = 20;
        linkSprites[7].x = 90;
        linkSprites[7].y = 30;

        // attack down
        linkSprites[8].w = 20;
        linkSprites[8].h = 32;
        linkSprites[8].x = 0;
        linkSprites[8].y = 80;

        // attack up
        linkSprites[9].w = 20;
        linkSprites[9].h = 32;
        linkSprites[9].x = 60;
        linkSprites[9].y = 80;

        // attack left
        linkSprites[10].w = 32;
        linkSprites[10].h = 20;
        linkSprites[10].x = 22;
        linkSprites[10].y = 90;

        // attack right
        linkSprites[11].w = 32;
        linkSprites[11].h = 20;
        linkSprites[11].x = 82;
        linkSprites[11].y = 90;


        link.setSprites(linkSprites); // set link's sprites
    }

    // Load treasure (red heart)
    if( !gTreasureTexture.loadFromFile( "zelda-sprites-treasures.png") ) {
	printf("Failed to load treasure image\n");
	success = false;
    }else{
        // full heart
        treasure[0].w = 7;
        treasure[0].h = 7;
        treasure[0].x = 0;
        treasure[0].y = 0;

        // half heart
        treasure[1].w = 7;
        treasure[1].h = 7;
        treasure[1].x = 8;
        treasure[1].y = 0;
    }

    // Load map image
    if( !gSpriteSheetTexture.loadFromFile( "zelda-overworld(map).png") ){
        printf("Failed to load map image\n");
        success = false;
    }else{
        // Set Top left sprite
        gSpriteClips[0].x = 2304;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 256;
        gSpriteClips[0].h = 176;

        // Set Top right sprite
        gSpriteClips[1].x = 768;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 256;
        gSpriteClips[1].h = 176;

        // Set bottom left sprite
        gSpriteClips[2].x = 0;
        gSpriteClips[2].y = 1232;
        gSpriteClips[2].w = 256;
        gSpriteClips[2].h = 176;

        // Set bottom right sprite
        gSpriteClips[3].x = 256;
        gSpriteClips[3].y = 176;
        gSpriteClips[3].w = 256;
        gSpriteClips[3].h = 176;
    }

    // Load hud image
    if( !gInvText.loadFromFile( "hud.png") ){
        printf("Failed to load hud image\n");
        success = false;
    }

    // Load dungeon image
    if( !gDungeonTexture.loadFromFile("zelda-dungeons(map).png") ){
        printf("Failed to load dungeon image\n");
        success = false;
    }else{
        gDungeonClip[0].x = 2304;
        gDungeonClip[0].y = 2112;
        gDungeonClip[0].w = 256;
        gDungeonClip[0].h = 176;
        
        gDungeonClip[1].x = 2560;
        gDungeonClip[1].y = 1936;
        gDungeonClip[1].w = 256;
        gDungeonClip[1].h = 176;
    }

    // Load enemy sprites
    if( !e0.setLTexture("zelda-sprites-enemies.png") || !e1.setLTexture("zelda-sprites-enemies.png") || !e2.setLTexture("zelda-sprites-enemies.png") || !b.setLTexture("zelda-sprites-enemies.png") ){
        printf("Failed to load enemy sprites\n");
        success = false;
    }else{
        // normal enemy, down
        enemies[0].x = 0;
        enemies[0].y = 0;
        enemies[0].w = 20;
        enemies[0].h = 20;

        // normal enemy, left
        enemies[1].x = 29;
        enemies[1].y = 0;
        enemies[1].w = 20;
        enemies[1].h = 20;        

        // normal enemy, up
        enemies[2].x = 58;
        enemies[2].y = 0;
        enemies[2].w = 20;
        enemies[2].h = 20;

        // normal enemy, right
        enemies[3].x = 87;
        enemies[3].y = 0;
        enemies[3].w = 20;
        enemies[3].h = 20;

        // boss 1
        enemies[4].x = 328;
        enemies[4].y = 181;
        enemies[4].w = 20;
        enemies[4].h = 15;

        // boss 2
        enemies[5].x = 328;
        enemies[5].y = 211;
        enemies[5].w = 20;
        enemies[5].h = 15;

        // blue enemy, down
        enemies[6].x = 116;
        enemies[6].y = 0;
        enemies[6].w = 20;
        enemies[6].h = 20;

        // blue enemy, left
        enemies[7].x = 145;
        enemies[7].y = 0;
        enemies[7].w = 20;
        enemies[7].h = 20;

        // blue enemy, up
        enemies[8].x = 174;
        enemies[8].y = 0;
        enemies[8].w = 20;
        enemies[8].h = 20;

        // blue enemy, right
        enemies[9].x = 203;
        enemies[9].y = 0;
        enemies[9].w = 20;
        enemies[9].h = 20;

        // set the enemy and boss sprites
        e0.setSprites(enemies);
        e1.setSprites(enemies);
        e2.setSprites(enemies);
        b.setSprites(enemies);
    }

    return success;
}

void close(){
    // Free Loaded images
    link.getLTexture().free();
    e0.getLTexture().free();
    e1.getLTexture().free();
    e2.getLTexture().free();
    b.getLTexture().free();
    gTreasureTexture.free();
    gInvText.free();
    gDungeonTexture.free();
    gStart.free();

    // Free music
    Mix_FreeMusic(gMusic1);
    Mix_FreeMusic(gMusic2);
    Mix_FreeMusic(gMusic3);
    gMusic1 = NULL;
    gMusic2 = NULL;
    gMusic3 = NULL;
        
    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool checkBounds(Character* thechar, SDL_Rect fullScreen, int &num, bool link){
                // Check if in bounds
                switch(num){
                    case 0: // top left overworld room (start room)
                        if(thechar->getYPos() < fullScreen.h / 6 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() < fullScreen.w / 16){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getYPos() > SCREEN_HEIGHT - thechar->getStretch().h - 2*fullScreen.h / 11 + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() < fullScreen.w / 8 && thechar->getYPos() < 4*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos()); 
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 3*fullScreen.w/16 && thechar->getYPos() < 3*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < fullScreen.w / 8 && thechar->getYPos() > 7*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 3*fullScreen.w/16 && thechar->getYPos() > 8*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos()); 
                        }else if(thechar->getXPos() > 12*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 3*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 12*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() > 8*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 15*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 5*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 15*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() > 6*fullScreen.h/11 + fullScreen.y - (thechar->getStretch().h - 10)){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > fullScreen.w - thechar->getStretch().w && link){
                            num = 1;
                            thechar->setXPos(0);
                        }else if(thechar->getXPos() + thechar->getStretch().w > fullScreen.w && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }
                        break;
                    case 1: // top right overworld room
                        if(thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() > 11*fullScreen.w/16 - thechar->getStretch().w + 10){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 5*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getXPos() < 10*fullScreen.w/16 && thechar->getYPos() > 6*fullScreen.h/11 + fullScreen.y - (thechar->getStretch().h - 10)){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 5*fullScreen.w/16 -thechar->getStretch().w + 10 && thechar->getYPos() < 5*fullScreen.h/11 + fullScreen.y - 10 ){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 5*fullScreen.w/16 && thechar->getYPos() > 9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < fullScreen.w/16 && thechar->getYPos() < 5*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 3*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 3*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 4*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 4*fullScreen.h / 11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos()); 
                        }else if(thechar->getXPos() > 3*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getXPos() < fullScreen.w/2 && thechar->getYPos() > 9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 4*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getXPos() < fullScreen.w/2 && thechar->getYPos() > 8*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 0 && link){
                            num = 0;
                            thechar->setXPos(SCREEN_WIDTH - thechar->getStretch().w);
                            thechar->setYPos(5*fullScreen.h/11 + fullScreen.y);
                        }else if(thechar->getXPos() < 0 && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getYPos() > SCREEN_HEIGHT - thechar->getStretch().h && link){
                            num = 3;
                            thechar->setXPos(7*fullScreen.w/16);
                            thechar->setYPos(fullScreen.y);
                        }else if(thechar->getYPos() > SCREEN_HEIGHT - thechar->getStretch().h && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }
                        break;
                    case 3: // bottom right overworld room
                        if(thechar->getYPos() > 9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() > 15*fullScreen.w/16 - thechar->getStretch().w + 10){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 7*fullScreen.w/16 && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 8*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 14*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 3*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 14*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() > 8*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getYPos() < fullScreen.y && link){
                            num = 1;
                            thechar->setYPos(SCREEN_HEIGHT - thechar->getStretch().w);
                            thechar->setXPos(10*fullScreen.w/16);
                        }else if(thechar->getYPos() < fullScreen.y && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 0 && link){
                            num = 2;
                            thechar->setXPos(SCREEN_WIDTH - thechar->getStretch().w);
                        }else if(thechar->getXPos() < 0 && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }
                        break;
                    case 2: // bottom left overworld room
                        if(thechar->getXPos() < fullScreen.w/16){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getYPos() > 9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() < 4*fullScreen.w/16 && thechar->getYPos() < 3*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 3*fullScreen.w/16 && thechar->getYPos() < 4*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 2*fullScreen.w/16 && thechar->getYPos() < 5*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 2*fullScreen.w/16 && thechar->getYPos() > 6*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 3*fullScreen.w/16 && thechar->getYPos() > 7*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 4*fullScreen.w/16 && thechar->getYPos() > 8*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() < 11*fullScreen.w/16 && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 12*fullScreen.w/16 - thechar->getStretch().w + 10 && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > fullScreen.w - thechar->getStretch().w && link){
                            num = 3;
                            thechar->setXPos(0);
                        }else if(thechar->getXPos() > fullScreen.w - thechar->getStretch().w && !link){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 11*fullScreen.w/16 - 15 && thechar->getXPos() < 12*fullScreen.w/16 - thechar->getStretch().w + 15 && thechar->getYPos() < 3*fullScreen.h/22 + fullScreen.y && link){
                            num = 4;
                            thechar->setXPos((fullScreen.w - thechar->getStretch().w) / 2);
                            thechar->setYPos(9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h);
                        }else if(thechar->getYPos() < fullScreen.y){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }
                        break;
                    case 4: //  boss room
                        if(thechar->getXPos() < 2*fullScreen.w/16){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 14*fullScreen.w/16 - thechar->getStretch().w + 10){
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getYPos() > 9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h + 10){
                            thechar->setYPos(thechar->getPrevYPos());
                        }else if(thechar->getXPos() < 7*fullScreen.w/16 && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                            thechar->setYPos(thechar->getPrevYPos());
                            thechar->setXPos(thechar->getPrevXPos());
                        }else if(thechar->getXPos() > 9*fullScreen.w/16 - thechar->getStretch().w && thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y - 10){
                                thechar->setYPos(thechar->getPrevYPos());
                                thechar->setXPos(thechar->getPrevXPos());
                        }

                        if(b.getAlive()){
                            if(thechar->getYPos() < 2*fullScreen.h/11 + fullScreen.y){
                                thechar->setYPos(thechar->getPrevYPos());
                                thechar->setXPos(thechar->getPrevXPos());
                            }
                        }else{
                             if(thechar->getYPos() < 3*fullScreen.h/22 + fullScreen.y - 10 && link){
                                num = 5;
                                thechar->setXPos((fullScreen.w - thechar->getStretch().w) / 2);
                                thechar->setYPos(9*fullScreen.h/11 + fullScreen.y - thechar->getStretch().h);
                            }
                        }
                        break;
                }
}

int main( int argc, char* args[] ){
    // Seed rand
    srand(time(NULL));

    // Start up SDL and create window
    if( !init() ){
        printf("Failed to initialize!\n");
    }else{
        // Load media
        if( !loadMedia() ){
            printf("Failed to load media!\n");
        }else{
            // Main loop flag
            bool quit = false;

            // start menu flag
            bool start = true;

            // Event handler
            SDL_Event e;

	    // position of red heart
	    int heartx = SCREEN_WIDTH / 2 + 40;
            int hearty = SCREEN_HEIGHT / 2 - 10;

            // Current rendered texture
            LTexture* currentTexture = NULL;

            // While application is running
            while( !quit ){

                // Start menu (runs until button pressed
                if(start){
                    // Play title music
                    if(Mix_PlayingMusic() == 0){
                        Mix_PlayMusic(gMusic1, -1);
                    }
                    while(e.type != SDL_KEYDOWN){ // wait for keypress
                        SDL_RenderCopy(gRenderer, gStart.texture(), NULL, &entireScreen);
                        SDL_RenderPresent(gRenderer);
                        SDL_PollEvent(&e);
                        if(e.type == SDL_QUIT){
                            quit = true;
                            break;
                        }
                    }
                    start = false; // leave start screen
                    Mix_HaltMusic(); // stop music
                }

                // Play music
                if(num < 5){
                    if(Mix_PlayingMusic() == 0){
                        Mix_PlayMusic(gMusic3, -1);
                    }
                }else if(num == 5 && prevnum != 5){
                    Mix_HaltMusic();
                }else if(num == 5 ){ //&& prevnum == 5){
                    if(Mix_PlayingMusic() == 0){
                        Mix_PlayMusic(gMusic2, -1);
                  }
                }


                // Handle events on queue
                while(SDL_PollEvent( &e ) != 0){
                    // User requests quit
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }else if(e.type == SDL_KEYDOWN){
                        // User presses 'A' for attack
                        if(e.key.keysym.scancode == SDL_SCANCODE_A){
                            link.attack();
                        }
                    }
                }

                // set value of previous frame;
                prevnum = num;

                // move link, change sprite for animation
                link.setPrevXPos(link.getXPos());
                link.setPrevYPos(link.getYPos());
                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                link.move(currentKeyStates);
                checkBounds(&link, fullScreen, num, true);

                // if link moves to a new sceen, respawn enemies and place them in the correct place
                if(prevnum != num){
                    dropHeart = false; // do not carry over heart drops to the next room
                    switch(num){
                        case 0:
                        case 2:
                        case 3:
                            e0.setAlive(true);
                            e1.setAlive(true);
                            e2.setAlive(true);
                            b.setAlive(false);
                            e0.setHealth(e0.getMaxHealth());
                            e1.setHealth(e1.getMaxHealth());
                            e2.setHealth(e2.getMaxHealth());
                            e0.setXPos(SCREEN_WIDTH / 2 - 20);
                            e0.setYPos(SCREEN_HEIGHT / 2 - 20);
                            e1.setXPos(3*SCREEN_HEIGHT / 4 - 10);
                            e1.setYPos(3*SCREEN_HEIGHT/ 4 - 10);
                            e2.setXPos(SCREEN_WIDTH / 4);
                            e2.setYPos(3*SCREEN_HEIGHT / 4);
                            break;
                        case 1:
                            e0.setAlive(true);
                            e0.setHealth(e0.getMaxHealth());
                            e1.setAlive(false);
                            e2.setAlive(false);
                            b.setAlive(false);
                            e0.setXPos(SCREEN_WIDTH / 6 - 20);
                            e0.setYPos(SCREEN_HEIGHT / 2 - 20);
                            break;
                        case 4:
                            b.setAlive(true);
                            b.setHealth(b.getMaxHealth());
                            e0.setAlive(false);
                            e1.setAlive(false);
                            e2.setAlive(false);
                            b.setXPos(SCREEN_WIDTH / 2 - 30);
                            b.setYPos(SCREEN_HEIGHT / 2 - 30);
                            break;
                        default:
                            b.setAlive(false);
                            e0.setAlive(false);
                            e1.setAlive(false);
                            e2.setAlive(false);
                            break;            
                    }
                }

                // move enemies
                if(e0.getAlive()){
                    e0.move(currentKeyStates);
                }
                if(e1.getAlive()){
                    e1.move(currentKeyStates);
                }
                if(e2.getAlive()){
                    e2.move(currentKeyStates);
                }
                if(b.getAlive()){
                    b.move(currentKeyStates);
                }   

                checkBounds(&e0, fullScreen, num, false);
                checkBounds(&e1, fullScreen, num, false);
                checkBounds(&e2, fullScreen, num, false);
                checkBounds(&b, fullScreen, num, false);

                // have enemies attack (or not, it depends on movecount)
                if(movecount % 50 > 45){
                    if(num < 4){
                        if(e0.getAlive()){
                            e0.attack();
                        }
                        if(e1.getAlive()){
                            e1.attack();
                        }
                        if(e2.getAlive()){
                            e2.attack();
                        }
                    }
                }
                if(movecount % 50 > 25){
                    if(num == 4 && b.getAlive()){
                        if(movecount % 50 == 26){
                            b.setAttack1XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8);
                            b.setAttack1YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11);
                            b.setAttack2XPos((rand() % 4*fullScreen.w/5) + fullScreen.w / 8);
                            b.setAttack2YPos((rand() % 3*fullScreen.h/5) + fullScreen.y + 2*fullScreen.h/11);
                        }
                        b.attack();
                    }
                }


                // set render locations for characters
                // link
                link.setStretchX(link.getXPos());
                link.setStretchY(link.getYPos());

                // enemies 0-2
                e0.setStretchX(e0.getXPos());
                e0.setStretchY(e0.getYPos());                
                e1.setStretchX(e1.getXPos());
                e1.setStretchY(e1.getYPos()); 
                e2.setStretchX(e2.getXPos());
                e2.setStretchY(e2.getYPos()); 

                // boss
                b.setStretchX(b.getXPos());
                b.setStretchY(b.getYPos());                 

                // Clear Screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

               if(link.getAlive()){ 
                     // Render current texture
                     if(num == 0){ // starting zone
                         SDL_RenderCopyEx(gRenderer, gSpriteSheetTexture.texture(), &gSpriteClips[num], &fullScreen, 0, NULL, SDL_FLIP_HORIZONTAL); // flip image for map area 0
                     }else if(num >=1 && num < 4){ // other overworld locations
                         SDL_RenderCopy(gRenderer, gSpriteSheetTexture.texture(), &gSpriteClips[num], &fullScreen);
                     }else{ // dungeon
                         SDL_RenderCopy(gRenderer, gDungeonTexture.texture(), &gDungeonClip[num-4], &fullScreen);
                     }

                     // Render link
                     link.renderChar(gRenderer);

                     // Render treasure
                     if(dropHeart){
                         SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &treasureSpace);
                     }

                     // Render Enemies
                     if(e0.getAlive()){
                         e0.renderChar(gRenderer);
                     }
                     if(e1.getAlive()){
                         e1.renderChar(gRenderer);
                     }
                     if(e2.getAlive()){
                         e2.renderChar(gRenderer);
                     }

                     // Render boss
                     if(b.getAlive()){
                         if(movecount % 2 == 0){
                             b.setFrame(4);
                             b.renderChar(gRenderer);
                         }else{
                             b.setFrame(5);
                             b.renderChar(gRenderer);
                         }
                     }

                     // Render MiniMap
                     SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF); // background
                     SDL_RenderFillRect(gRenderer, &blackRect); // background

                     if(num < 4){ // if in overworld, render minimap
                         SDL_SetRenderDrawColor(gRenderer, 156, 156, 156, 0xFF);
                         SDL_RenderFillRect(gRenderer, &greyRect);
                         SDL_SetRenderDrawColor(gRenderer, 46, 206, 46, 0xFF);
                         SDL_RenderFillRect(gRenderer, &greenLocation);
                     }else{ // if in boss room or final room, render boss health bar
                         SDL_SetRenderDrawColor(gRenderer, 156, 156, 156, 0xFF);
                         SDL_Rect maxHealth = {SCREEN_WIDTH / 24, SCREEN_HEIGHT / 12, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 24};
                         SDL_RenderFillRect(gRenderer, &maxHealth);
                         SDL_SetRenderDrawColor(gRenderer, 170, 41, 23, 0xFF);
                         SDL_Rect currHealth = {SCREEN_WIDTH / 24, SCREEN_HEIGHT / 12, b.getHealth()*maxHealth.w/6, SCREEN_HEIGHT/24};
                         SDL_RenderFillRect(gRenderer, &currHealth);
                     }
                     
                         

                     // Render Inventory
                     SDL_RenderCopy(gRenderer, gInvText.texture(), &inventory, &invStretch);

                     // Render Life
                     switch(link.getHealth()){
                         case 1: // half heart
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[1], &heart1);
                             break;
                         case 2: // full heart
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                             break;
                         case 3: // half and full heart
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[1], &heart2);
                             break;
                         case 4: // two full hearts
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart2);
                             break;
                         case 5: // half heart and two full hearts
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart2);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[1], &heart3);
                             break;
                         default: // three full hearts
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart2);
                             SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart3);
                             break;

                     }
                     SDL_RenderCopy(gRenderer, gInvText.texture(), &lifeFont, &lifeStretch);

                     // Render door in boss room until boss is dead
                     if(num == 4 && b.getAlive()){
                         SDL_SetRenderDrawColor(gRenderer, 115, 115, 115, 0xFF);
                         SDL_Rect door = {15*fullScreen.w/32 - 1, fullScreen.h/11 + fullScreen.y + 3, fullScreen.w/16 + 2, fullScreen.h/11 - 4};
                         SDL_RenderFillRect(gRenderer, &door);
                     }

                    // render game over (win) image
                    if(num == 5 && endgamecount > 30){
                        SDL_RenderCopy(gRenderer, gWin.texture(), &gWinSection, &fullScreen);
                        if(e.type == SDL_KEYDOWN && endgamecount > 60){
                            reset();
                        }
                    }
            
                    // increment in game count
                    if(num == 5){
                        endgamecount++;
                    }

                }else{ // if link is dead
                    endgamecount++; // increment endgamecount
                    SDL_RenderCopy(gRenderer, gGameOver.texture(), NULL, &entireScreen);
                    if(e.type == SDL_KEYDOWN && endgamecount > 30){
                        reset();
                    }
                }

                // Update screen
                SDL_RenderPresent(gRenderer);

                // if attacking, return to previous sprite
                if(link.getFrame() > 7 && e.type == SDL_KEYUP){
                    link.setFrame(link.getPrevFrame());
                }

                // Update location of greenLocation rectangle
                float xratio = float(link.getXPos()) / SCREEN_WIDTH;
                float yratio = float(link.getYPos() - SCREEN_HEIGHT / 4) / (3*SCREEN_HEIGHT / 4);
                greenLocation.x = SCREEN_WIDTH / 16 + xratio * SCREEN_WIDTH / 8;
                greenLocation.y = SCREEN_HEIGHT / 32 + yratio * 3 * SCREEN_HEIGHT / 32;
                switch(num){
                    case 1:
                        greenLocation.x += SCREEN_WIDTH / 8;
                        break;
                    case 2:
                        greenLocation.y += 3*SCREEN_HEIGHT / 32;
                        break;
                    case 3:
                        greenLocation.x += SCREEN_WIDTH / 8;
                        greenLocation.y += 3*SCREEN_HEIGHT / 32;
                        break;
                }
                
                // Update Link InvinceTime
                if(link.getInvinceTime() > 0){
                    link.setInvinceTime(link.getInvinceTime() - 1);
                }

                // increment movecount
                movecount++;
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}

void reset(){
    // reset link
    link.setXPos(SCREEN_WIDTH / 4);
    link.setYPos(SCREEN_HEIGHT / 2);
    link.setHealth(link.getMaxHealth());
    link.setAlive(true);

    // reset enemies
    e0.setAlive(true);
    e1.setAlive(true);
    e2.setAlive(true);
    b.setAlive(false);
    e0.setHealth(e0.getMaxHealth());
    e1.setHealth(e1.getMaxHealth());
    e2.setHealth(e2.getMaxHealth());
    e0.setXPos(SCREEN_WIDTH / 2 - 20);
    e0.setYPos(SCREEN_HEIGHT / 2 - 20);
    e1.setXPos(3*SCREEN_HEIGHT / 4 - 10);
    e1.setYPos(3*SCREEN_HEIGHT/ 4 - 10);
    e2.setXPos(SCREEN_WIDTH / 4);
    e2.setYPos(3*SCREEN_HEIGHT / 4);

    // reset screen that is rendered
    num = 0;

    // halt the current music
    if(Mix_PlayingMusic() == 1){
        Mix_HaltMusic();
    }

    // reset counters
    movecount = 0;
    endgamecount = 0;
}


/* vim: set sts=4 sw=5 ts=8 expandtab ft=c: */
 
 
