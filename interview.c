#include <SDL2/SDL.h>//This is the header file for SDL (Simple DirectMedia Layer), which is a cross-platform development 
//library designed for creating multimedia applications, especially games.
//It provides functions for initializing video, audio, handling input devices like keyboard and mouse, and rendering graphics.
#include <SDL2/SDL_ttf.h> //This header file is for SDL_ttf, 
//an SDL extension library that allows SDL applications to use TrueType fonts for text rendering.
#include <stdbool.h>
//It defines the boolean data type along with the values true and false.
//Using stdbool.h, you can use the bool type and true and false keywords in C programs,
// making the code more readable and expressive.
#include <stdio.h>
//It provides functions like printf() and scanf() for formatted input and output.
// It also includes file input and output functions like fopen(), fclose(), fprintf(), etc.
#include <time.h>
//It provides functions for getting the current system time, formatting time values,
// and performing arithmetic operations on time values.

//x axis goes from left to right 
//y axis goes from top to down
//* means it is the memory location of a variable which contains the variable, can be used to allocate a memory for a variable or call a memory that contains a variable
//* also means multiply
//define is preprocessor directive to define macros

// Screen dimension constants
#define SCREEN_WIDTH 800 //SCREEN WIDTH is defined or replaced as a variable with 800 pixels
#define SCREEN_HEIGHT 600 //SCREEN HEIGHT is defined or replaced as a variable with 600 pixels
// Block constants
#define BLOCK_ROWS 5 // BLOCK ROWS is defined or replaced as variable with 5 rows
#define BLOCK_COLUMNS 10 //BLOCK COLUMS is defined or replaced as variable with 10 columns
#define BLOCK_WIDTH 64 //BLOCK WIDTH is the width of each block defined or replaced as variable with 64 pixels
#define BLOCK_HEIGHT 20 //BLOCK HEIGHT  is the height of each block defined or replaced as variable with 20 pixels
#define PADDLE_WIDTH 100 //PADDLE WIDTH  is the width of the paddle defined or replaced as variable with 100 pixels
#define PADDLE_HEIGHT 20 //PADDLE HEIGHT is the height of the paddle defined or replaced as variable with 20 pixels

// Ball constants
#define BALL_SIZE 15 //BALL SIZE is the size of the ball which is replaced as 15 pixels

// Game objects
typedef struct { //struct data type
    float x, y; //Ball.x and Ball.y will be a float (integer with decimals)
    float velX, velY; //Ball.velX and Ball.velY will be a float
} Ball;

typedef struct {//struct data type
    int x, y;//Paddle.x and Paddle.y will be an integer
    int width, height; //Paddle.width and Paddle.height will be an integer
} Paddle;

typedef struct {
    int x, y; //Block.x and Block.y will be an integer
    bool active; //Block.active will be a boolean (true or false)
} Block;

SDL_Window* window = NULL; //declares variable window as the memory address of SDL_Window and its value is nothing
SDL_Renderer* renderer = NULL;// declares variable renderer as the memory address of SDL Renderer with nothing
TTF_Font* font = NULL;//declares variable font as a type of TTF font memory address with no value
Ball ball; //declares ball as a type of Ball
Paddle paddle; //declares paddle as a type of Paddle
Block blocks[BLOCK_ROWS][BLOCK_COLUMNS]; // Declares a global 2D array blocks as a type of Block.  
//The array dimensions are specified by BLOCK_ROWS and BLOCK_COLUMNS constants, 
//BLOCK_ROWS and BLOCK_COLUMNS are replaced by their defined values during preprocessing,
// resulting in an array declaration of size [5][10] which is the rows and columns of an object
int health = 3;
//declare health as variable with 3 value

Uint32 startTime; //32 bit unsinged integers (0-4294967295 integer values)(variable is startTime)
Uint32 elapsedTime;//32 bit unsinged integers (0-4294967295 integer values)(variable is elapsedTime)
Uint32 bestTime; //32 bit unsinged integers (0-4294967295 integer values)(variable is bestTime)

