#pragma once
#include <SDL.h>
#include <string>
#include "math.h"

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

#define POLYGON4DV1_ATTR_TWOSIDED           0x0001
#define POLYGON4DV1_ATTR_TRANSPARENT        0x0002
#define POLYGON4DV1_ATTR_8BITCOLOUR         0x0004
#define POLYGON4DV1_ATTR_RGB16              0x0008
#define POLYGON4DV1_ATTR_RGB24              0x0010

#define POLYGON4DV1_ATTR_SHADE_MODE_PURE       0x0020
#define POLYGON4DV1_ATTR_SHADE_MODE_CONSTANT   0x0020 // (alias)
#define POLYGON4DV1_ATTR_SHADE_MODE_FLAT       0x0040
#define POLYGON4DV1_ATTR_SHADE_MODE_GOURAUD    0x0080
#define POLYGON4DV1_ATTR_SHADE_MODE_PHONG      0x0100
#define POLYGON4DV1_ATTR_SHADE_MODE_FASTPHONG  0x0100 // (alias)
#define POLYGON4DV1_ATTR_SHADE_MODE_TEXTURE    0x0200 

// states of polygons and faces
#define POLYGON4DV1_STATE_ACTIVE             0x0001
#define POLYGON4DV1_STATE_CLIPPED            0x0002
#define POLYGON4DV1_STATE_BACKFACE           0x0004
struct POLYGON4DV1
{
    // polygon state information
    int state;

    // physical attributes of the polygon
    int attributes;

    // the colour of the polygon
    int colour;

    // list of vertices that make up the polygon.
    POINT4D* vertexList;

    // indices for use with vertexList
    int vertices[3];
};

struct POLYGONF4DV1
{
    // state information
    int state;

    // physical attributes of the polygon
    int attributes;

    // colour of the polygon
    int colour;

    // the triangles vertices
    POINT4D vertexList[3];

    // the vertices after they've been transformed
    POINT4D transformedVertexList[3];

    // pointer to the next polygon in the list
    POLYGONF4DV1* next;

    // pointer to the previous polygon in the list
    POLYGONF4DV1* previous;
};

// defines for objects version 1
#define OBJECT4DV1_MAX_VERTICES           1024  // 64
#define OBJECT4DV1_MAX_POLYS              1024 // 128

// states for objects
#define OBJECT4DV1_STATE_ACTIVE           0x0001
#define OBJECT4DV1_STATE_VISIBLE          0x0002 
#define OBJECT4DV1_STATE_CULLED           0x0004

// an object based on a vertex list and a list of polygons
struct OBJECT4DV1
{
    // objects numeric ID
    int id;

    // ASCII name of the object
    char name[64];

    // object state
    int state;

    // objects attributes
    int attributes;

    // the objects average radius; used for collision detection.
    float averageRadius;

    // the objects maximum radius.
    float maximumRadius;

    // the objects position within the world
    POINT4D worldPosition;

    // rotation angle of the object in local coordinates.
    VECTOR4D direction;

    // local axes to track full orientation of the object
    VECTOR4D ux;
    VECTOR4D uy;
    VECTOR4D uz;

    // the number of vertices the object has
    int numVertices;

    // the objects local vertices
    POINT4D vlistLocalVertices[OBJECT4DV1_MAX_VERTICES];

    // the objects transformed vertices
    POINT4D vlistTransformedVertices[OBJECT4DV1_MAX_VERTICES];

    // the number of polygons in the objects mesh.
    int numPolygons;

    // the objects polygons.
    POLYGON4DV1 plist[OBJECT4DV1_MAX_POLYS];
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
    /*if(y >= surface->h)
        return;*/

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