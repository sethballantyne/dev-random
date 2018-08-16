#pragma once
#include <cstdio>
#include "math.h"
#include "graphics.h"

// defines for the PLG file format.

// the surface descriptor is 16-bit in the following format
// d15                      d0
//   CSSD | RRRR| GGGG | BBBB

// C is the RGB/indexed color flag
// SS are two bits that define the shading mode
// D is the double sided flag
// and RRRR, GGGG, BBBB are the red, green, blue bits for RGB mode
// or GGGGBBBB is the 8-bit color index for 8-bit mode

// mask to extract RGB or indexed colour
#define PLX_RGB_MASK                    0x8000

// mask to extract shading mode
#define PLX_SHADE_MODE_MASK             0x6000

// mask for double sided
#define PLX_TWOSIDED_MASK                 0x1000

// RGB with 4 bits per channel: XXXXRRRRGGGGBBBB
#define PLX_COLOUR_MASK                 0x0FFF

// polygon uses RGB colour
#define PLX_COLOUR_MODE_RGB_FLAG        0x8000

// polygon uses indexed 8-bit colour.
#define PLX_COLOUR_MODE_INDEXED_FLAG    0x0000

// double sided polygon
#define PLX_TWOSIDED_FLAG  0x1000

// one sided polygon
#define PLX_ONESIDED_FLAG  0x0000

// polygon consists of a constant colour.
// PLX_SHADE_MODE_PURE_FLAG is just an alias
// for *_CONSTANT_FLAG.
#define PLX_SHADE_MODE_PURE_FLAG     0x0000
#define PLX_SHADE_MODE_CONSTANT_FLAG 0x0000

// polygon uses flat shading
#define PLX_SHADE_MODE_FLAT_FLAG     0x2000

// polygon uses gouraud shading.
#define PLX_SHADE_MODE_GOURAUD_FLAG  0x4000

// polygon uses phong shading.
// PLX_SHADE_MODE_PHONG_FLAG and *_FASTPHONG_FLAG mean the same thing.
#define PLX_SHADE_MODE_PHONG_FLAG    0x6000
#define PLX_SHADE_MODE_FASTPHONG_FLAG 0x6000

// reads past comments and blank lines in a PLG file.
// returning the first line it encounters containing model data.
// subsequent calls will return subsequent lines. 
// returns NULL when the file is empty or when the EOF is reached.
char* PLG_GetLine(char* buffer, int maxLength, FILE* fp);

// loads and parses a PLG file, returning it as an instance of OBJECT4DV1.
// scale and rotation optionally allow the caller to manipulate the object to save calls later on.
// obj is zero'd before use.
// returns 0 on error.
int Load_OBJECT4DV1_PLG(SDL_PixelFormat *pixelFormat, OBJECT4DV1* obj, char *filename, VECTOR4D* scale, VECTOR4D* position, VECTOR4D* rotation); 

// computes the average and maximum radius for sent object and opdates the object data
float Compute_OBJECT4DV1_Radius(OBJECT4DV1* obj);

// translates an object without using matrices.
inline void Translate_OBJECT4DV1(OBJECT4DV1* obj, VECTOR4D* translation)
{
    VECTOR4D_Add(&obj->worldPosition, translation, &obj->worldPosition);
}

// scales an object without matrices.
// modifies the objects vertex list and radii.
inline void Scale_OBJECT4DV1(OBJECT4DV1* obj, VECTOR4D* scale)
{
    for(int vertex = 0; vertex < obj->numVertices; vertex++)
    {
        obj->vlistLocalVertices[vertex].x *= scale->x;
        obj->vlistLocalVertices[vertex].y *= scale->y;
        obj->vlistLocalVertices[vertex].z *= scale->z;
        // w is unchanged, always 1.
    }

    // now since the object is scaled we have to do something with 
    // the radii calculation, but we don't know how the scaling
    // factors relate to the original major axis of the object,
    // therefore for scaling factors all ==1 we will simple multiply
    // which is correct, but for scaling factors not equal to 1, we
    // must take the largest scaling factor and use it to scale the
    // radii with since it's the worst case scenario of the new max and
    // average radii

    // find the max scaling factor
    float fscale = MAX(scale->x, scale->y);
    fscale = MAX(fscale, scale->z);

    obj->maximumRadius *= fscale;
    obj->averageRadius *= fscale;
}

// builds up a general local to world transformation matrix that is really nothing more than a translation
// of the origin by the amount specified in vpos
inline void Build_Model_To_World_MATRIX4X4(VECTOR4D* position, MATRIX4X4* output)
{
    Mat_Init_4X4(output, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position->x, position->y, position->z, 1);
}

void Model_To_World_OBJECT4DV1(OBJECT4DV1* object, int transformType);
