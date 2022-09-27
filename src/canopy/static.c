#include <err.h>
#include <SDL2/SDL.h>

// Initial width and height of the window.
const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

// Ratio used to reduce the length of a segment.
const double RATIO = 0.7;

// Maximum recursion level.
const int TOP_LEVEL = 10;

// Step angle to rotate a segment.
const double STEP_ANGLE = M_PI / 6;

// Recursive function that draws the fractal canopy.
//
// renderer: Renderer to draw on.
// x: Abscissa of the starting point of the segments.
// y: Ordinate of the starting point of the segments.
// len: Length of the segments.
// a: Angle use to rotate the segments.
// level: Recursion level (0 = first iteration).
void v(SDL_Renderer* renderer, int x, int y, double len, double a, int level)
{
    // If level is 0, draws a single segment.
    if (level == 0)
    {
        // Gets the ordinate of the end of the segment.
        int y1 = y - len;

        // Draws the segment.
        SDL_RenderDrawLine(renderer, x, y, x, y1);

        // Draws the segments for the next level.
        v(renderer, x, y1, RATIO * len, 0, 1);
    }

    // Otherwise, draws all the other segments.
    else if (level <= TOP_LEVEL)
    {

        int x1 = x - len * sin(a + STEP_ANGLE);
        int y1 = y - len * cos(a + STEP_ANGLE);

        int x2 = x - len * sin(a - STEP_ANGLE); 
        int y2 = y - len * cos(a - STEP_ANGLE);

        SDL_RenderDrawLine(renderer, x, y, x1, y1);
        SDL_RenderDrawLine(renderer, x, y, x2, y2);

        v(renderer, x1, y1, len * RATIO, a + STEP_ANGLE, level+1);
        v(renderer, x2, y2, len * RATIO, a - STEP_ANGLE, level+1);
    }
}

// Initializes the renderer, draws the fractal canopy and updates the display.
//
// renderer: Renderer to draw on.
// w: Current width of the window.
// h: Current height of the window.
void draw(SDL_Renderer* renderer, int w, int h)
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
    v(renderer, w / 2, h, h / 4, 0, 0);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer)
{
    // Width and height of the window.
    int w = INIT_WIDTH;
    int h = INIT_HEIGHT;

    // Draws the fractal canopy (first draw).
    draw(renderer, w, h);

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
                    draw(renderer, w, h);
                }
                break;
        }
    }
}

int main()
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Static Fractal Canopy", 0, 0, INIT_WIDTH, INIT_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a renderer.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Dispatches the events.
    event_loop(renderer);

    // Destroys the objects.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
