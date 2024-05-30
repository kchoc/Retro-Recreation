#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>

using namespace std;

int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 630;

int GRID_SIZE = 25;

int SNAKE_SPEED = 100;
int SNAKE_LENGTH = 6;
int SNAKE_SIZE = SCREEN_WIDTH / GRID_SIZE;

int TICK_INTERVAL = 200;

int x = GRID_SIZE / 2;
int y = GRID_SIZE / 2;


int direction = 2;

struct Location {
    int x, y;
};

vector<Location> snakeSegments;
Location food;

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if (next_time <= now)
        return 0;
    else
        return next_time - now;
}

void GenerateNewFood() {
    generateFood:
        food = { rand() % GRID_SIZE, rand() % GRID_SIZE };
        for (Location& segment : snakeSegments) {
            if (segment.x == food.x and segment.y == food.y) goto generateFood;
        }
        return;
}

void ChangeDirection(int newDirection) {
    if (direction % 2 != newDirection % 2) direction = newDirection;
}

bool CheckCollision() {
    if (x < 0 or x > GRID_SIZE - 1 or y < 0 or y > GRID_SIZE - 1)
        return true;

    for (Location& segment : snakeSegments) {
        if (segment.x == x and segment.y == y) {
            return true;
        }
    }
    return false;
}

bool HandleInputs() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_UP:    ChangeDirection(0); return true;
            case SDLK_RIGHT: ChangeDirection(1); return true;
            case SDLK_DOWN:  ChangeDirection(2); return true;
            case SDLK_LEFT:  ChangeDirection(3); return true;
            }
        }
    }
    return true;
}

void ResetBoard() {
    SNAKE_LENGTH = 6;
    x = GRID_SIZE / 2;
    y = GRID_SIZE / 2;

    snakeSegments.clear();
    for (int i = 0; i < SNAKE_LENGTH; i++)
        snakeSegments.push_back({ x, y });

    GenerateNewFood();
}

void Move() {

    switch (direction) {
    case 0: y -= 1; break;
    case 1: x += 1; break;
    case 2: y += 1; break;
    case 3: x -= 1; break;
    }

    if (food.x == x and food.y == y)
    {
        GenerateNewFood(); 
        SNAKE_LENGTH++;
    }
    else
        snakeSegments.erase(snakeSegments.begin());


    if (CheckCollision()) ResetBoard();

    snakeSegments.push_back({ x, y });
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    next_time = SDL_GetTicks() + TICK_INTERVAL;

    ResetBoard();

    while (HandleInputs()) {
        Move();

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0x8f, 0x91, 0x62, 0xFF);
        SDL_Rect fillRect = { 0, 0, SCREEN_WIDTH, 30 };
        SDL_RenderFillRect(renderer, &fillRect);

        SDL_SetRenderDrawColor(renderer, 0x94, 0x00, 0x00, 0xFF);
        SDL_Rect foodRect = { food.x * SNAKE_SIZE + 5, food.y * SNAKE_SIZE + 35, SNAKE_SIZE - 10, SNAKE_SIZE - 10 };
        SDL_RenderFillRect(renderer, &foodRect);

        int i = 0;
        for (Location& segment : snakeSegments) {
            if (i%2) SDL_SetRenderDrawColor(renderer, 0x00, 0x80, 0x00, 0xFF);
            else SDL_SetRenderDrawColor(renderer, 0x4C, 0xBB, 0x17, 0xFF);
            SDL_Rect fillRect = { segment.x * SNAKE_SIZE, segment.y * SNAKE_SIZE + 30, SNAKE_SIZE, SNAKE_SIZE };
            SDL_RenderFillRect(renderer, &fillRect);
            i++;
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
    }

    theEnd:
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    return 0;
}