typedef enum {//struct data type
    MENU,//GameState.MENU is the value of GameState
    PLAYING,//GameState.PLAYING is the value of GameState
    GAME_OVER//GameState.GAME_OVER is the value of GameState
} GameState;

GameState currentState = MENU; //declared currentState as a variable for enum Gamestate using MENU as a test data example

//functions in the program:

bool init();///init(): Initializes SDL. If initialization fails, it prints an error message and returns -1.
void close();//closes the program and cleans up the program when x button is hit
void initGameObjects();//initGameObjects(): Initializes game objects (like the ball, paddle, blocks, etc.).
void handleEvents(bool* quit);//to make the paddle move
void update();//runs the game and make the game mechanics work
void render();//presents the game with time and sets the content of the game
bool loadMedia();//initialize the font variable with the font file
void handleMenuEvents(bool* quit);//to make the game able to quit without crashing with x button
void renderMenu();// prints the menu words and function to start the game
// void handleGameOverEvents(bool* quit);
void renderGameOver(); // prints the game over page 
bool areAllBlocksDestroyed(); //use to check whether all the blocks are destroyed
void renderText(const char* message, int x, int y);//used in rendering text in line 378
void loadBestTime();//to read the best time record in a text file
void saveBestTime();// to write the best time record in a text file

bool init() { //init is either true or false
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {//SDL INIT VIDEO is the library for video
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());//prints the get error message
        return false;
        //returns false for the boolen if failed
    }

    if (TTF_Init() == -1) {//TTF_Init is ttf library will print out error if it cannot run the library
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());//prints the ttf get error message
        return false;//returns false for the boolen if failed
    }

    window = SDL_CreateWindow("Bouncing Ball Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {//Creates window for application will print out error if window cannot be created
    //position of the window on the screen is undefined meaning it will be default position
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());//prints the sdl get error message
        return false;//returns false for the boolen if failed
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//Renderer accelerated creates hardware accceleration
    if (!renderer) {//Creates a renderer for the window will print out error if cannot be created
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());//prints the sdl get error message
        return false;//returns false for the boolen if failed
    }

    return true;
    //returns true to the boolean if succeed
}

bool loadMedia() { //returns either true or false, true meaning it will run false meaning it wont run
    font = TTF_OpenFont("arial.ttf", 28);//TTFOpen Font loads a true type font//arial.ttf is the folder of the font// 28 is the size of the font
    if (font == NULL) {//if font is not existed it will print the error message
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());//TTF_GetError will get the error message on the msys panel
        return false;//return false
    }

    return true;//returns true if succeed
}

void renderText(const char* message, int x, int y) {//used in line 378 to render text
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

//SDL_Color textColor = { 255, 255, 255, 255 };: 
//This line defines the text color as white with full opacity (RGBA values: Red = 255, Green = 255, Blue = 255, Alpha = 255).

//SDL_Surface* textSurface = TTF_RenderText_Solid(font, message, textColor);: 
//This line renders the text onto an SDL surface using the specified TrueType font (font), message text (message), 
//and text color (textColor).

//SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);: 
//Here, the text surface is converted into an SDL texture which can be rendered efficiently by the renderer.

//SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };: 
//This line defines the rectangle where the text texture will be rendered. 
//The x and y coordinates specify the top-left corner of the rectangle, 
//and the width and height are obtained from the dimensions of the text surface.

//SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);: 
//The text texture is rendered onto the renderer at the specified position

//SDL_DestroyTexture(textTexture); and SDL_FreeSurface(textSurface);: 
//Finally, the allocated texture and surface resources are freed to prevent memory leaks.


void renderMenu() {
    SDL_SetRenderDrawColor(renderer, 135, 149, 153, 1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 135, 149, 153, 1);//roman silver color the background color of the start button

    SDL_Rect startButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25, 100, 50 };
    SDL_RenderFillRect(renderer, &startButton);

    renderText("Press any key to start.", startButton.x + 10, startButton.y + 10);

    SDL_RenderPresent(renderer);
}

//SDL_SetRenderDrawColor(renderer,  135, 149, 153, 1); roman silver color 
//This line sets the draw color of the renderer to black with full opacity.

