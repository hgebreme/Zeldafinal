/*Andrew Nemecek
 * SDL2 tutorial 2 */
// UPDATE 04/12: added treasure (red heart) sprite

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<string>
#include<time.h>
using namespace std;

// Screen dimensions constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPEED = 5;

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
SDL_Rect link[12]; // link
SDL_Rect treasure[1]; // treasure
LTexture gFooTexture;	// texture for link
LTexture gTreasureTexture; 	// texture for treasure
SDL_Rect stretch = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 20, 40, 40}; // initially will put link at middle of screen
SDL_Rect treasureSpace = {SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 - 20, 20, 20}; // heart location
LTexture gEnemyTexture; // texture for enemies
SDL_Rect enemies[6]; // enemy sprites (size may need to be increased if more enemies are added
SDL_Rect estretch0 = {SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 20, 40, 40}; // initially at middle of screen
SDL_Rect estretch1 = {3*SCREEN_WIDTH / 4, 3*SCREEN_HEIGHT / 4, 40, 40}; // initially lower right
SDL_Rect estretch2 = {SCREEN_WIDTH/4, 3*SCREEN_HEIGHT / 4, 40, 40}; // lower left
SDL_Rect bstretch = {SCREEN_WIDTH / 2 - 30, SCREEN_HEIGHT / 2 - 30, 60, 60}; // initially middle of the screen

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
LTexture gInvText;
SDL_Rect invStretch = {SCREEN_WIDTH/3, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4}; // stretches inventory to correct size
SDL_Rect heart1 = {3*SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8, 30, 30}; // first heart in life meter
SDL_Rect heart2 = {3*SCREEN_WIDTH / 4 + 45, SCREEN_HEIGHT / 8, 30, 30}; // second heart in life meter
SDL_Rect heart3 = {3*SCREEN_WIDTH / 4 + 90, SCREEN_HEIGHT / 8, 30, 30}; // third heart in life meter
SDL_Rect lifeFont = {2*256/3, 10, 256/3, 224 / 8 - 10}; // "-LIFE-"
SDL_Rect lifeStretch = {2*SCREEN_WIDTH / 3 + 20, 0, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8}; // stretches "-LIFE-" to correct size


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
        
        // walking down 1
        link[0].w = 20;
        link[0].h = 20;
        link[0].x = 0;
        link[0].y = 0;

        // walking down 2
        link[1].w = 20;
        link[1].h = 20;
        link[1].x = 0;
        link[1].y = 30;

        // walking up 1
        link[2].w = 20;
        link[2].h = 20;
        link[2].x = 60;
        link[2].y = 0;

        // walking up 2
        link[3].w = 20;
        link[3].h = 20;
        link[3].x = 60;
        link[3].y = 30;

        // walking left 1
        link[4].w = 20;
        link[4].h = 20;
        link[4].x = 30;
        link[4].y = 0;

        // walking left 2
        link[5].w = 20;
        link[5].h = 20;
        link[5].x = 30;
        link[5].y = 30;

        // walking right 1
        link[6].w = 20;
        link[6].h = 20;
        link[6].x = 90;
        link[6].y = 0;

        // walking right 2
        link[7].w = 20;
        link[7].h = 20;
        link[7].x = 90;
        link[7].y = 30;

        // attack down
        link[8].w = 20;
        link[8].h = 32;
        link[8].x = 0;
        link[8].y = 80;

        // attack up
        link[9].w = 20;
        link[9].h = 32;
        link[9].x = 60;
        link[9].y = 80;

        // attack left
        link[10].w = 32;
        link[10].h = 20;
        link[10].x = 22;
        link[10].y = 90;

        // attack right
        link[11].w = 32;
        link[11].h = 20;
        link[11].x = 82;
        link[11].y = 90;

    }

	// Load treasure (red heart)
	if( !gTreasureTexture.loadFromFile( "zelda-sprites-treasures.png") ) {
		printf("Failed to load treasure image\n");
		success = false;
	}else{
		// edit rect dimensions to include only red heart
		treasure[0].w = 7;
		treasure[0].h = 7;
		treasure[0].x = 0;
		treasure[0].y = 0;
	}

    // Load map image
    if( !gSpriteSheetTexture.loadFromFile( "zelda-overworld(map).png") ){
        printf("Failed to load map image\n");
        success = false;
    }else{
        // must edit for appropriate pixels and add loops to get full image
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
    if( !gEnemyTexture.loadFromFile("zelda-sprites-enemies.png") ){
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
    }

    

    return success;
}

void close(){
    // Free Loaded images
    gSpriteSheetTexture.free();
    gFooTexture.free();
    gTreasureTexture.free();
    gInvText.free();

    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}

