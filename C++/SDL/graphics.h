#pragma once
#include <SDL.h>
#include <string>

struct VERTEX2DF
{
    float x = 0.0f;
    float y = 0.0f; 
};

struct POLYGON2D
{
    bool isVisible = true;        // state of polygon
    int numberOfVertices = 0;  // number of vertices
    int xPositionCenter = 0;
    int yPositionCenter = 0;   // position of center of polygon  
    int xVelocity = 0;
    int yVelocity = 0;    // initial velocity
    Uint32 colour = 0;      // could be index or PALETTENTRY
    VERTEX2DF *vertexList = nullptr; // pointer to vertex list
};

// returns the pixel colour at the specified coordinate
inline Uint32 Graphics_GetPixel(SDL_Surface *surface, int x, int y)
{
    Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
        case 2:
            return *(Uint16 *)pixel;

        case 4:
            return *(Uint32 *)pixel;

        default:
            printf("invalid bpp");
            return 0;

    }
}

// Draws a pixel of the specified colour on the specified surface
inline void Graphics_DrawPixel(SDL_Surface *surface, int x, int y, Uint32 colour)
{
    // crashes without this
    if(y >= surface->h)
        return;

    Uint8 *pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;

    switch(surface->format->BytesPerPixel)
    {
        case 2:
            *(Uint16 *)pixel = colour;
            break;
        case 4:
            *(Uint32 *)pixel = colour;
            break;
        default:
            printf("unsupported bpp.");
            break;
    }
}

inline void Graphics_Uint32ToRGBA(SDL_Surface *surface, Uint32 pixel, Uint8& R, Uint8& G, Uint8& B, Uint8& A)
{
    Uint32 itemp = pixel & surface->format->Rmask;
    itemp = itemp >> surface->format->Rshift;
    itemp = itemp << surface->format->Rloss;
    R = (Uint8)itemp;

    itemp = pixel & surface->format->Gmask;
    itemp = itemp >> surface->format->Gshift;
    itemp = itemp << surface->format->Gloss;
    G = (Uint8)itemp;

    itemp = pixel & surface->format->Bmask;
    itemp = itemp >> surface->format->Bshift;
    itemp = itemp << surface->format->Bloss;
    B = (Uint8)itemp;

    itemp = pixel & surface->format->Amask;
    itemp = itemp >> surface->format->Ashift;
    itemp = itemp << surface->format->Aloss;
    A = (Uint8)itemp;
}

inline void Graphics_Uint32ToRGB(SDL_Surface *surface, Uint32 pixel, Uint8& R, Uint8& G, Uint8& B)
{
    Uint8 A;
    Graphics_Uint32ToRGBA(surface, pixel, R, G, B, A);
}

int Graphics_ClipLine(int& x1, int& y1, int& x2, int& y2, int regionX, int regionY, int regionX2, int regionY2);

int Graphics_DrawClippedLine(SDL_Surface *surface, int x, int y, int x2, int y2, Uint32 colour);

// Scales the specified polygon
void Graphics_ScalePolygon2D(POLYGON2D *polygon, float xScale, float yScale);

// Rotates the specified 2D Polygon in degrees
void Graphics_RotatePolygon2D(POLYGON2D *polygon, int theta);

// 
int Graphics_DrawPolygon2D(SDL_Surface *surface, POLYGON2D* polygonList);

SDL_Surface* Graphics_LoadImage(std::string filename, Uint8 R, Uint8 G, Uint8 B);

void Graphics_RenderSurface(SDL_Surface *screen, SDL_Surface *image, Uint32 x, Uint32 y);

void Graphics_FillRect(SDL_Surface *surface, int fromX, int fromY, int toX, int toY, Uint8 R, Uint8 G, Uint8 B);

void Graphics_ClearSurface(SDL_Surface *surface, Uint8 R, Uint8 G, Uint8 B);