//SDL_RenderClear(renderer);: 
//It clears the entire renderer with the current draw color, effectively filling the screen with roman silver

//SDL_SetRenderDrawColor(renderer,  135, 149, 153, 1);: This line sets the draw color to roman silver again
// This could be redundant as it seems to set the draw color to the same color as before.

//SDL_Rect startButton = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 25, 100, 50 };: 
//Defines the rectangle representing the start button.
// It's positioned at the center of the screen with a width of 100 pixels and a height of 50 pixels.

//SDL_RenderFillRect(renderer, &startButton);: Renders the start button rectangle onto the renderer,
// effectively displaying it on the screen.

//renderText("Press any key to start.", startButton.x + 10, startButton.y + 10);: 
//Calls the renderText() function to render the text "Press any key to start." onto the start button.
// The text is positioned with an offset of (10, 10) from the top-left corner of the start button.

//SDL_RenderPresent(renderer);: Presents the rendered content on the screen, making it visible to the user.


void handleMenuEvents(bool* quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            *quit = true;
        } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_KEYDOWN) {
            currentState = PLAYING;
        }
    }
}

//
//This function is responsible for handling events that occur during the menu state.
//It continuously polls for events using SDL_PollEvent(&e) until there are no more events in the event queue.

//If the event type is SDL_QUIT, indicating that the user has attempted to close the window,
// *quit is set to true, 
//indicating that the game should be terminated.

//If the event type is SDL_MOUSEBUTTONDOWN or SDL_KEYDOWN,
// indicating that the user has pressed a mouse button or a key, respectively, the currentState is set to PLAYING,
// transitioning the game state from the menu to the playing state.


void initGameObjects() {
    ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
    ball.velX = 3.0f;
    ball.velY = 3.0f;
    //ball.x and ball.y: Set the initial position of the ball at the center of the screen. 
    //SCREEN_WIDTH and SCREEN_HEIGHT likely represent the dimensions of the game screen.
    //ball.velX and ball.velY: Set the initial velocity of the ball.
    // The ball starts moving with a velocity of 3.0 units per frame in both the horizontal and vertical directions

    paddle.x = SCREEN_WIDTH / 2 - 50;
    paddle.y = SCREEN_HEIGHT - 30;
    paddle.width = PADDLE_WIDTH;
    paddle.height = PADDLE_HEIGHT;

    //paddle.x and paddle.y: Set the initial position of the paddle near the bottom center of the screen.
    //paddle.width and paddle.height: Set the dimensions of the paddle. PADDLE_WIDTH and PADDLE_HEIGHT likely represent predefined
    // constants for the width and height of the paddle.

    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLUMNS; ++j) {
            blocks[i][j].x = j * (BLOCK_WIDTH + 5) + 35; //35 pixels distance from left, +5 is distance between blocks
            blocks[i][j].y = i * (BLOCK_HEIGHT + 5) + 100;// 100 pixels distance from top, +5 is distance between blocks
            blocks[i][j].active = true;
        }
    }
    //will be rendered out on line 517

    
    //Nested loops iterate over each row and column of the blocks array.
//blocks[i][j].x and blocks[i][j].y: Set the initial position of each block in the grid.
// Blocks are positioned in a grid pattern with some spacing between them. 
//The position calculation likely ensures that blocks are evenly distributed across the screen.
//blocks[i][j].active: Set all blocks to be initially active, indicating that they are not yet destroyed or inactive.
//blocks[i][j].x: Sets the x-coordinate of the block at position [i][j] in the blocks array.
//j * (BLOCK_WIDTH + 5): Calculates the horizontal position of the block. 
//It multiplies the column index j by the total width of a block plus a spacing of 5 pixels between each block.
//+ 35: Adds an offset of 35 pixels to adjust the horizontal position of the blocks. 
//This likely shifts the blocks towards the right on the screen

//blocks[i][j].y: Sets the y-coordinate of the block at position [i][j] in the blocks array.
//i * (BLOCK_HEIGHT + 5): Calculates the vertical position of the block. 
//It multiplies the row index i by the total height of a block plus a spacing of 5 pixels between each block.
//+ 100: Adds an offset of 100 pixels to adjust the vertical position of the blocks. 
//This likely shifts the blocks downward on the screen.