bool checkBounds(SDL_Rect stretch, SDL_Rect fullScreen, int &xpos, int &ypos, int prevxpos, int prevypos, int &num, bool link){
                // Check if in bounds
                switch(num){
                    case 0:
                        if(ypos < fullScreen.h / 6 + fullScreen.y - 10){
                            ypos = prevypos;
                        }else if(xpos < fullScreen.w / 16){
                            xpos = prevxpos;
                        }else if(ypos > SCREEN_HEIGHT - stretch.h - 2*fullScreen.h / 11 + 10){
                            ypos = prevypos;
                        }else if(xpos < fullScreen.w / 8 && ypos < 4*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos; 
                            xpos = prevxpos;
                        }else if(xpos < 3*fullScreen.w/16 && ypos < 3*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < fullScreen.w / 8 && ypos > 7*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 3*fullScreen.w/16 && ypos > 8*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos; 
                        }else if(xpos > 12*fullScreen.w/16 - stretch.w + 10 && ypos < 3*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 12*fullScreen.w/16 - stretch.w + 10 && ypos > 8*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 15*fullScreen.w/16 - stretch.w + 10 && ypos < 5*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 15*fullScreen.w/16 - stretch.w + 10 && ypos > 6*fullScreen.h/11 + fullScreen.y - (stretch.h - 10)){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > fullScreen.w - stretch.w && link){
                            num = 1;
                            xpos = 0;
                        }
                        break;
                    case 1:
                        if(ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                        }else if(xpos > 11*fullScreen.w/16 - stretch.w + 10){
                            xpos = prevxpos;
                        }else if(xpos > 5*fullScreen.w/16 - stretch.w + 10 && xpos < 10*fullScreen.w/16 && ypos > 6*fullScreen.h/11 + fullScreen.y - (stretch.h - 10)){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 5*fullScreen.w/16 -stretch.w + 10 && ypos < 5*fullScreen.h/11 + fullScreen.y - 10 ){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 5*fullScreen.w/16 && ypos > 9*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < fullScreen.w/16 && ypos < 5*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 3*fullScreen.w/16 - stretch.w + 10 && ypos < 3*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 4*fullScreen.w/16 - stretch.w + 10 && ypos < 4*fullScreen.h / 11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos; 
                        }else if(xpos > 3*fullScreen.w/16 - stretch.w + 10 && xpos < fullScreen.w/2 && ypos > 9*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 4*fullScreen.w/16 - stretch.w + 10 && xpos < fullScreen.w/2 && ypos > 8*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 0 && link){
                            num = 0;
                            xpos = SCREEN_WIDTH - stretch.w;
                            ypos = 5*fullScreen.h/11 + fullScreen.y;
                        }else if(ypos > SCREEN_HEIGHT - stretch.h && link){
                            num = 3;
                            xpos = 7*fullScreen.w/16;
                            ypos = fullScreen.y;
                        }
                        break;
                    case 3:
                        if(ypos > 9*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                        }else if(xpos > 15*fullScreen.w/16 - stretch.w + 10){
                            xpos = prevxpos;
                        }else if(xpos < 7*fullScreen.w/16 && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 8*fullScreen.w/16 - stretch.w + 10 && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 14*fullScreen.w/16 - stretch.w + 10 && ypos < 3*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 14*fullScreen.w/16 - stretch.w + 10 && ypos > 8*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(ypos < fullScreen.y && link){
                            num = 1;
                            ypos = SCREEN_HEIGHT - stretch.w;
                            xpos = 10*fullScreen.w/16;
                        }else if(xpos < 0 && link){
                            num = 2;
                            xpos = SCREEN_WIDTH - stretch.w;
                        }
                        break;
                    case 2:
                        if(xpos < fullScreen.w/16){
                            xpos = prevxpos;
                        }else if(ypos > 9*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                        }else if(xpos < 4*fullScreen.w/16 && ypos < 3*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 3*fullScreen.w/16 && ypos < 4*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 2*fullScreen.w/16 && ypos < 5*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 2*fullScreen.w/16 && ypos > 6*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 3*fullScreen.w/16 && ypos > 7*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 4*fullScreen.w/16 && ypos > 8*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos < 11*fullScreen.w/16 && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 12*fullScreen.w/16 - stretch.w + 10 && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > fullScreen.w - stretch.w && link){
                            num = 3;
                            xpos = 0;
                        }else if(xpos > 11*fullScreen.w/16 - 15 && xpos < 12*fullScreen.w/16 - stretch.w + 15 && ypos < 3*fullScreen.h/22 + fullScreen.y && link){
                            num = 4;
                            xpos = (fullScreen.w - stretch.w) / 2;
                            ypos = 9*fullScreen.h/11 + fullScreen.y - stretch.h;
                        }
                        break;
                    case 4:
                        if(xpos < 2*fullScreen.w/16){
                            xpos = prevxpos;
                        }else if(xpos > 14*fullScreen.w/16 - stretch.w + 10){
                            xpos = prevxpos;
                        }else if(ypos > 9*fullScreen.h/11 + fullScreen.y - stretch.h + 10){
                            ypos = prevypos;
                        }else if(xpos < 7*fullScreen.w/16 && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(xpos > 9*fullScreen.w/16 - stretch.w && ypos < 2*fullScreen.h/11 + fullScreen.y - 10){
                            ypos = prevypos;
                            xpos = prevxpos;
                        }else if(ypos < 3*fullScreen.h/22 + fullScreen.y - 10){
                            num = 5;
                            xpos = (fullScreen.w - stretch.w) / 2;
                            ypos = 9*fullScreen.h/11 + fullScreen.y - stretch.h;
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

            // Event handler
            SDL_Event e;
        
            // Determine which square link is in
            int num = 0;

            // Determine which sprite to use for animating Link
            int frame = 0;
            int prev = frame; // tracks previous frame

            // Character position locations
            int lxpos = SCREEN_WIDTH / 2; // link initial x
            int lypos = SCREEN_HEIGHT / 2; // link initial y
            int e0xpos = SCREEN_WIDTH / 2 - 20; // enemy 0 x
            int e0ypos = SCREEN_HEIGHT / 2 - 20; // enemy 0 y
            int e1xpos = 3* SCREEN_WIDTH / 4 - 20; // enemy 1 x
            int e1ypos = 3*SCREEN_HEIGHT / 4 - 20; // enemy 1 y
            int e2xpos = SCREEN_WIDTH / 4 - 20; // enemy 2 x
            int e2ypos = 3*SCREEN_HEIGHT / 4 - 20; // enemy 2 y
            int bxpos = SCREEN_WIDTH / 2 - 20; // boss x
            int bypos = SCREEN_HEIGHT / 2 - 20; // boss y

			// position of red heart
			int heartx = SCREEN_WIDTH / 2 + 40;
			int hearty = SCREEN_HEIGHT / 2 - 10;

            // Current rendered texture
            LTexture* currentTexture = NULL;

            // While application is running
            while( !quit ){
                // Handle events on queue
                while(SDL_PollEvent( &e ) != 0){
                    // User requests quit
                    if( e.type == SDL_QUIT ){
                        quit = true;
                    }else if(e.type == SDL_KEYDOWN){
                        // User presses 'A' for attack
                        if(e.key.keysym.scancode == SDL_SCANCODE_A){
                            if(frame == 0 || frame == 1){
                                prev = frame;
                                frame = 8;
                            }else if(frame == 2 || frame == 3){
                                prev = frame;
                                frame = 9;
                            }else if(frame == 4 || frame == 5){
                                prev = frame;
                                frame = 10;
                            }else if(frame == 6 || frame == 7){
                                prev = frame;
                                frame = 11;
                            }
                        }
                    }
                }

                // move link, change sprite for animation
                int lprevxpos = lxpos; // used for boundary check
                int lprevypos = lypos; // used for boundary check
                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                if(currentKeyStates[SDL_SCANCODE_UP]){
                    lypos -= SPEED; // move up
                    if(frame == 2){
                        frame = 3;
                    }else{
                        frame = 2;
                    }
                }else if(currentKeyStates[SDL_SCANCODE_DOWN]){
                    lypos += SPEED; // move down
                    if(frame == 0){
                        frame = 1;
                    }else{
                        frame = 0;
                    }
                }else if(currentKeyStates[SDL_SCANCODE_LEFT]){
                    lxpos -= SPEED; // move left
                    if(frame == 4){
                        frame = 5;
                    }else{
                        frame = 4;
                    }
                }else if(currentKeyStates[SDL_SCANCODE_RIGHT]){
                    lxpos += SPEED; // move right;
                    if(frame == 6){
                        frame = 7;
                    }else{
                        frame = 6;
                    }
                }

                checkBounds(stretch, fullScreen, lxpos, lypos, lprevxpos, lprevypos, num, true);

                // move enemies
                int direction = rand() % 4;
                int e0prevxpos = e0xpos;
                int e0prevypos = e0ypos;
                int e1prevxpos = e1xpos;
                int e1prevypos = e1ypos;
                int e2prevxpos = e2xpos;
                int e2prevypos = e2ypos;
                int bprevxpos = bxpos;
                int bprevypos = bypos;
                switch(direction){
                    case 0:
                        e0xpos += SPEED;
                        e1xpos += SPEED;
                        e2xpos += SPEED;
                        bxpos += SPEED;
                        break;
                    case 1:
                        e0xpos -= SPEED;
                        e1xpos -= SPEED;
                        e2xpos -= SPEED;
                        bxpos -= SPEED;
                        break;
                    case 2:
                        e0ypos += SPEED;
                        e1ypos += SPEED;
                        e2ypos += SPEED;
                        bypos += SPEED;
                        break;
                    case 3:
                        e0ypos -= SPEED;
                        e1ypos -= SPEED;
                        e2ypos -= SPEED;
                        bypos -= SPEED;
                        break;
                }

                checkBounds(estretch0, fullScreen, e0xpos, e0ypos, e0prevxpos, e0prevypos, num, false);
                checkBounds(estretch1, fullScreen, e1xpos, e1ypos, e1prevxpos, e1prevypos, num, false);
                checkBounds(estretch2, fullScreen, e2xpos, e2ypos, e2prevxpos, e2prevypos, num, false);
                checkBounds(bstretch, fullScreen, bxpos, bypos, bprevxpos, bprevypos, num, false);


                // set render locations for characters
                // link
                stretch.x = lxpos;
                stretch.y = lypos;

                // enemies 0-2
                estretch0.x = e0xpos;
                estretch0.y = e0ypos;
                estretch1.x = e1xpos;
                estretch1.y = e1ypos;
                estretch2.x = e2xpos;
                estretch2.y = e2ypos;

                // boss
                bstretch.x = bxpos;
                bstretch.y = bypos;

                // Clear Screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render current texture
                if(num == 0){ // starting zone
                    SDL_RenderCopyEx(gRenderer, gSpriteSheetTexture.texture(), &gSpriteClips[num], &fullScreen, 0, NULL, SDL_FLIP_HORIZONTAL); // flip image for map area 0
                }else if(num >=1 && num < 4){ // other overworld locations
                    SDL_RenderCopy(gRenderer, gSpriteSheetTexture.texture(), &gSpriteClips[num], &fullScreen);
                }else{ // dungeon
                    SDL_RenderCopy(gRenderer, gDungeonTexture.texture(), &gDungeonClip[num-4], &fullScreen);
                }
//                gSpriteSheetTexture.render(0, 0, &gSpriteClips[num], &fullScreen);

                // Render link
                SDL_RenderCopy(gRenderer, gFooTexture.texture(), &link[frame], &stretch);
//                gFooTexture.render(xpos, ypos, &link);

		// Render treasure
		SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &treasureSpace);
//	          gTreasureTexture.render(heartx, hearty, &treasure);

                // Render enemies
                SDL_RenderCopy(gRenderer, gEnemyTexture.texture(), &enemies[1], &estretch0);
                SDL_RenderCopy(gRenderer, gEnemyTexture.texture(), &enemies[1], &estretch1);
                SDL_RenderCopy(gRenderer, gEnemyTexture.texture(), &enemies[1], &estretch2);

                // Render boss
                if(lxpos % 2 == 0){
                SDL_RenderCopy(gRenderer, gEnemyTexture.texture(), &enemies[4], &bstretch);
                }else{
                SDL_RenderCopy(gRenderer, gEnemyTexture.texture(), &enemies[5], &bstretch);
                }

                // Render HUD
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &blackRect);
                SDL_SetRenderDrawColor(gRenderer, 156, 156, 156, 0xFF);
                SDL_RenderFillRect(gRenderer, &greyRect);
                SDL_SetRenderDrawColor(gRenderer, 46, 206, 46, 0xFF);
                SDL_RenderFillRect(gRenderer, &greenLocation);
                SDL_RenderCopy(gRenderer, gInvText.texture(), &inventory, &invStretch);
//                gInvText.render(3*SCREEN_WIDTH / 8, SCREEN_HEIGHT/32, &inventory);
                SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart1);
                SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart2);
                SDL_RenderCopy(gRenderer, gTreasureTexture.texture(), &treasure[0], &heart3);
//                gInvText.render(2*SCREEN_WIDTH/3, SCREEN_HEIGHT/32, &lifeFont);
                SDL_RenderCopy(gRenderer, gInvText.texture(), &lifeFont, &lifeStretch);

                // Update screen
                SDL_RenderPresent(gRenderer);

                // if attacking, return to previous sprite
                if(frame > 7 && e.type == SDL_KEYUP){
                    frame = prev;
                }

                // Update location of greenLocation rectangle
                float xratio = float(lxpos) / SCREEN_WIDTH;
                float yratio = float(lypos - SCREEN_HEIGHT / 4) / (3*SCREEN_HEIGHT / 4);
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
 
            }
        }
    }

    // Free resources and close SDL
    close();

    return 0;
}

/* vim: set sts=4 sw=5 ts=8 expandtab ft=c: */
 
 
