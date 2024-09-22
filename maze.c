#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define BALL_RADIUS 10
#define MOVE_STEP 5
#define MAZE_EXIT_X 600

void drawMaze(SDL_Renderer *renderer);
void drawBall(SDL_Renderer *renderer, int x, int y);
void displayInstructions(int moves);
void updateBallPosition(int *x, int *y, char direction);
void checkCollision(int *x, int *y);

void drawMaze(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);


    SDL_RenderDrawLine(renderer, 0, 100, 450, 100);
    SDL_RenderDrawLine(renderer, 50, 130, 400, 130);
    SDL_RenderDrawLine(renderer, 100, 160, 450, 160);
    SDL_RenderDrawLine(renderer, 150, 190, 300, 190);
    SDL_RenderDrawLine(renderer, 100, 220, 250, 220);
    SDL_RenderDrawLine(renderer, 50, 250, 200, 250);
    SDL_RenderDrawLine(renderer, 0, 400, 500, 400);
    SDL_RenderDrawLine(renderer, 450, 370, 500, 370);

    SDL_RenderDrawLine(renderer, 0, 100, 0, 400);
    SDL_RenderDrawLine(renderer, 50, 130, 50, 370);
    SDL_RenderDrawLine(renderer, 100, 160, 100, 220);
    SDL_RenderDrawLine(renderer, 100, 280, 100, 400);
    SDL_RenderDrawLine(renderer, 150, 250, 150, 370);
    SDL_RenderDrawLine(renderer, 200, 280, 200, 400);
    SDL_RenderDrawLine(renderer, 250, 220, 250, 370);
    SDL_RenderDrawLine(renderer, 300, 190, 300, 400);
    SDL_RenderDrawLine(renderer, 350, 160, 350, 370);
    SDL_RenderDrawLine(renderer, 400, 190, 400, 400);
    SDL_RenderDrawLine(renderer, 450, 100, 450, 370);
}

void drawBall(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
    for (int w = 0; w < BALL_RADIUS * 2; w++) {
        for (int h = 0; h < BALL_RADIUS * 2; h++) {
            int dx = BALL_RADIUS - w; 
            int dy = BALL_RADIUS - h; 
            if ((dx * dx + dy * dy) <= (BALL_RADIUS * BALL_RADIUS)) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void displayInstructions(int moves) {
    printf("Moves: %d\n", moves); 
}

void updateBallPosition(int *x, int *y, char direction) {
    switch (direction) {
        case 'w': case 'W': 
            *y -= MOVE_STEP;
            break;
        case 's': case 'S': 
            *y += MOVE_STEP;
            break;
        case 'a': case 'A': 
            *x -= MOVE_STEP;
            break;
        case 'd': case 'D': 
            *x += MOVE_STEP;
            break;
        default:
            break;
    }
}

void checkCollision(int *x, int *y) {
    if (*x < 0) *x = 0;
    if (*x > SCREEN_WIDTH) *x = SCREEN_WIDTH;
    if (*y < 0) *y = 0;
    if (*y > SCREEN_HEIGHT) *y = SCREEN_HEIGHT;
}

int main(int argc, char *args[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Maze Runner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int x = 20, y = 250; 
    int moves = 0;
    SDL_bool quit = SDL_FALSE;

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = SDL_TRUE;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                    case SDLK_UP:
                        updateBallPosition(&x, &y, 'w');
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        updateBallPosition(&x, &y, 's');
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        updateBallPosition(&x, &y, 'a');
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        updateBallPosition(&x, &y, 'd');
                        break;
                    case SDLK_SPACE:
                        quit = SDL_TRUE;
                        break;
                }
                checkCollision(&x, &y);
                moves++;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawMaze(renderer);
        drawBall(renderer, x, y);
        displayInstructions(moves);

        SDL_RenderPresent(renderer);

        if (x >= MAZE_EXIT_X) {
            printf("You Escaped the Maze!\n");
            quit = SDL_TRUE;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