//when i and j is equal to one it will render the block at the first position

    startTime = 0;//time when it starts is 0
}

void handleEvents(bool* quit) { //bool* quit: A pointer to a boolean variable that indicates whether the game should quit. 
//The function modifies the value pointed to by this pointer if the user triggers an event that should exit the game.
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
//SDL_Event e;: Declares an SDL event variable e to store the current event being processed.
//SDL_PollEvent(&e) != 0: This loop polls for events from the event queue. 
//It continues looping as long as there are events in the queue to process. 
//SDL_PollEvent retrieves the next event from the queue and stores it in e.
        if (e.type == SDL_QUIT) {
//This conditional checks if the current event type is SDL_QUIT, which occurs when the user attempts to close the game window.
//If the SDL_QUIT event is detected, it sets the value pointed to by quit to true, indicating that the game should quit
            *quit = true;
        } else if (e.type == SDL_KEYDOWN) {
              switch (e.key.keysym.sym) {//reporting key press for sdl library, making key press commands work
                case SDLK_LEFT:
                    paddle.x -= 10;
                    break;
                case SDLK_RIGHT:
                    paddle.x += 10;
                    break;

//This conditional checks if the current event type is SDL_KEYDOWN, which occurs when a key is pressed.
//Inside the SDL_KEYDOWN block, a switch statement is used to handle different key presses.
//If the pressed key is SDLK_LEFT, it moves the paddle to the left by subtracting 10 from its x-coordinate.
//If the pressed key is SDLK_RIGHT, it moves the paddle to the right by adding 10 to its x-coordinate.
            }
        }
    }
}



bool areAllBlocksDestroyed() {//will be called in line 436
    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLUMNS; ++j) {
            if (blocks[i][j].active) {// blocks.active is not existed then this condition wont be met, relates to line 399
                return false;
            }
        }
    }
    return true;
}

//this function detects the amoung of blocks that are still existing after the amount of blocks that are updated on the void update()
//This function checks whether all blocks on the game screen are destroyed.
// It iterates through each block in the grid of blocks (BLOCK_ROWS rows and BLOCK_COLUMNS columns).
// If it finds an active block (i.e., not destroyed), it immediately returns false, indicating that not all blocks are destroyed.
// If the function completes its loop without finding any active blocks, it returns true, indicating that all blocks are destroyed.

