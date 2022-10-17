#include <math.h>
#include <err.h>
#include <SDL2/SDL.h>

// Initial width and height of the window.
int WIDTH = 640;
int HEIGHT = 400;

// Number max of iteration for mandelbrot calculation
#define MAX_ITER 64
int ITER = MAX_ITER;
int GAP;

// Verify whether the given number is in the set
int mandelbrot(int Px, int Py);
// Initialize a rect
SDL_Rect * init_rect(int x, int y, int w, int h);
// Draw a square o 1 pixel
void draw_square(SDL_Surface * surface, int x, int y, int m);
// Draw mandlebrot
void draw(SDL_Renderer * renderer, SDL_Surface * surface, int w, int h);
// Loop to verify if an event is trigered
void event_loop(SDL_Renderer * renderer);


int mandelbrot(int Px, int Py) 
{
    double x0 = (double)Px/((double)WIDTH-(double)WIDTH/2) - 1.5;
    double y0 = (double)Py/((double)HEIGHT-(double)HEIGHT/2) - 1;
    int n = 0;
    double tmp;
    double x = 0, y = 0;
    while (x*x + y*y <= 4 && n < ITER)
    {
        tmp = x*x - y*y + x0;;
        y = 2*x*y + y0;
        x = tmp;
        n++;
    }
    return n;
}


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



// Draw a square of 1 pixel
void draw_square(SDL_Surface * surface, int x, int y, int m)
{
    SDL_Rect * rect = init_rect(x,y,1,1);
    Uint8 v = 255 - (m * 255 / ITER);
    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format,v/3,v/3,v));
    free(rect);
}

// Draw squares that verifies that are in the mandelbrot
void draw(SDL_Renderer * renderer, SDL_Surface * surface, int w, int h)
{
    // Clears the renderer (sets the background to black).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++)
            draw_square(surface, x, y, mandelbrot(x, y));

    // Create a Texture to apply on the render
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Applying texture
    SDL_Rect * rect = init_rect(0,0,w,h);
    SDL_RenderCopy(renderer, texture, NULL, rect); 
    free(rect);
    //free(texture);

    // Updates the display.
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
void event_loop(SDL_Renderer* renderer)
{
    // Draws the fractal
    SDL_Surface * surface = SDL_CreateRGBSurface(0,WIDTH,HEIGHT,32,0,0,0,0);
    draw(renderer, surface, WIDTH, HEIGHT);
    
    int last_x = 0;
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
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    WIDTH = event.window.data1;
                    HEIGHT = event.window.data2;
                    draw(renderer, surface, WIDTH, HEIGHT);
                }
                break;
            case SDL_MOUSEMOTION :
                GAP = WIDTH/10;
                if (GAP + last_x < event.motion.x || last_x - GAP > event.motion.x)
                {
                    last_x = event.motion.x;
                    ITER = (int) ((double)MAX_ITER * ((double) event.motion.x + 1.0) / WIDTH);
                    draw(renderer, surface, WIDTH, HEIGHT);
                }
        }
    }
}

int main()
{
    // Initializes the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // Creates a window.
    SDL_Window* window = SDL_CreateWindow("Mandelbrot", 0, 0, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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
