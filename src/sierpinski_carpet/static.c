#include <err.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>

#define TOP_LEVEL 12

// Initialize a rect
SDL_Rect * init_rect(int x, int y, int w, int h)
{
    SDL_Rect * rect = calloc(1, sizeof(SDL_Rect));
    if (!rect)
        errx(EXIT_FAILURE, "Unable to generate a rectangle");

    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;

    return rect;
}

// Recursive function that draws the fractal canopy.
//
// renderer: Renderer to draw on.
// x: Abscissa of the starting point of the segments.
// y: Ordinate of the starting point of the segments.
// w: width of the carpet
// h: height of the carpet
// level: Recursion level (0 = last iteration).
void v(SDL_Surface * surface, int x, int y, int w, int h, int level)
{
    // Trace
    if (level == 0 || w <= 3 || h <= 3)
    {
        SDL_Rect * rect = init_rect(x,y,w,h);
        SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, 255, 255, 255)),
        free(rect);
    }
    // Divide the current segment into 2 parts.
    else
    {
        v(surface, x/3, y/3, w/3, h/3, level-1);
        v(surface, 2*x/3, y/3, w/3, h/3, level-1);
        v(surface, x, y/3, w/3, h/3, level-1);

        v(surface, x/3, 2*y/3, w/3, h/3, level-1);
        v(surface, 2*x/3, 2*y/3, w/3, h/3, level-1);
        v(surface, x, 2*y/3, w/3, h/3, level-1);

        v(surface, x/3, y, w/3, h/3, level-1);
        v(surface, 2*x/3, y, w/3, h/3, level-1);
    }
}

// Initializes the renderer, draws the fractal canopy and updates the display.
//
// renderer: Renderer considered
// surface : the surface to draw on.
// w: Current width of the window.
// h: Current height of the window.
void draw(SDL_Renderer* renderer, SDL_Surface * surface, int w, int h, int level)
{
    // If the width or the height is too small, we do not draw anything.
    if (w < 20 || h < 20)
        return;

    // Clears the renderer (sets the background to black).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Sets the color for drawing operations to white.
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draws the fractal canopy. 
    v(surface, w / 4, h/2, 3*w/4, h/2, level > TOP_LEVEL ? TOP_LEVEL : level);

    // Create a Texture to apply on the render
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Applying texture
    SDL_Rect * rect = init_rect(0,0,w,h);
    SDL_RenderCopy(renderer, texture, NULL, rect); 
    free(rect);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer, int level)
{
    // Width and height of the window.
    int w = 500;
    int h = 500;

    SDL_Surface * surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (!surface)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Draws the fractal canopy (first draw).
    draw(renderer, surface, w, h, level);

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
                    SDL_FreeSurface(surface);
                    surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
                    draw(renderer, surface, w, h, level);
                }
                break;
        }
    }
    SDL_FreeSurface(surface);
}

int main(int argc, char * argv[])
{
    // Randomize
    srand(time(NULL));

    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Static Mountain", 0, 0, 500, 500,
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
