#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define TOP_LEVEL 16

// Recursive function that draws the fractal canopy.
//
// renderer: Renderer to draw on.
// x: Abscissa of the starting point of the segments.
// y: Ordinate of the starting point of the segments.
// len: Length of the segments.
// a: Angle use to rotate the segments.
// level: Recursion level (0 = first iteration).
void v(SDL_Renderer* renderer, int x, int y, int z, int t, int level)
{
    // Trace
    if (level == 0)
        SDL_RenderDrawLine(renderer, x, y, z, t);
    // Divide the current segment into 2 parts.
    else
    {
        int m = (x+z)/2 + (t-y)/2;
        int u = (y+t)/2 - (z-x)/2;
        v(renderer, x, y, m, u, level-1);
        v(renderer, m, u, z, t, level-1);
    }
}

// Initializes the renderer, draws the fractal canopy and updates the display.
//
// renderer: Renderer to draw on.
// w: Current width of the window.
// h: Current height of the window.
void draw(SDL_Renderer* renderer, int w, int h, int level)
{

    // If the width or the height is too small, we do not draw anything.
    if (w < 20 || h < 20)
        return;

    // Clears the renderer (sets the background to black).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Sets the color for drawing operations to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draws the fractal
    v(renderer, w / 4, 2*h/3, 3*w/4, 2*h/3, level > TOP_LEVEL ? TOP_LEVEL : level);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer, int level)
{
    // Width and height of the window.
    int w = 800;
    int h = 900;

    // Draws the fractal canopy (first draw).
    draw(renderer, w, h, level);

    // Creates a variable to get the events.
    SDL_Event event;

    while (1)
    {
        // Waits for an event.
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    w = event.window.data1;
                    h = event.window.data2;
                    draw(renderer, w, h, level);
                }
                break;
        }
    }
}

int main(int argc, char * argv[])
{
    // Randomize
    srand(time(NULL));

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Static Dragon", 0, 0, 500, 500,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Dispatches the events.
    if (argc == 2)
        event_loop(renderer, atoi(argv[1]));
    else
        event_loop(renderer, 10);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
