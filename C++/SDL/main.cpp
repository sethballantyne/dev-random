
#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "math.h"

int main(int argc, char *argv[])
{
    Math_BuildLookupTables();

    SDL_Surface *screen;

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    screen = SDL_SetVideoMode(800, 600, 16, SDL_HWACCEL | SDL_DOUBLEBUF);
    if(!screen)
    {
        printf("failed to create screen mode.\n");
        return 1;
    }

    SDL_Surface *asteroidSurface = SDL_CreateRGBSurface(SDL_HWSURFACE, screen->w, screen->h, screen->format->BitsPerPixel, 0, 0, 0, 0);
    bool quit = false;
    SDL_Event event;
    Uint32 colour;
    POLYGON2D asteroids[64];
    VERTEX2DF asteroid_vertices[8];
    asteroid_vertices[0].x = 33.0f;
    asteroid_vertices[0].y = -3.0f;
    asteroid_vertices[1].x = 9.0f;
    asteroid_vertices[1].y = -18.0f;
    asteroid_vertices[2].x = -12.0f;
    asteroid_vertices[2].y = -9.0f;
    asteroid_vertices[3].x = -21.0f;
    asteroid_vertices[3].y = -12.0f;
    asteroid_vertices[4].x = -9.0f;
    asteroid_vertices[4].y = 6.0f;
    asteroid_vertices[5].x = -15.0f;
    asteroid_vertices[5].y = 15.0f;
    asteroid_vertices[6].x = -3.0f;
    asteroid_vertices[6].y = 27.0f;
    asteroid_vertices[7].x = 21.0f;
    asteroid_vertices[7].y = 21.0f;
    
    for(int curr_index = 0; curr_index < 64; curr_index++)
    {
        //SetCursorPos(400, 600);
        // initialize the asteroid
        asteroids[curr_index].isVisible = true;   // turn it on
        asteroids[curr_index].numberOfVertices = 8;
        asteroids[curr_index].xPositionCenter = 400; // position it
        asteroids[curr_index].yPositionCenter = 300;
        asteroids[curr_index].xVelocity = 1 + rand() % 5;
        asteroids[curr_index].yVelocity = 1 + rand() % 5;
        asteroids[curr_index].colour = SDL_MapRGB(screen->format, rand() % 256, rand() % 256, rand() % 256);
        asteroids[curr_index].vertexList = new VERTEX2DF[asteroids[curr_index].numberOfVertices];

        for(int index = 0; index < asteroids[curr_index].numberOfVertices; index++)
            asteroids[curr_index].vertexList[index] = asteroid_vertices[index];
        int scale = rand() % 5;

        Graphics_ScalePolygon2D(&asteroids[curr_index], 0.35, 0.35);

    } // end for curr_index
    while(!quit)
    {

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_UP:
                            Graphics_ScalePolygon2D(&asteroids[0], 1.1, 1.1);
                            break;

                        case SDLK_DOWN:
                            Graphics_ScalePolygon2D(&asteroids[0], 0.9, 0.9);
                            break;

                        case SDLK_LEFT:
                            Graphics_RotatePolygon2D(&asteroids[0], -2);
                            break;

                        case SDLK_RIGHT:
                            Graphics_RotatePolygon2D(&asteroids[0], 2);
                            break;

                        default:
                            break;
                    }
                 
                    break;
                default:
                    break;

            }
        }
        // convert the image to the format used by the display.
        // this is done after the colour key is set because colour key blits
        // can sometimes be optimized for a particular display. 
        SDL_Delay(30);
        Graphics_RotatePolygon2D(&asteroids[0], -2);
        if(SDL_MUSTLOCK(screen))
        {
            if(SDL_LockSurface(screen) < 0)
            {
                printf("Failed to lock screen.");
                return 1;
            }
        }
        
        //Graphics_DrawPixel(screen, x, y, yellow);
        //colour = Graphics_GetPixel(screen, x, y);
        //Graphics_DrawClippedLine(screen, -100, 0, 300, 200, SDL_MapRGB(screen->format, 255, 0, 0));
        Graphics_ClearSurface(asteroidSurface, 0, 0, 0);
        Graphics_DrawPolygon2D(asteroidSurface, &asteroids[0]);
        if(SDL_MUSTLOCK(screen))
        {
            SDL_UnlockSurface(screen);
        }

        Graphics_RenderSurface(screen,asteroidSurface, 0, 0);
        // swap the back buffer to the screen.
        SDL_Flip(screen);
    }

    // values will be different if 16bit screen resolution is used.
    // only 32 bit screen resolution will always provide the exact expected
    // RGB values.
    Uint32 pixel = Graphics_GetPixel(screen, 16, 7);
    Uint8 R, G, B, A;
    Graphics_Uint32ToRGBA(screen, pixel, R, G, B, A);
    printf("R: %d G: %d B: %d A: %d", (Uint8) R, G, B, A);
    SDL_Delay(3000);
    return 0;
}