#include <err.h>
#include <SDL2/SDL.h>

#define MIN(a, b) ( ( (a) < (b) ) ? (a) : (b) )
#define MAX(a, b) ( ( (a) > (b) ) ? (a) : (b) )
#define DIM(x, a, b) ( MIN(MAX(x, a), b) )

// Initial width and height of the window.
const int INIT_WIDTH = 640;
const int INIT_HEIGHT = 400;

// Initial (fake) position of the mouse pointer.
const int INIT_MOUSE_X = INIT_WIDTH / 10;
const int INIT_MOUSE_Y = INIT_HEIGHT;

// Ratio used to reduce the length of a segment.
const double RATIO = 0.7;

// Recursive function that draws the fractal canopy.
//
// renderer: Renderer to draw on.
// x: Abscissa of the starting point of the segments.
// y: Ordinate of the starting point of the segments.
// len: Length of the segments.
// a: Angle use to rotate the segments.
// level: Recursion level (0 = first iteration).
// top_level: Maximum level for recursion.
// step_angle: Angle used to rotate the left- and right-angled segments.
void v(SDL_Renderer* renderer, int x, int y, double len, double a, int level, int top_level, double step_angle)
{
    if (level == 0)
    {
        int y1 = y - len;
        SDL_RenderDrawLine(renderer, x, y, x, y1);
        v(renderer, x, y1, len, a, level+1, top_level, step_angle);
    }
    else if (level <= top_level)
    {
        // Getting locations
        int x1 = x - len * sin(a + step_angle);
        int y1 = y - len * cos(a + step_angle);
        int x2 = x - len * sin(a - step_angle);
        int y2 = y - len * cos(a - step_angle);

        // Drawing stuff
        SDL_RenderDrawLine(renderer, x, y, x1, y1);
        SDL_RenderDrawLine(renderer, x, y, x2, y2);

        // Recursion call
        v(renderer, x1, y1, len * RATIO, a + step_angle, level+1, top_level, step_angle);
        v(renderer, x2, y2, len * RATIO, a - step_angle, level+1, top_level, step_angle);
    }
}

// Initializes the renderer, draws the fractal canopy and updates the display.
//
// renderer: Renderer to draw on.
// w: Current width of the window.
// h: Current height of the window.
// mouse_x: Abscissa of the mouse cursor.
// mouse_y: Ordinate of the mouse cursor.
void draw(SDL_Renderer* renderer, int w, int h, int mouse_x, int mouse_y)
{

    // If the width or the height is too small, we do not draw anything.
    if (w < 20 || h < 20)
        return;

    // Fill BG with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set color to white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Getting top_level and step_angle
    int top_level = DIM(mouse_y / (h/11), 0, 10);
    double step_angle = DIM(M_PI / (2.00 + mouse_x / (w / 18.0)), M_PI / 20, M_PI / 2);
    // Call recursive function to draw
    v(renderer, w/2, h, (double) h/4, 0, 0, top_level, step_angle);

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

    // (Fake) position of the mouse cursor.
    int mouse_x = INIT_MOUSE_X;
    int mouse_y = INIT_MOUSE_Y;

    // Draws the fractal canopy (first draw).
    draw(renderer, w, h, mouse_x, mouse_y);

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
                    draw(renderer, w, h, mouse_x, mouse_y);
                }
                break;

            // If the mouse is moving, updates the position of the cursor.
            case SDL_MOUSEMOTION:
                mouse_x = event.motion.x;
                mouse_y = event.motion.y;
                draw(renderer, w, h, mouse_x, mouse_y);
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
    SDL_Window* window = SDL_CreateWindow("Dynamic Fractal Canopy", 0, 0, INIT_WIDTH, INIT_HEIGHT,
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
