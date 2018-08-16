#pragma once
#include <SDL.h>
#include <string>
#include "math.h"

// perform the transformation in place on the local/world vertex list 
#define TRANSFORM_LOCAL_ONLY       0 

// perfrom the transformation in place on the "transformed" vertex list
#define TRANSFORM_TRANS_ONLY       1  

// perform the transformation to the local vertex list, but store the results in the
// transformed vertex list
#define TRANSFORM_LOCAL_TO_TRANS   2  

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

// defines for objects version 1
#define OBJECT4DV1_MAX_VERTICES           1024  // 64
#define OBJECT4DV1_MAX_POLYS              1024 // 128

// states for objects
#define OBJECT4DV1_STATE_ACTIVE           0x0001
#define OBJECT4DV1_STATE_VISIBLE          0x0002 
#define OBJECT4DV1_STATE_CULLED           0x0004a

#define RENDERLIST4DV1_MAX_POLYGONS          32768

// general culling flags
#define CULL_OBJECT_X_PLANE           0x0001 // cull on the x clipping planes
#define CULL_OBJECT_Y_PLANE           0x0002 // cull on the y clipping planes
#define CULL_OBJECT_Z_PLANE           0x0004 // cull on the z clipping planes
#define CULL_OBJECT_XYZ_PLANES        (CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE)

#define CAMERA_ROTATE_SEQUENCE_XYZ  0
#define CAMERA_ROTATE_SEQUENCE_YXZ  1
#define CAMERA_ROTATE_SEQUENCE_XZY  2
#define CAMERA_ROTATE_SEQUENCE_YZX  3
#define CAMERA_ROTATE_SEQUENCE_ZYX  4
#define CAMERA_ROTATE_SEQUENCE_ZXY  5

// defines for special types of camera projections
#define CAMERA_PROJECTION_NORMALIZED        0x0001
#define CAMERA_PROJECTION_SCREEN            0x0002
#define CAMERA_PROJECTION_FOV90             0x0004

#define CAMERA_MODEL_EULER            0x0008
#define CAMERA_MODEL_UVN              0x0010

#define UVN_MODE_SIMPLE            0 
#define UVN_MODE_SPHERICAL         1

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

struct RENDERLIST4DV1
{
    // render list state.
    int state;

    // attributes of the render list
    int attributes;

    // each pointer points to a self containd "renderable" polygon face
    POLYGONF4DV1* polygonPointers[RENDERLIST4DV1_MAX_POLYGONS];

    // where the actual polygon faces are stored
    POLYGONF4DV1* polygonData[RENDERLIST4DV1_MAX_POLYGONS];

    // the number of polygons in the render list
    int numPolygons;
};

struct CAMERA4DV1
{
    int state;
    int attributes;
    POINT4D position; // world position of the camera used by both camera models.
    VECTOR4D direction; // angles the look at direction of the camera for simple euler camera models.
    // Also the elevation and heading of the uvn model.
    // uvn used to track camera orientation
    VECTOR4D u;
    VECTOR4D v;
    VECTOR4D n;

    VECTOR4D target; // the look at target.
    float viewDistance; // focal length
    float fieldOfView; // FOV for both the horizontal and vertical axes
    float nearZClippingPlane; // near Z constant clipping plane
    float farZClippingPlane; // far Z constant clipping plane

    PLANE3D rightClippingPlane;
    PLANE3D leftClippingPlane;
    PLANE3D topClippingPlane;
    PLANE3D bottomClippingPlane;

    // the width and height of the view plane to project onto.
    // Usually 2x2 for normalised projection or the same size as the viewport
    // or screen window.
    float viewPlaneWidth;
    float viewPlaneHeight;

    // screen and viewport are synomomous
    float viewportWidth;
    float viewportHeight;
    float viewportCenterX; // x position of the viewports center
    float viewportCenterY; // y position of the viewports center

    float aspectRatio;

    MATRIX4X4 worldToCameraTransform;
    MATRIX4X4 cameraToPerspectiveTransform;
    MATRIX4X4 perspectiveToScreenTransform;
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

// transforms all of the vertices in the local or transformed array by the sent matrix
void Transform_OBJECT4DV1(OBJECT4DV1* object, MATRIX4X4* transformationMatrix, int selectedCoordinates, bool transformOrientation);

// rotates an object parallel to the XYZ axes in that order.
void Rotate_XYZ_OBJECT4DV1(OBJECT4DV1* object, float thetaX, float thetaY, float thetaZ);

// transforms the world coordinates of an object into camera coordinates.
// based on the sent camera matrix but it totally disregards the polygons themselves
// it only works on the vertices in the object->vlistTransformedVertices list.
void World_To_Camera_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera);

// builds up a camera to perspective transformation
// matrix, in most cases the camera would have a 2x2 normalized
// view plane with a 90 degree FOV, since the point of the having
// this matrix must be to also have a perspective to screen (viewport)
// matrix that scales the normalized coordinates, also the matrix
// assumes that you are working in 4D homogenous coordinates and at 
// some point there will be a 4D->3D conversion, it might be immediately
// after this transform is applied to vertices, or after the perspective
// to screen transform
inline void Build_Camera_To_Perspective_MATRIX4X4(CAMERA4DV1* camera, MATRIX4X4* output)
{
    float temp = camera->viewDistance * camera->aspectRatio;
    Mat_Init_4X4(output, camera->viewDistance, 0, 0, 0, 0, temp, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0);
}

// builds up a perspective to screen transformation matrix.
inline void Build_Persepective_To_Screen_4D_MATRIX4X4(CAMERA4DV1* camera, MATRIX4X4* output)
{
    // assumes that you want to perform the
    // transform in homogeneous coordinates and at raster time there will be 
    // a 4D->3D homogenous conversion.
    float alpha = ((0.5f * camera->viewportWidth) - 0.5f);
    float beta = ((0.5f * camera->viewportHeight) - 0.5f);

    Mat_Init_4X4(output, alpha, 0, 0, 0, 0, -beta, 0, 0, alpha, beta, 1, 0, 0, 0, 0, 1);
}

// creates a single matrix that performs the entire camera->perspective->screen transform. 
// Note that the camera must be created with a viewplane specified to be the size of the viewport.
// After this transform is applied the vertex must be converted from 4D homogeneous coordinates to 3D coordinates.
// The z is irrelevant since the data would be used for the screen, but still the division by w is needed no matter
// what.
inline void Build_Camera_To_Screen_MATRIX4X4(CAMERA4DV1* camera, MATRIX4X4* output)
{
    float alpha = ((0.5f * camera->viewportWidth) - 0.5f);
    float beta = ((0.5f * camera->viewportHeight) - 0.5f);

    Mat_Init_4X4(output, camera->viewDistance, 0, 0, 0, 0, -camera->viewDistance, 0, 0, alpha, beta, 1, 1, 0, 0, 0, 0);
}

// culls an entire object from the viewing frustrum. cullFlags determines on which axes the culling takes place.
// cullFlags can consists of one or more or'd CULL_OBJECT_*_PLANE defines.
int Cull_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera, int cullFlags);

// removes the backfaces from an object's polygon mesh. It does this based on the vertex
// data in object->vlistTransformedVertices[] along with the camera position (only).
// note that only the backface state is set in each polygon
void Remove_Backfaces_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera);

// flags the backface polygons present in the specified renderList.
void Remove_Backfaces_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera);