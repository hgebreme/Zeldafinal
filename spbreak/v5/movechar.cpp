/*Andrew Nemecek
 * SDL2 tutorial 2 */

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<string>
using namespace std;

// Screen dimensions constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Texture wrapper class
class LTexture{
    public:
        // Initializes variables
        LTexture();

        // Deallocates memory
        ~LTexture();

        // Loads image at specified path
        bool loadFromFile(string path);

        // Deallocates texture
        void free();

        // Renders texture at given point
        void render(int x, int y, SDL_Rect* clip = NULL);

        // Gets image dimensions
        int getWidth();
        int getHeight();

        // Return texture
        SDL_Texture* texture();

    private:
        // The actual hardware texture
        SDL_Texture* mTexture;

        // Image dimensions
        int mWidth;
        int mHeight;
};

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

// Character sprite
SDL_Rect link;
LTexture gFooTexture;

// Scene sprites
SDL_Rect gSpriteClips[128];
LTexture gSpriteSheetTexture;
SDL_Rect fullScreen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // allows stretching to fill full screen

LTexture::LTexture(){
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture(){
    //Deallocate
    free();
}

bool LTexture::loadFromFile(string path){
    //Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ){
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }else{
        // Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL ){
            printf("Unable to create texture from %s! SDL_Error: %s\n", path.c_str(), SDL_GetError() );
        }else{
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

void LTexture::free(){
    // Free texture if it exists
    if(mTexture != NULL){
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect* clip){
    // Set rendering space and render to screen
    SDL_Rect renderTile = {x, y, mWidth, mHeight };

    // Set clip rendering dimensions
    if(clip != NULL){
        renderTile.w = clip->w;
        renderTile.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderTile);
}

int LTexture::getWidth(){
    return mWidth;
}

int LTexture::getHeight(){
    return mHeight;
}

SDL_Texture* LTexture::texture(){
    return mTexture;
}

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
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
            }
        }
    }

    return success;
}        

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load character texture
    if( !gFooTexture.loadFromFile("zelda-sprites-link.png")){
        printf("Failed to load Foo' texture image!\n");
        success = false;
    }else{
        // change values to get good sprite
        link.w = 20;
        link.h = 20;
        link.x = 0;
        link.y = 0;
    }

    // Load map image
    if( !gSpriteSheetTexture.loadFromFile( "zelda-overworld.png") ){
        printf("Failed to load map image\n");
        success = false;
    }else{
        // must edit for appropriate pixels and add loops to get full image
        // Set Top left sprite
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 100;
        gSpriteClips[0].h = 100;

        // Set Top right sprite
        gSpriteClips[1].x = 100;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 100;
        gSpriteClips[1].h = 100;

        // Set bottom left sprite
        gSpriteClips[1].x = 0;
        gSpriteClips[1].y = 100;
        gSpriteClips[1].w = 100;
        gSpriteClips[1].h = 100;

        // Set bottom right sprite
        gSpriteClips[2].x = 100;
        gSpriteClips[2].y = 100;
        gSpriteClips[2].w = 100;
        gSpriteClips[2].h = 100;
    }

    return success;
}

void close(){
    // Free Loaded images
    gSpriteSheetTexture.free();
    gFooTexture.free();

    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] ){
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

            // Event handler
            SDL_Event e;

            // Character position locations
            int xpos = SCREEN_WIDTH / 2;
            int ypos = SCREEN_HEIGHT / 2;

            // Current rendered texture
            LTexture* currentTexture = NULL;

            // While application is running
            while( !quit ){
                // Handle events on queue
                while(SDL_PollEvent( &e ) != 0){
                    // User requests quit
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }
                }

                // Set texture besed on current keystate
                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                if(currentKeyStates[SDL_SCANCODE_UP]){
                    ypos--; // move up
                }else if(currentKeyStates[SDL_SCANCODE_DOWN]){
                    ypos++; // move down
                }else if(currentKeyStates[SDL_SCANCODE_LEFT]){
                    xpos--; // move left
                }else if(currentKeyStates[SDL_SCANCODE_RIGHT]){
                    xpos++; // move right;
                }

                // Determine which square character is in
                int num = 0;
                if(xpos >= SCREEN_WIDTH - link.w){
                    switch(num){
                        case 0:
                            num = 1;
                            xpos = link.w; // move back to left side, might need to be set to 0
                            break;
                        case 1:
                            xpos--; // if in quadrant 1, it cannot move to the right so move back so it is still on the screen
                            break;
                        case 2:
                            num = 3;
                            xpos = link.w;
                            break;
                        case 3:
                            xpos--; // cannot move to the right anymore
                            break;
                    }
                }else if(xpos < link.w){ // might need to be 0
                    switch(num){
                        case 0:
                            xpos++; // move link back into range
                            break;
                        case 1:
                            num = 0; // move into quadrant 0
                            xpos = SCREEN_WIDTH - link.w; // move to right side of the screen
                            break;
                        case 2:
                            xpos++; // move link back into screen
                            break;
                        case 3:
                            num = 2; // move to quadrant 2
                            xpos = SCREEN_WIDTH - link.w; // move to right side of screen
                            break;
                    }
                }else if(ypos >= SCREEN_HEIGHT - link.h){
                    switch(num){
                        case 0:
                            num = 2; // move to quadrant 2
                            ypos = link.h; // move back to the top side, might need to be set to 0
                            break;
                        case 1:
                            num = 3; // move to quadrant 3
                            ypos = link.h; // move back to the top side of the screen, might need to be set to 0
                            break;
                        case 2:
                            ypos--; // move back into screen
                            break;
                        case 3:
                            ypos--; // move back into screen
                            break;
                    }
                }else if(ypos < link.h){ // might need to be 0
                    switch(num){
                        case 0:
                            ypos++; // move back into screen
                            break;
                        case 1:
                            ypos++; // move back into screen
                            break;
                        case 2:
                            num = 0; // move into quadrant 0
                            ypos = SCREEN_HEIGHT - link.h; // move to the bottom of the screen
                            break;
                        case 3:
                            num = 1; // move into quadrant 1
                            ypos = SCREEN_HEIGHT - link.h; // move to the bottom of the screen
                            break;
                    }
                }
                    

                // Clear Screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render current texture
                SDL_RenderCopy(gRenderer, gSpriteSheetTexture.texture(), &gSpriteClips[num], &fullScreen);
//                gSpriteSheetTexture.render(0, 0, &gSpriteClips[num], &fullScreen);

                // Render link
                gFooTexture.render(xpos, ypos, &link);

                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}

/* vim: set sts=4 sw=5 ts=8 expandtab ft=c: */