void update() {
    // Ball movement
    ball.x += ball.velX;
    ball.y += ball.velY;
    //These lines update the position of the ball by adding its velocity (ball.velX and ball.velY)
    //to its current position (ball.x and ball.y).

    startTime = 0;
    //This line seems to reset some variable named startTime to 0

    // Ball collision with walls
    if (ball.x <= 0 || ball.x >= SCREEN_WIDTH - BALL_SIZE) {
        ball.velX = -ball.velX;
    //These conditional statements check if the ball collides with the left, right, or top walls of the game window. 
    //If the ball collides with the left or right walls, its horizontal velocity (ball.velX) is reversed to simulate bouncing off 
    //the walls. 
    //If the ball collides with the top wall(0), its vertical velocity (ball.velY) is reversed.
    }
    if (ball.y <= 0) {
        ball.velY = -ball.velY;
    }

    // Ball collision with paddle
    if (ball.y + BALL_SIZE >= paddle.y &&
        ball.x + BALL_SIZE >= paddle.x &&
        ball.x <= paddle.x + paddle.width &&
        ball.y <= paddle.y + paddle.height) {
        ball.velY = -ball.velY;
    }
   //ball.y + BALL_SIZE >= paddle.y: Checks if the bottom edge of the ball (its y-coordinate plus its size) is at or below 
   //the top edge of the paddle. This condition ensures that the ball is vertically aligned with or below the top edge of the paddle.

    //ball.x + BALL_SIZE >= paddle.x: Checks if the right edge of the ball (its x-coordinate plus its size) is to the
    // right of or aligned with the left edge of the paddle.
    // This condition ensures that the ball is horizontally aligned with or to the right of the left edge of the paddle.

    //ball.x <= paddle.x + paddle.width: Checks if the left edge of the ball is to the left of or aligned
    // with the right edge of the paddle (paddle's x-coordinate plus its width). This condition ensures that the 
    //ball is horizontally aligned with or to the left of the right edge of the paddle.

    //ball.y <= paddle.y + paddle.height: Checks if the top edge of the ball is above or aligned with the 
    //bottom edge of the paddle (paddle's y-coordinate plus its height).
    // This condition ensures that the ball is vertically aligned with or above the bottom edge of the paddle.

    //If all these conditions are true, it means that the ball's bounding 
    //box overlaps with the paddle's bounding box, indicating a collision between the ball and the paddle. 
    //When such a collision occurs, the code sets ball.velY (the vertical velocity of the ball) to its negative value (-ball.velY),
    // effectively reversing the vertical direction of the ball's movement.

    // Ball collision with blocks
    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLUMNS; ++j) {
            Block* block = &blocks[i][j];
            //This line declares a pointer to a Block structure named block and initializes it to point to the block at position [i][j]
            // in the blocks array.
            // This allows easy access to the properties of the current block being examined.
            if (block->active &&
                ball.x + BALL_SIZE >= block->x &&
                ball.x <= block->x + BLOCK_WIDTH &&
                ball.y + BALL_SIZE >= block->y &&
                ball.y <= block->y + BLOCK_HEIGHT) {
            //block->active: Checks if the current block is active. If it's inactive, the collision check is skipped for this block.
            //ball.x + BALL_SIZE >= block->x: 
            //Checks if the right edge of the ball is to the right of or aligned with the left edge of the block.
            //ball.x <= block->x + BLOCK_WIDTH: 
            //Checks if the left edge of the ball is to the left of or aligned with the right edge of the block.
            //ball.y + BALL_SIZE >= block->y: 
            //Checks if the bottom edge of the ball is below or aligned with the top edge of the block.
            //ball.y <= block->y + BLOCK_HEIGHT: 
            //Checks if the top edge of the ball is above or aligned with the bottom edge of the block

                block->active = false;
                ball.velY = -ball.velY;
                break;

            //If a collision between the ball and a block is detected, the block is marked as inactive (block->active = false),
            // indicating that it's destroyed or should no longer be rendered.
            //The vertical velocity of the ball (ball.velY) is reversed by setting it to its negative value (-ball.velY).
            // This simulates the ball bouncing off the block vertically.
            }
        }
    }

    // Check if the ball falls below the paddle
    if (ball.y >= SCREEN_HEIGHT) {
        health--;
        if (health <= 0) {
            currentState = GAME_OVER;

            
        } else {
            // Reset ball position and velocity
            ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
            ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
            ball.velX = 3.0f;
            ball.velY = 3.0f;

            currentState = PLAYING;
        }
    }
    // conditional statement 
    //checks if the ball falls below the paddle (i.e., reaches the bottom of the screen).
    //If the ball falls below the paddle, the player loses a life (health--). 
    //If the player has no remaining lives (health <= 0), the game state is updated to GAME_OVER.
    //If the player has remaining lives, the ball's position and velocity are
    // reset to their initial values, and the game state is updated to PLAYING.

    // Check if all blocks are destroyed
    if (areAllBlocksDestroyed()) {
        elapsedTime = SDL_GetTicks(); //SDL get ticks counts the number of milliseconds since the library ran

        if (elapsedTime < bestTime) { //bestTime is the variable in the functions from loadBestTime and saveBestTime for reading and writing the file data
                                    //saveBestTime function writes the best time which is the shortest time given game completed 
            bestTime = elapsedTime; //bestTime is the new best time if the SDL get ticks is shorter that the previous record
            saveBestTime();     //saveBestTime function writes the best time into the file
        }
       currentState = GAME_OVER; //to record the game state
       renderGameOver();
    }
}

void saveBestTime() {
    FILE* file = fopen("best_time.txt", "w");
    if (file) {
        fprintf(file, "%u\n", bestTime);
        fclose(file);
    }
}

//This function attempts to open the file named "best_time.txt" in write mode ("w").

// If the file is successfully opened (file != NULL), 
//it writes the value of bestTime followed by a newline character ('\n') to the file using fprintf().

// Finally, it closes the file using fclose() to save the changes and release the file resource.


void loadBestTime() {
    FILE* file = fopen("best_time.txt", "r");
    if (file) {
        fscanf(file, "%u", &bestTime);
        fclose(file);
    }
}

//This function attempts to open the file named "best_time.txt" in read mode ("r").

// If the file is successfully opened (file != NULL)(not equals to nothing),

// it reads an unsigned integer from the file using fscanf() and stores it in the variable bestTime.

// Finally, it closes the file using fclose() to release the file resource.

void render() {
    SDL_SetRenderDrawColor(renderer, 135, 149, 153, 1);
    SDL_RenderClear(renderer);
//SDL_SetRenderDrawColor is a function provided by SDL that sets the color used for drawing operations on the renderer.
//The parameters ( 135, 149, 153, 1); represent the RGBA color components: Red = 135, Green = 149, Blue = 153, and Alpha = 1
// (which is fully opaque).
//This line sets the render draw color to roman silver with full opacity.
//SDL_RenderClear is a function that clears the entire rendering target (usually the window associated with the renderer)
// with the current render draw color.
//In this case, it clears the renderer with the previously set black color, effectively filling the entire window with black.

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);// ball rect color, paddleRect color, and blockRect
//This line sets the render draw color to white (#FFFFFF) with full opacity.
    SDL_Rect ballRect = { (int)ball.x, (int)ball.y, BALL_SIZE, BALL_SIZE };
//This code segment creates an SDL_Rect structure named ballRect, which represents the rectangle to be rendered as the ball.
//The position of the rectangle is determined by the current position of the ball (ball.x, ball.y), 
//and its size of height and width are defined by the BALL_SIZE constant.
//SDL_RenderFillRect is a function that fills the given rectangle on the renderer with the current render draw color.
//In this case, it fills the rectangle representing the ball with the previously set white color, 
//effectively rendering a white rectangle representing the ball at its current position.

    SDL_RenderFillRect(renderer, &ballRect);

    // SDL_Rect paddleRect = { paddle.x, paddle.y, paddle.width, paddle.height };
    // SDL_RenderFillRect(renderer, &paddleRect);

    // // Render ball
    // SDL_Rect ballRect = { (int)ball.x, (int)ball.y, BALL_SIZE, BALL_SIZE };
    // SDL_RenderFillRect(renderer, &ballRect);

    // Render paddle
    SDL_Rect paddleRect = { (int)paddle.x, (int)paddle.y, paddle.width, paddle.height };// first two int is the x and y coordinates of the paddle, with 
    //the width and height of the paddle
    SDL_RenderFillRect(renderer, &paddleRect);

    for (int i = 0; i < BLOCK_ROWS; ++i) {
        for (int j = 0; j < BLOCK_COLUMNS; ++j) {
            if (blocks[i][j].active) {//condition states it will take the block coordinates and the block arrays that is active on the line 249
                SDL_Rect blockRect = { blocks[i][j].x, blocks[i][j].y, BLOCK_WIDTH, BLOCK_HEIGHT };// adds the block width and height into the coordinates
                SDL_RenderFillRect(renderer, &blockRect);
            }
        //or each active block (if (blocks[i][j].active)), it creates an SDL_Rect structure named blockRect representing the rectangle
        // to be rendered as the block.
        //The position of the rectangle is determined by the x and y properties of the current block (blocks[i][j].x, blocks[i][j].y),
        // and its size is defined by BLOCK_WIDTH and BLOCK_HEIGHT.
        //SDL_RenderFillRect is used to fill the rectangle representing the block on the renderer.
        }
    }

    // Render current time
    char timeText[50];
    snprintf(timeText, 50, "Time: %u ms", SDL_GetTicks() - startTime);
    renderText(timeText, 10, 400);// calls render text function
    //This code segment creates a character array timeText to store the text to be rendered.

    //snprintf is used to format the text to display the current time elapsed since the game started which is a string. 
    //It calculates the difference between the current tick count (SDL_GetTicks()) and the startTime variable.
    //The formatted text is then passed to a function renderText along with the position where it should be rendered (x = 10, y = 400).
    // This function likely renders text on the screen using SDL.


    loadBestTime(); //function to display best time from reading file

    // Render best time text
    if (bestTime) {
        char bestTimeText[50];
        snprintf(bestTimeText, 50, "Best Time: %u ms", bestTime);// best time is the variable used in loadBestTime to read the best time and will be printed out
        //snprintf is used for strings
        //printf also works for uint
        renderText(bestTimeText, 30, 30);// present the renderer to the screen, 30 is x and y coordinates from left and top
    }
    SDL_RenderPresent(renderer);//presents all the render functions
}





void renderGameOver() {
    SDL_SetRenderDrawColor(renderer, 135, 149, 153, 1);
    SDL_RenderClear(renderer);

    renderText("Game Over, Nice try", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 -50);

    SDL_RenderPresent(renderer);//presents all the render functions

    // loadBestTime();

    currentState = GAME_OVER;
}

// This function is responsible for rendering the "Game Over" screen when the game ends.
// It first sets the renderer's draw color to roman silver (135, 149, 153, 1);, and then clears the renderer, 
//effectively filling the screen with black.

// Using the renderText() function, it renders the text "Game Over" at the center of the screen horizontally (SCREEN_WIDTH / 2 - 100) 
//and vertically (SCREEN_HEIGHT / 2 - 50).

// After rendering the text, it presents the renderer, displaying the "Game Over, nice try, press x button to quit" screen to the user.

// Then, it calls loadBestTime() function. 

// Finally, it sets the currentState to GAME_OVER, indicating that the game state has transitioned to the game over state.



void close() {//function for making sure it shuts down properly
    SDL_DestroyRenderer(renderer);//releases rendered for created graphics
    SDL_DestroyWindow(window);//Destroys the window that was created
    TTF_CloseFont(font);//Releases the font resources
    TTF_Quit();//cleans up resources used for ttf library
    SDL_Quit();//cleans up resources used by sdl library
}



int main(int argc, char* args[]) {
    //argc is the number of strings or arguments in the program pointed to args, args is the argument array or vector containing the argument or string of the program
    if (!init()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

//init(): Initializes SDL. If initialization fails, it prints an error message and returns -1.

    if (!loadMedia()) {
        printf("Failed to load media!\n");
        return -1;
    }

//loadMedia(): Loads media resources. If loading fails, it prints an error message and returns -1

    initGameObjects();
//initGameObjects(): Initializes game objects (like the ball, paddle, blocks, etc.).

    bool quit = false;
    while (!quit) {
        if (currentState == MENU) {
            renderMenu();//prints out the menu words and press any key function to start the game
            handleMenuEvents(&quit);//makes the quit or x button quit the game
        } else if (currentState == PLAYING) {
            update();//runs the game and makes the game move with syntax that makes the game works as a whole
            render();//present the game the time and sets the content of the game
            handleEvents(&quit);//makes the quit or x button quit the game
        } else if (currentState == GAME_OVER) {
            // handleGameOverEvents(&quit);
            renderGameOver();//prints out game over and shows game over pae
            handleEvents(&quit); //makes the quit or x button quit the game
        }
    // The loop continues until the variable quit becomes true.

    // Inside the loop, it checks the current state of the game (currentState).

    // If the state is MENU, it handles menu events and renders the menu screen.

    // If the state is PLAYING, it handles game events, updates the game state, and renders the game screen.

    // If the state is GAME_OVER, it handles game over events and renders the game over screen.

        SDL_Delay(16); // Approx 60 FPS
        //it will stop the program for 16 milliseconds before returning or running, setting the frame rate to approximately 60 fps
    }

    close();
    //calls the close function
    return 0;
    //closes the main function
}
