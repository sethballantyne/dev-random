#include <algorithm>
#include <SDL_image.h>
#include "graphics.h"

extern float g_SineLookupTable[361];
extern float g_CosineLookupTable[361];

int debugPolygonsRenderedPerFrame = 0;

int Graphics_ClipLine(int& x1, int& y1, int& x2, int& y2, int clipRegionX, int clipRegionY, int clipRegionX2, int clipRegionY2)
{
    const int ClipCodeC = 0;
    const int ClipCodeW = 1;
    const int ClipCodeE = 2;
    const int ClipCodeS = 4;
    const int ClipCodeN = 8;
    const int ClipCodeNE = 10;
    const int ClipCodeSE = 6;
    const int ClipCodeNW = 9;
    const int ClipCodeSW = 5;

    int p1_code = 0;
    int p2_code = 0;

    int xc1 = x1, yc1 = y1, xc2 = x2, yc2 = y2;


    if(y1 < clipRegionY)
    {
        p1_code |= ClipCodeN;
    }
    else if(y1 > clipRegionY2)
    {
        p1_code |= ClipCodeS;
    }

    if(x1 < clipRegionX)
    {
        p1_code |= ClipCodeW;
    }
    else if(x1 > clipRegionX2)
    {
        p1_code |= ClipCodeE;
    }

    if(y2 < 0)
    {
        p2_code |= ClipCodeN;
    }
    else if(y2 > clipRegionY2)
    {
        p2_code |= ClipCodeS;
    }


    if(x2 < clipRegionX)
    {
        p2_code |= ClipCodeW;
    }
    else if(x2 > clipRegionX2)
    {
        p2_code |= ClipCodeE;
    }

    if((p1_code & p2_code))
    {
        return 0; // error
    }

    // neither point is clipped by the bounding rect, so just draw the line
    if(p1_code == 0 && p2_code == 0)
    {
        // draw line
        return 1;
    }

    // determine end clip point for p1
    switch(p1_code)
    {
        case ClipCodeC:
            break;

        case ClipCodeN:
            yc1 = clipRegionY;
            xc1 = x1 + 0.5f + (clipRegionY - y1) * (x2 - x1) / (y2 - y1);
            break;

        case ClipCodeS:
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5f + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);
            break;

        case ClipCodeW:
            xc1 = clipRegionX;
            yc1 = y1 + 0.5f + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
            break;

        case ClipCodeE:
            xc1 = clipRegionX2;
            yc1 = y1 + 0.5f + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            break;

            // these cases are more complex, must compute 2 intersections
        case ClipCodeNE:

            // north hline intersection
            yc1 = clipRegionY;
            xc1 = x1 + 0.5f + (clipRegionY - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                // east vline intersection
                xc1 = clipRegionX2;
                yc1 = y1 + 0.5f + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeSE:

            // south hline intersection
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5f + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                // east vline intersection
                xc1 = clipRegionX2;
                yc1 = y1 + 0.5f + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeNW:

            // north hline intersection
            yc1 = clipRegionY;
            xc1 = x1 + 0.5f + (clipRegionY - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                xc1 = clipRegionX;
                yc1 = y1 + 0.5f + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeSW:

            // south hline intersection
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5f + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                xc1 = clipRegionX;
                yc1 = y1 + 0.5f + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
            } // end if
            break;

        default:
            break;
    }

    // determine clip point for p2
    switch(p2_code)
    {
        case ClipCodeC:
            break;

        case ClipCodeN:
        {
            yc2 = clipRegionY;
            xc2 = x2 + (clipRegionY - y2)*(x1 - x2) / (y1 - y2);
        } break;

        case ClipCodeS:
        {
            yc2 = clipRegionY2;
            xc2 = x2 + (clipRegionY2 - y2)*(x1 - x2) / (y1 - y2);
        } break;

        case ClipCodeW:
        {
            xc2 = clipRegionX;
            yc2 = y2 + (clipRegionX - x2)*(y1 - y2) / (x1 - x2);
        } break;

        case ClipCodeE:
        {
            xc2 = clipRegionX2;
            yc2 = y2 + (clipRegionX2 - x2)*(y1 - y2) / (x1 - x2);
        } break;

        // these cases are more complex, must compute 2 intersections
        case ClipCodeNE:
        {
            // north hline intersection
            yc2 = clipRegionY;
            xc2 = x2 + 0.5f + (clipRegionY - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                // east vline intersection
                xc2 = clipRegionX2;
                yc2 = y2 + 0.5f + (clipRegionX2 - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeSE:
        {
            // south hline intersection
            yc2 = clipRegionY2;
            xc2 = x2 + 0.5f + (clipRegionY2 - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                // east vline intersection
                xc2 = clipRegionX2;
                yc2 = y2 + 0.5f + (clipRegionX2 - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeNW:
        {
            // north hline intersection
            yc2 = clipRegionY;
            xc2 = x2 + 0.5f + (clipRegionY - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                xc2 = clipRegionX;
                yc2 = y2 + 0.5f + (clipRegionX - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeSW:
        {
            // south hline intersection
            yc2 = clipRegionY2;
            xc2 = x2 + 0.5f + (clipRegionY2 - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                xc2 = clipRegionX;
                yc2 = y2 + 0.5f + (clipRegionX - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        default:
            break;

    } // end switch

    // verify they've been clipped. 
    if((xc1 < clipRegionX) || (xc1 > clipRegionX2) ||
        (yc1 < clipRegionY) || (yc1 > clipRegionY2) ||
        (xc2 < clipRegionX) || (xc2 > clipRegionX2) ||
        (yc2 < clipRegionY) || (yc2 > clipRegionY2))
    {
        // nope. 
        return(0);
    }

    x1 = xc1;
    x2 = xc2;
    y1 = yc1;
    y2 = yc2;

    return 1;
}

int Graphics_Internal_DrawLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 colour)
{
    int deltaX;         // difference in x's
    int deltaY;         // difference in y's
    int dx2;        // result of dx * 2
    int dy2;        // result of dy * 2
    int xIncrementAmount;      // the amount of pixels to move along the x axis when drawing
    int yIncrementAmount;      // the amount of pixels to move along the y axis when drawing
    int error;


    //UINT *video_buffer = (UINT *)surfaceDescription->lpSurface;
    //int lPitch = surfaceDescription->lPitch >> 2;

    // grab the first pixel address in the video buffer
    //video_buffer = &video_buffer[x1 + y1 * 256];

    // compute the deltas for each axis
    deltaX = x2 - x1;
    deltaY = y2 - y1;

    // test for the slopes angle
    if(deltaX >= 0)
    {
        xIncrementAmount = 1;
    }
    else
    {
        xIncrementAmount = -1;
        deltaX = -deltaX;
    }
    if(deltaY >= 0)
    {
        yIncrementAmount = 1;
    } // end if line is moving down
    else
    {
        yIncrementAmount = -1;
        deltaY = -deltaY;  // need absolute value

    } // end else moving up

    // multiply dx and dy by two
    dx2 = deltaX << 1;
    dy2 = deltaY << 1;



    // now based on which delta is greater we can draw the line

    // current pixel coordinates
    int xCoordinate = x1;
    int yCoordinate = y1;

    if(deltaX > deltaY)
    {
        // initialize error term
        error = dy2 - deltaX;

        // draw the line
        for(int i = 0; i <= deltaX; i++)
        {
            // set the pixel
            //video_buffer[xCoordinate + yCoordinate * surfaceDescription->dwWidth] = colour;
            Graphics_DrawPixel(surface, xCoordinate, yCoordinate, colour);
            // test if error has overflowed
            if(error >= 0)
            {
                error -= dx2;

                // move to next line
                yCoordinate += yIncrementAmount;

            } // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            xCoordinate += xIncrementAmount;

        } // end for

    } // end if |slope| <= 1
    else
    {
        // initialize error term
        error = dx2 - deltaY;

        // draw the line
        for(int i = 0; i <= deltaY; i++)
        {
            // set the pixel
            //video_buffer[xCoordinate + yCoordinate * surfaceDescription->dwWidth] = colour;
            Graphics_DrawPixel(surface, xCoordinate, yCoordinate, colour);

            // test if error overflowed
            if(error >= 0)
            {
                error -= dy2;

                // move to next line
                xCoordinate += xIncrementAmount;

            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            yCoordinate += yIncrementAmount;

        } // end for

    } // end else |slope| > 1

    return 1;
}

int Graphics_DrawClippedLine(SDL_Surface *surface, int x, int y, int x2, int y2, Uint32 colour)
{
    int cxs = x;
    int cys = y;
    int cxe = x2;
    int cye = y2;

    if(Graphics_ClipLine(cxs, cys, cxe, cye, 0, 0, surface->w, surface->h))
    {
        Graphics_Internal_DrawLine(surface, cxs, cys, cxe, cye, colour);
    }

    return 1;
}

void Graphics_ScalePolygon2D(POLYGON2D *polygon, float xScale, float yScale)
{
    for(int i = 0; i < polygon->numberOfVertices; i++)
    {
        polygon->vertexList[i].x *= xScale;
        polygon->vertexList[i].y *= yScale;
    }
}

void Graphics_RotatePolygon2D(POLYGON2D *polygon, int theta)
{
    if(theta < 0)
        theta += 360;

    for(int i = 0; i < polygon->numberOfVertices; i++)
    {
        float rotatedX = (float)polygon->vertexList[i].x * g_CosineLookupTable[theta] -
            (float)polygon->vertexList[i].y * g_SineLookupTable[theta];

        float rotatedY = (float)polygon->vertexList[i].x * g_SineLookupTable[theta] +
            (float)polygon->vertexList[i].y * g_CosineLookupTable[theta];

        polygon->vertexList[i].x = rotatedX;
        polygon->vertexList[i].y = rotatedY;
    }
}

int Graphics_DrawPolygon2D(SDL_Surface *surface, POLYGON2D* polygonList)
{
    if(polygonList->isVisible)
    {
        int i = 0;
        for(i; i < polygonList->numberOfVertices - 1; i++)
        {
            Graphics_DrawClippedLine(surface,
                polygonList->vertexList[i].x + polygonList->xPositionCenter,
                polygonList->vertexList[i].y + polygonList->yPositionCenter,
                polygonList->vertexList[i + 1].x + polygonList->xPositionCenter,
                polygonList->vertexList[i + 1].y + polygonList->yPositionCenter,
                polygonList->colour);
        }

        Graphics_DrawClippedLine(surface,
            polygonList->vertexList[0].x + polygonList->xPositionCenter,
            polygonList->vertexList[0].y + polygonList->yPositionCenter,
            polygonList->vertexList[i].x + polygonList->xPositionCenter,
            polygonList->vertexList[i].y + polygonList->yPositionCenter,
            polygonList->colour);

        return 1;
    }
    else
    {
        return 0;
    }
}

SDL_Surface* Graphics_LoadImage(std::string filename, Uint8 R, Uint8 G, Uint8 B)
{
    if(filename.length() <= 4)
    {
        return nullptr;
    }

    int startPosition = filename.length() - 4;
    std::string& fileExtension = filename.substr(startPosition, 4);
    std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

    SDL_Surface *tempSurface;
    if(fileExtension == ".bmp" || 
       fileExtension == ".tga" || 
       fileExtension == ".pcx")
    {
        tempSurface = IMG_Load(filename.c_str());
        if(!tempSurface)
        {
            return nullptr;
        }

        Uint32 colourKey = SDL_MapRGB(tempSurface->format, R, G, B);
        // SDL_RLEACCEL can slow down SDL_LockSurface.
        SDL_SetColorKey(tempSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, colourKey);
        SDL_Surface *image = SDL_DisplayFormat(tempSurface);
        SDL_FreeSurface(tempSurface);

        return image;
    }

    return nullptr;
    
}

void Graphics_RenderSurface(SDL_Surface *screen, SDL_Surface *surfaceToRender, Uint32 x, Uint32 y) 
{
    SDL_Rect src, dest;

    src.x = 0;
    src.y = 0;
    src.w = surfaceToRender->w;
    src.h = surfaceToRender->h;

    dest.x = x;
    dest.y = y;
    dest.w = surfaceToRender->w;
    dest.h = surfaceToRender->h;

    SDL_BlitSurface(surfaceToRender, &src, screen, &dest);
}

void Graphics_FillRect(SDL_Surface *surface, int fromX, int fromY, int toX, int toY, Uint8 R, Uint8 G, Uint8 B)
{
    SDL_Rect rect;
    rect.h = toY;
    rect.w = toX;
    rect.x = fromX;
    rect.y = fromY;

    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, R, G, B));
}

void Graphics_ClearSurface(SDL_Surface *surface, Uint8 R, Uint8 G, Uint8 B)
{
    Graphics_FillRect(surface, 0, 0, surface->w, surface->h, R, G, B);
}

void Transform_OBJECT4DV1(OBJECT4DV1* object, MATRIX4X4* transformationMatrix, int selectedCoordinates, bool transformOrientation)
{
    switch(selectedCoordinates)
    {
        case TRANSFORM_LOCAL_ONLY:

            // transform each local/model vertex of the object mesh in place
            for(int vertex = 0; vertex < object->numVertices; vertex++)
            {
                POINT4D transformationResult;

                // transform the specified local vertex
                Mat_Mul_VECTOR4D_4X4(&object->vlistLocalVertices[vertex], transformationMatrix, &transformationResult);

                // copy it back in
                VECTOR4D_COPY(&object->vlistLocalVertices[vertex], &transformationResult);
            }
            break;

        case TRANSFORM_TRANS_ONLY:
            // transform each "transformed" vertec of the object mesh in place.

            for(int vertex = 0; vertex < object->numVertices; vertex++)
            {
                POINT4D transformationResult;

                // Transform the point. Note we're using vlistTransformedVertices[] here,
                // not vlistLocalVertices[] as above.
                Mat_Mul_VECTOR4D_4X4(&object->vlistTransformedVertices[vertex], transformationMatrix, &transformationResult);

                // copy it back out.
                VECTOR4D_COPY(&object->vlistTransformedVertices[vertex], &transformationResult);
            }
            break;

        case TRANSFORM_LOCAL_TO_TRANS:

            // transfom each local / model  vertex of the object mesh and store the result
            // in the transformed vertex list.
            for(int vertex = 0; vertex < object->numVertices; vertex++)
            {
                Mat_Mul_VECTOR4D_4X4(&object->vlistLocalVertices[vertex], transformationMatrix, &object->vlistTransformedVertices[vertex]);
            }
            break;
        default:
            break;
    }

    if(transformOrientation)
    {
        VECTOR4D rotationResult;

        // rotate the ux of the orientation basis
        Mat_Mul_VECTOR4D_4X4(&object->ux, transformationMatrix, &rotationResult);
        VECTOR4D_COPY(&object->ux, &rotationResult);

        // rotate the uy of the orientation basis
        Mat_Mul_VECTOR4D_4X4(&object->uy, transformationMatrix, &rotationResult);
        VECTOR4D_COPY(&object->uy, &rotationResult);

        // rotate the uz of the orientation basis
        Mat_Mul_VECTOR4D_4X4(&object->uz, transformationMatrix, &rotationResult);
        VECTOR4D_COPY(&object->uz, &rotationResult);
    }
}


void Rotate_XYZ_OBJECT4DV1(OBJECT4DV1* object, float thetaX, float thetaY, float thetaZ)
{
    MATRIX4X4 rotationMatrix;

    Build_XYZ_Rotation_MATRIX4X4(thetaX, thetaY, thetaZ, &rotationMatrix);

    // rotate each point of the mesh in local/model coordinates
    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        POINT4D transformationResult;

        // transform the point
        Mat_Mul_VECTOR4D_4X4(&object->vlistLocalVertices[vertex], &rotationMatrix, &transformationResult);

        // store the result back
        VECTOR4D_COPY(&object->vlistLocalVertices[vertex], &transformationResult);
    }

    // rotate the orientation basis of the object
    VECTOR4D rotationResult;

    // rotate the ux of the orientation basis
    Mat_Mul_VECTOR4D_4X4(&object->ux, &rotationMatrix, &rotationResult);
    VECTOR4D_COPY(&object->ux, &rotationResult);

    // rotate the uy of the orientation basis
    Mat_Mul_VECTOR4D_4X4(&object->uy, &rotationMatrix, &rotationResult);
    VECTOR4D_COPY(&object->uy, &rotationResult);

    // rotate the uz of the orientation basis
    Mat_Mul_VECTOR4D_4X4(&object->uz, &rotationMatrix, &rotationResult);
    VECTOR4D_COPY(&object->uz, &rotationResult);

}

void World_To_Camera_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera)
{
    // this is one way to do it, you might instead transform
    // the global list of polygons in the render list since you 
    // are guaranteed that those polys represent geometry that 
    // has passed through backface culling (if any)

    // transform each vertex in the object to camera coordinates.
    // assumes the object has already been transformed to world
    // coordinates and the result is in OBJECT4DV1::vlistTransformedVertices
    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        POINT4D transformationResult;

        // transform object->vlistTransformedVertices[vertex]
        Mat_Mul_VECTOR4D_4X4(&object->vlistTransformedVertices[vertex], &camera->worldToCameraTransform, &transformationResult);

        // store the results back
        VECTOR4D_COPY(&object->vlistTransformedVertices[vertex], &transformationResult);
    }
}

int Cull_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera, int cullFlags)
{
    // result of transforming the center of the bounding sphere
    POINT4D spherePosition;

    Mat_Mul_VECTOR4D_4X4(&object->worldPosition, &camera->worldToCameraTransform, &spherePosition);

    if(cullFlags & CULL_OBJECT_Z_PLANE) // only cull the Z clipping plane
    {
        // is the object outside the bounds of the clipping plane?
        if(((spherePosition.z - object->maximumRadius) > camera->farZClippingPlane) ||
            ((spherePosition.z + object->maximumRadius) < camera->nearZClippingPlane))
        {
            // why yes, yes it is; lets not draw it!
            // okay, good idea.
            // Word.
            object->state |= OBJECT4DV1_STATE_CULLED;
            return 1;
        }
    }

    if(cullFlags & CULL_OBJECT_X_PLANE)
    {
        // cull on the X clipping plane! FUCK YEAH! WOOOOOOOO!!!! HACK THE PLANET!!!
        float zTest = 0.5f * camera->viewPlaneWidth * spherePosition.z / camera->viewDistance;

        // test the left and right clipping planes against the leftmost and rightmost
        // points of the bounding sphere.
        if(((spherePosition.z - object->maximumRadius) > zTest) || // right side
            ((spherePosition.z + object->maximumRadius) < -zTest))  // left side
        {
            object->state |= OBJECT4DV1_STATE_CULLED;
            return 1;
        }
    }

    if(cullFlags & CULL_OBJECT_Y_PLANE)
    {
        float zTest = 0.5f * camera->viewPlaneHeight * spherePosition.z / camera->viewDistance;

        // test the top and bottom clipping planes against the bottommost and topmost
        // points of the bounding sphere.
        if(((spherePosition.y - object->maximumRadius) > zTest) || // top side
            ((spherePosition.y + object->maximumRadius) < -zTest)) // bottom side
        {
            object->state |= OBJECT4DV1_STATE_CULLED;
            return 1;
        }
    }

    return 0; // invalid flags passed
}

void Remove_Backfaces_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera)
{
    if(object->state & OBJECT4DV1_STATE_CULLED)
    {
        return;
    }

    for(int polygon = 0; polygon < object->numPolygons; polygon++)
    {
        POLYGON4DV1* currentPolygon = &object->plist[polygon];

        // test this polygon if and only if it's not clipped, not culled,
        // active, and visible and not 2 sided. 
        // note the test for the POLYGON4DV1_STATE_BACKFACE flag. If it's alredy
        // been set, then it's already been determined that this polygon is a backface
        // so.. move on.
        if(!(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
            (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
            (currentPolygon->attributes & POLYGON4DV1_ATTR_TWOSIDED) ||
            (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        // remember that polygons are not self-contained; they're based on the 
        // vertex list stored in the object.
        int verticeIndex1 = currentPolygon->vertices[0];
        int verticeIndex2 = currentPolygon->vertices[1];
        int verticeIndex3 = currentPolygon->vertices[2];


        // we need to compute the normal of this polygon face.
        // remember the vertices are in cw order.
        VECTOR4D u; // u = p0->p1
        VECTOR4D v; // v = p0->p2
        VECTOR4D normal; // normal = u * v

        VECTOR4D_Build(&object->vlistTransformedVertices[verticeIndex1],
            &object->vlistTransformedVertices[verticeIndex2], &u);
        VECTOR4D_Build(&object->vlistTransformedVertices[verticeIndex1],
            &object->vlistTransformedVertices[verticeIndex3], &v);

        // compute the cross the product
        VECTOR4D_Cross(&u, &v, &normal);

        // create the eye vector to viewpoint
        VECTOR4D view;
        VECTOR4D_Build(&object->vlistTransformedVertices[verticeIndex1], &camera->position, &view);

        // compute the dot product, because that'd probably be quite handy in the long run.
        // Boy howdy.
        float dotProduct = VECTOR4D_Dot(&normal, &view);

        // 0 = scathing.
        // if the sign is > 0 then the polygon is visible.
        // < 0 is invisible.
        if(dotProduct <= 0.0f)
        {
            currentPolygon->state |= POLYGON4DV1_STATE_BACKFACE;
        }

    }
}

void Remove_Backfaces_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera)
{
    for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // is this polygon valid?
        // test this polygon if and only if it's not clipped, not culled,
        // active, and visible and not 2 sided. 
        // note the test for the POLYGON4DV1_STATE_BACKFACE flag.If it's alredy
        // been set, then it's already been determined that this polygon is a backface
        // so.. move on. 
        if((currentPolygon == nullptr) ||
            !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
            (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
            (currentPolygon->attributes & POLYGON4DV1_ATTR_TWOSIDED) ||
            (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        // u & v are used to compute the normal of the polygon face
        VECTOR4D u; // u = p0->p1
        VECTOR4D v; // v = p0->p2
        VECTOR4D normal; // normal = u * v

        // build u, v
        VECTOR4D_Build(&currentPolygon->transformedVertexList[0], &currentPolygon->transformedVertexList[1], &u);
        VECTOR4D_Build(&currentPolygon->transformedVertexList[0], &currentPolygon->transformedVertexList[2], &v);

        // compute the cross product
        VECTOR4D_Cross(&u, &v, &normal);

        // create the eye vector to viewpoint
        VECTOR4D view;
        VECTOR4D_Build(&currentPolygon->transformedVertexList[0], &camera->position, &view);

        float dotProduct = VECTOR4D_Dot(&normal, &view);

        if(dotProduct <= 0.0f)
        {
            currentPolygon->state |= POLYGON4DV1_STATE_BACKFACE;
        }
    }
}

void Camera_To_Perspective_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera)
{
    // this implementation disregards the polygons themselves - 
    // it only works on the vertices in object->vlistTransformedVertices list.
    // this is one way to do it, you might instead transform
    // the global list of polygons in the render list since you 
    // are guaranteed that those polys represent geometry that 
    // has passed thru backfaces culling (if any)
    // finally this function is really for experimental reasons only
    // you would probably never let an object stay intact this far down
    // the pipeline, since it's probably that there's only a single polygon
    // that is visible! But this function has to transform the whole mesh!

    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        float x = object->vlistTransformedVertices[vertex].x;
        float y = object->vlistTransformedVertices[vertex].y;
        float z = object->vlistTransformedVertices[vertex].z;

        // transform the vertex by the camera's view distance
        object->vlistTransformedVertices[vertex].x = camera->viewDistance * x / z;
        object->vlistTransformedVertices[vertex].y = camera->viewDistance * y * camera->aspectRatio / z;
        // z = z, so no change
    }
}

void Camera_To_Perspective_Screen_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera)
{
    float alpha = (0.5f * camera->viewportWidth) - 0.5f;
    float beta = (0.5f * camera->viewPlaneHeight) - 0.5f;

    // transform each vertex in the object to perspective screen coordinates,
    // assuming the object has already been transformed to camera coordinates
    // and the result is in OBJECT4DV1::vlistTransformedVertices
    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        float x = object->vlistTransformedVertices[vertex].x;
        float y = object->vlistTransformedVertices[vertex].y;
        float z = object->vlistTransformedVertices[vertex].z;

        // transform the vertex by the view parameters in the camera
        object->vlistTransformedVertices[vertex].x = camera->viewDistance * x / z;
        object->vlistTransformedVertices[vertex].y = camera->viewDistance * y / z;

        // now the coordinates are in the range x:(-viewport_width/2 to viewport_width/2)
        // and y:(-viewport_height/2 to viewport_height/2), thus we need a translation and
        // since the y-axis is inverted, we need to invert y to complete the screen 
        // transform:
        object->vlistTransformedVertices[vertex].x = object->vlistTransformedVertices[vertex].x + alpha;
        object->vlistTransformedVertices[vertex].y = -object->vlistTransformedVertices[vertex].y + beta;
    }
}

void Perspective_To_Screen_OBJECT4DV1(OBJECT4DV1* object, CAMERA4DV1* camera)
{
    // transform each vertex in the object to screen coordinates
    // assumes the object has already been transformed to perspective
    // coordinates and the result is in OBJECT4DV1::viewlistTransformedVertices

    float alpha = (0.5f * camera->viewportWidth) - 0.5f;
    float beta = (0.5f * camera->viewportHeight) - 0.5f;

    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        // assumes the vertex is in perspective normalised coordinates from -1 to 1
        // on each axis. Scale them to viewport and y axis and project to screen.

        // transform the vertex by the view parameters in the camera
        object->vlistTransformedVertices[vertex].x = alpha + alpha * object->vlistTransformedVertices[vertex].x;
        object->vlistTransformedVertices[vertex].y = beta - beta * object->vlistTransformedVertices[vertex].y;
    }
}

void Convert_From_Homogenous4D_OBJECT4DV1(OBJECT4DV1* object)
{
    for(int vertex = 0; vertex < object->numVertices; vertex++)
    {
        // convert to non-homoegenous coordinates
        VECTOR4D_DIV_BY_W(&object->vlistTransformedVertices[vertex]);
    }
}

void Transform_RENDERLIST4DV1(RENDERLIST4DV1* renderList, MATRIX4X4* transformationMatrix, int coordinatesToTransform)
{
    switch(coordinatesToTransform)
    {
        case TRANSFORM_LOCAL_ONLY:
            for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
            {
                POLYGONF4DV1 *currentPolygon = renderList->polygonPointers[polygon];

                // transform this polygon if and only if it's not clipped, not culled,
                // active, and visible, note however the concept of "backface" is 
                // irrelevant in a wire frame engine though
                if((currentPolygon == nullptr) ||
                   !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
                    (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
                    (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
                {
                    continue;
                }

                for(int vertex = 0; vertex < 3; vertex++)
                {
                    // transform the vertex by the transformationMatrix argument
                    POINT4D transformationResult;

                    // transform the point indicated by currentPolygon->vertexList[vertex]
                    Mat_Mul_VECTOR4D_4X4(&currentPolygon->vertexList[vertex], transformationMatrix, &transformationResult);

                    // copy back into currentPolygon->vertexList[vertex]
                    VECTOR4D_COPY(&currentPolygon->vertexList[vertex], &transformationResult);
                }
            }
            break;

        case TRANSFORM_TRANS_ONLY:
            // transform each "transformed" vertex of the render list
            // remember, the idea of the POLYGONF4DV1::transformedVertexList[vertex] array is to accumulate
            // transformations
            for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
            {
                POLYGONF4DV1 *currentPolygon = renderList->polygonPointers[polygon];

                // is this polygon valid?
                // transform this polygon if and only if it's not clipped, not culled,
                // active, and visible, note however the concept of "backface" is 
                // irrelevant in a wire frame engine though
                if((currentPolygon == nullptr) ||
                  !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
                   (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
                   (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
                {
                    continue;
                }

                for(int vertex = 0; vertex < 3; vertex++)
                {
                    // transform the vertex by the transformationMatrix argument
                    POINT4D transformationResult;

                    // transform the point in currentPolygon->transformedVertexList[vertex]
                    Mat_Mul_VECTOR4D_4X4(&currentPolygon->transformedVertexList[vertex], transformationMatrix, &transformationResult);

                    // store the result back into currentPolygon->transformedVertexList[vertex]
                    VECTOR4D_COPY(&currentPolygon->transformedVertexList[vertex], &transformationResult);
                }
            }
            break;

        case TRANSFORM_LOCAL_TO_TRANS:
            // transform each local/model vertex of the render list and store result
            // in "transformed" vertex list
            for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
            {
                POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

                // is this polygon valid?
                // transform this polygon if and only if it's not clipped, not culled,
                // active, and visible, note however the concept of "backface" is 
                // irrelevant in a wire frame engine though
                if((currentPolygon == nullptr) ||
                  !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
                   (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
                   (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
                {
                    continue;
                }

                for(int vertex = 0; vertex < 3; vertex++)
                {
                    // transform the vertex by mt
                    Mat_Mul_VECTOR4D_4X4(&currentPolygon->vertexList[vertex], transformationMatrix, &currentPolygon->transformedVertexList[vertex]);
                }
            }
            break;

        default:
            break;
    }
}

void Model_To_World_RENDERLIST4DV1(RENDERLIST4DV1* renderList, POINT4D* worldPosition, int coordinatesToTransform)
{
    // interate through the vertex list and transform all the model/local 
    // coords to world coords by translating the vertex list by
    // the amount world_pos and storing the results in the polygon's transformedVertexList.

    if(coordinatesToTransform == TRANSFORM_LOCAL_TO_TRANS)
    {
        for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
        {
            POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

            // transform this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concept of "backface" is 
            // irrelevant in a wire frame engine though
            if((currentPolygon == nullptr) ||
              !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
               (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
               (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
            {
                continue;
            }

            for(int vertex = 0; vertex < 3; vertex++)
            {
                // translate vertex
                VECTOR4D_Add(&currentPolygon->vertexList[vertex], worldPosition, &currentPolygon->transformedVertexList[vertex]);
            }
        }
    }
    else // TRANSFORM_TRANS_ONLY
    {
        for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
        {
            POLYGONF4DV1 *currentPolygon = renderList->polygonPointers[polygon];

            // transform this polygon if and only if it's not clipped, not culled,
            // active, and visible, note however the concept of "backface" is 
            // irrelevant in a wire frame engine though
            if((currentPolygon == nullptr) ||
              !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
               (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
               (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
            {
                continue;
            }

            for(int vertex = 0; vertex < 3; vertex++)
            {
                // translate the vertex
                VECTOR4D_Add(&currentPolygon->transformedVertexList[vertex], worldPosition, &currentPolygon->transformedVertexList[vertex]);
            }
        }
    }
}

void Convert_From_Homogenous4D_RENDERLIST4DV1(RENDERLIST4DV1* renderList)
{
    for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // transform this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concept of "backface" is 
        // irrelevant in a wire frame engine though
        if((currentPolygon == nullptr) ||
          !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
           (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
           (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        for(int vertex = 0; vertex < 3; vertex++)
        {
            // convert to non-homogenous coords
            VECTOR4D_DIV_BY_W(&currentPolygon->transformedVertexList[vertex]);
        }
    }
}

void World_To_Camera_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera)
{
    // you would use this function instead of the object based function
    // if you decided earlier in the pipeline to turn each object into 
    // a list of polygons and then add them to the global render list.
    // The conversion of an object into polygons probably would have
    // happened after object culling, local transforms, local to world
    // and backface culling, so the minimum number of polygons from
    // each object are in the list. Note that the function assumes
    // that at LEAST the local to world transform has been called
    // and the polygon data is in the transformed list POLYGONF4DV1::transformedVertexList[vertex].

    for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // transform this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concept of "backface" is 
        // irrelevant in a wire frame engine though
        if((currentPolygon == nullptr) ||
          !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
           (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
           (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        for(int vertex = 0; vertex < 3; vertex++)
        {
            // transform the vertex by the world to camera transform matrix.
            POINT4D transformationResult;

            // transform the specified vertex in currentPolygon->transformedVertexList[vertex]
            Mat_Mul_VECTOR4D_4X4(&currentPolygon->transformedVertexList[vertex], &camera->worldToCameraTransform, &transformationResult);

            // copy the result back into currentPolygon->transformedVertexList[vertex]
            VECTOR4D_COPY(&currentPolygon->transformedVertexList[vertex], &transformationResult);
        }
    }
}

void Camera_To_Perspective_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera)
{
    // transform each polygon in the render list into camera coordinates
    // assumes the render list has already been transformed to world
    // coordinates and the result is in POLYGONF4DV1::transformedVertexList[vertex] of each polygon object

    for(int polygon = 0; renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // transform this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concept of "backface" is 
        // irrelevant in a wire frame engine though
        if((currentPolygon == nullptr) ||
          !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
           (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
           (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        for(int vertex = 0; vertex < 3; vertex++)
        {
            float x = currentPolygon->transformedVertexList[vertex].x;
            float y = currentPolygon->transformedVertexList[vertex].y;
            float z = currentPolygon->transformedVertexList[vertex].z;

            currentPolygon->transformedVertexList[vertex].x = camera->viewDistance * x / z;
            currentPolygon->transformedVertexList[vertex].y = camera->viewDistance * y * camera->aspectRatio / z;
            // z = z, no change.

            // not that we are NOT dividing by the homogenous w coordinate since
            // we are not using a matrix operation for this version of the function 
        }
    }
}

void Camera_To_Perspective_Screen_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera)
{
    // transform each polygon in the render list to perspective screen 
    // coordinates assumes the render list has already been transformed 
    // to camera coordinates and the result is in POLYGON4DFV1::transformedVertexList[vertex]

    for(int polygon = 0; renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // transform this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concept of "backface" is 
        // irrelevant in a wire frame engine though
        if((currentPolygon == nullptr) ||
          !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
           (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
           (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        float alpha = (0.5f * camera->viewportWidth) - 0.5f;
        float beta = (0.5f * camera->viewportHeight) - 0.5f;

        for(int vertex = 0; vertex < 3; vertex++)
        {
            float x = currentPolygon->transformedVertexList[vertex].x;
            float y = currentPolygon->transformedVertexList[vertex].y;
            float z = currentPolygon->transformedVertexList[vertex].z;

            // transform the vertex by the view parameters in the camera
            currentPolygon->transformedVertexList[vertex].x = camera->viewDistance * x / z;
            currentPolygon->transformedVertexList[vertex].y = camera->viewDistance * y / z;
            // z = z, so no change

            // note that we are NOT dividing by the homogenous w coordinate since
            // we are not using a matrix operation for this version of the function

            // now the coordinates are in the range x:(-viewport_width/2 to viewport_width/2)
            // and y:(-viewport_height/2 to viewport_height/2), thus we need a translation and
            // since the y-axis is inverted, we need to invert y to complete the screen 
            // transform:
            currentPolygon->transformedVertexList[vertex].x = currentPolygon->transformedVertexList[vertex].x + alpha;
            currentPolygon->transformedVertexList[vertex].y = -currentPolygon->transformedVertexList[vertex].y + beta;
        }
    }
}

void Perspective_To_Screen_RENDERLIST4DV1(RENDERLIST4DV1* renderList, CAMERA4DV1* camera)
{
    // transform each polygon in the render list from perspective to screen 
    // coordinates assumes the render list has already been transformed 
    // to normalized perspective coordinates and the result is in POLYGONF4DV1::transformedVertexList
    for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
    {
        POLYGONF4DV1* currentPolygon = renderList->polygonPointers[polygon];

        // transform this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concept of "backface" is 
        // irrelevant in a wire frame engine though
        if((currentPolygon == nullptr) ||
          !(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
           (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
           (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        float alpha = (0.5f * camera->viewportWidth) - 0.5f;
        float beta = (0.5f * camera->viewportHeight) - 0.5f;

        for(int vertex = 0; vertex < 3; vertex++)
        {
            // the vertex is in perspective normalized coords from - 1 to 1
            // on each axis, simple scale them and invert y axis and project
            // to screen

            // transform the vertex by the view parameters in the camera
            currentPolygon->transformedVertexList[vertex].x = alpha + alpha * currentPolygon->transformedVertexList[vertex].x;
            currentPolygon->transformedVertexList[vertex].y = beta - beta * currentPolygon->transformedVertexList[vertex].y;
        }
    }
}

void Reset_OBJECT4DV1(OBJECT4DV1* object)
{
// #define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))

    // here's where you would add stuff to ready any object for the pipeline
    object->state &= ~OBJECT4DV1_STATE_CULLED;

    for(int polygon = 0; polygon < object->numPolygons; polygon++)
    {
        POLYGON4DV1* currentPolygon = &object->plist[polygon];

        // is the polygon visible?
        if(!(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE))
        {
            // nope!
            continue;
        }

        // reset clipped and backface flags
        currentPolygon->state &= ~POLYGON4DV1_STATE_CLIPPED;
        currentPolygon->state &= ~POLYGON4DV1_STATE_BACKFACE;
    }
}

int Insert_POLYGON4DV1_RENDERLIST4DV1(RENDERLIST4DV1* renderList, POLYGON4DV1* polygon)
{
    if(renderList->numPolygons >= RENDERLIST4DV1_MAX_POLYGONS)
    {
        return 0;
    }

    renderList->polygonPointers[renderList->numPolygons] = &renderList->polygonData[renderList->numPolygons];

    renderList->polygonData[renderList->numPolygons].state = polygon->state;
    renderList->polygonData[renderList->numPolygons].attributes = polygon->attributes;
    renderList->polygonData[renderList->numPolygons].colour = polygon->colour;


    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].transformedVertexList[0],
        &polygon->vertexList[polygon->vertices[0]]);

    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].transformedVertexList[1],
        &polygon->vertexList[polygon->vertices[1]]);

    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].transformedVertexList[2],
        &polygon->vertexList[polygon->vertices[2]]);

    // copy into local vertices too
    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].vertexList[0],
        &polygon->vertexList[polygon->vertices[0]]);

    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].vertexList[1],
        &polygon->vertexList[polygon->vertices[1]]);

    VECTOR4D_COPY(&renderList->polygonData[renderList->numPolygons].vertexList[2],
        &polygon->vertexList[polygon->vertices[2]]);

    // now the polygon is loaded into the next free array position, but
    // we need to fix up the links

    if(renderList->numPolygons == 0)
    {
        renderList->polygonData[0].next = nullptr;
        renderList->polygonData[0].previous = nullptr;
    }
    else
    {
        // set this node to point the the previous node and the next node (nullptr)
        renderList->polygonData[renderList->numPolygons].next = nullptr;
        renderList->polygonData[renderList->numPolygons].previous = &renderList->polygonData[renderList->numPolygons - 1];

        // set the previous node to this node.
        renderList->polygonData[renderList->numPolygons - 1].next = &renderList->polygonData[renderList->numPolygons];
    }

    renderList->numPolygons++;

    return 1;
}

int Insert_POLYGONF4DV1_RENDERLIST4DV1(RENDERLIST4DV1* renderList, POLYGONF4DV1* polygon)
{
    if(renderList->numPolygons >= RENDERLIST4DV1_MAX_POLYGONS)
    {
        return 0;
    }

    // step 1: copy polygon into next opening in polygon render list
    // point pointer to polygon structure
    renderList->polygonPointers[renderList->numPolygons] = &renderList->polygonData[renderList->numPolygons];

    // copy face right into array, thats it
    memcpy((void*) &renderList->polygonData[renderList->numPolygons], (void*)polygon, sizeof(POLYGONF4DV1));

    // now the polygon is loaded into the next free array position, but
    // we need to fix up the links est if this is the first entry
    if(renderList->numPolygons == 0)
    {
        renderList->polygonData[0].next = nullptr;
        renderList->polygonData[0].previous = nullptr;
    }
    else
    {
        // first set this node to point to previous node and next node (null)
        renderList->polygonData[renderList->numPolygons].next = nullptr;
        renderList->polygonData[renderList->numPolygons].previous = &renderList->polygonData[renderList->numPolygons - 1];

        // now set previous node to point to this node
        renderList->polygonData[renderList->numPolygons - 1].next = &renderList->polygonData[renderList->numPolygons];
    }

    renderList->numPolygons++;

    return 1;
}

int Insert_OBJECT4DV1_RENDERLIST4DV1(RENDERLIST4DV1* renderList, OBJECT4DV1* object, int insertLocal)
{
    if(!(object->state * OBJECT4DV1_STATE_ACTIVE) ||
        (object->state & OBJECT4DV1_STATE_CULLED) ||
       !(object->state & OBJECT4DV1_STATE_VISIBLE))
    {
        return 0;
    }

    // the object is valid, let's rip it apart polygon by polygon
    for(int polygon = 0; polygon < object->numPolygons; polygon++)
    {
        POLYGON4DV1* currentPolygon = &object->plist[polygon];

        // is this polygon even visible?
        if(!(currentPolygon->state & POLYGON4DV1_STATE_ACTIVE) ||
            (currentPolygon->state & POLYGON4DV1_STATE_CLIPPED) ||
            (currentPolygon->state & POLYGON4DV1_STATE_BACKFACE))
        {
            // nope
            continue;
        }

        // override vertex list polygon refers to
        // the case that you want the local coords used
        // first save old pointer
        POINT4D* oldVertexList = currentPolygon->vertexList;

        if(insertLocal)
        {
            currentPolygon->vertexList = object->vlistLocalVertices;
        }
        else
        {
            currentPolygon->vertexList = object->vlistTransformedVertices;
        }

        if(!Insert_POLYGON4DV1_RENDERLIST4DV1(renderList, currentPolygon))
        {
            // failed, the list contains the maximum number of polygons
            currentPolygon->vertexList = oldVertexList;

            return 0;
        }

        currentPolygon->vertexList = oldVertexList;
    }

    return 1;
}


void Draw_OBJECT4DV1_Wire(SDL_Surface *surface, OBJECT4DV1* object)
{
    for(int polygon = 0; polygon < object->numPolygons; polygon++)
    {
        // render this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concecpt of "backface" is 
        // irrelevant in a wire frame engine though
        if(!(object->plist[polygon].state & POLYGON4DV1_STATE_ACTIVE) ||
            (object->plist[polygon].state & POLYGON4DV1_STATE_CLIPPED) ||
            (object->plist[polygon].state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        // extract vertex indices into master list, rember the polygons are 
        // NOT self contained, but based on the vertex list stored in the object
        // itself
        int vertexIndex0 = object->plist[polygon].vertices[0];
        int vertexIndex1 = object->plist[polygon].vertices[1];
        int vertexIndex2 = object->plist[polygon].vertices[2];


        Graphics_DrawClippedLine(surface, 
            object->vlistTransformedVertices[vertexIndex0].x, // from x
            object->vlistTransformedVertices[vertexIndex0].y, // from y
            object->vlistTransformedVertices[vertexIndex1].x, // to x
            object->vlistTransformedVertices[vertexIndex1].y, // to y
            object->plist[polygon].colour);

        Graphics_DrawClippedLine(surface, 
            object->vlistTransformedVertices[vertexIndex1].x, // from x
            object->vlistTransformedVertices[vertexIndex1].y, // from y
            object->vlistTransformedVertices[vertexIndex2].x, // to x
            object->vlistTransformedVertices[vertexIndex2].y, // to y
            object->plist[polygon].colour);

        Graphics_DrawClippedLine(surface,
            object->vlistTransformedVertices[vertexIndex2].x, // from x
            object->vlistTransformedVertices[vertexIndex2].y, // from y
            object->vlistTransformedVertices[vertexIndex0].x, // to x
            object->vlistTransformedVertices[vertexIndex0].y, // to y
            object->plist[polygon].colour);
#ifdef _DEBUG
        // rendering stats
        debugPolygonsRenderedPerFrame++;
#endif
    }
}

void Draw_RENDERLIST4DV1_Wire(SDL_Surface* surface, RENDERLIST4DV1* renderList)
{
    for(int polygon = 0; polygon < renderList->numPolygons; polygon++)
    {
        // render this polygon if and only if it's not clipped, not culled,
        // active, and visible, note however the concecpt of "backface" is 
        // irrelevant in a wire frame engine though
        if(!(renderList->polygonPointers[polygon]->state & POLYGON4DV1_STATE_ACTIVE) ||
            (renderList->polygonPointers[polygon]->state & POLYGON4DV1_STATE_CLIPPED) ||
            (renderList->polygonPointers[polygon]->state & POLYGON4DV1_STATE_BACKFACE))
        {
            continue;
        }

        Graphics_DrawClippedLine(surface, 
            renderList->polygonPointers[polygon]->transformedVertexList[0].x, // from x
            renderList->polygonPointers[polygon]->transformedVertexList[0].y, // from y
            renderList->polygonPointers[polygon]->transformedVertexList[1].x, // to x
            renderList->polygonPointers[polygon]->transformedVertexList[1].y, // to y
            renderList->polygonPointers[polygon]->colour);

        Graphics_DrawClippedLine(surface,
            renderList->polygonPointers[polygon]->transformedVertexList[1].x, // from x
            renderList->polygonPointers[polygon]->transformedVertexList[1].y, // from y
            renderList->polygonPointers[polygon]->transformedVertexList[2].x, // to x
            renderList->polygonPointers[polygon]->transformedVertexList[2].y, // to y
            renderList->polygonPointers[polygon]->colour);

        Graphics_DrawClippedLine(surface,
            renderList->polygonPointers[polygon]->transformedVertexList[2].x, // from x
            renderList->polygonPointers[polygon]->transformedVertexList[2].y, // from y
            renderList->polygonPointers[polygon]->transformedVertexList[0].x, // to x
            renderList->polygonPointers[polygon]->transformedVertexList[0].y, // to y
            renderList->polygonPointers[polygon]->colour);
    }

#ifdef _DEBUG
    debugPolygonsRenderedPerFrame++;
#endif

}

void Build_CAMERA4DV1_Matrix_Euler(CAMERA4DV1* camera, int cameraRotationSequence)
{
    // we need to create a transformation matrix that looks like:
    // Mcam = mt(-1) * my(-1) * mx(-1) * mz(-1)
    // that is the inverse of the camera translation matrix mutilplied
    // by the inverses of yxz, in that order. Note that the order of
    // the rotation matrices is really up to you, so no particular order is enforced.
    // The order of XYZ is based on the value specified in cameraRotationSequence,
    // of cameraRotationSequence which can be any value CAMERA_ROTATE_SEQUENCE_XYZ where 
    // XYZ can be in any order, YXZ, ZXY, etc.

    MATRIX4X4 inversedCameraRotationMatrix;
    MATRIX4X4 inversedCameraXAxisRotationMatrix;
    MATRIX4X4 inversedCameraYAxisRotationMatrix;
    MATRIX4X4 inversedCameraZAxisRotationMatrix;
    MATRIX4X4 concatenatedInverseRotationMatrix;
    MATRIX4X4 tempMatrix;

    // step 1: create the inverse translation matrix for the camera position
    Mat_Init_4X4(&inversedCameraRotationMatrix, 
        1, 0, 0, 0, // m00, m01, m02, m03
        0, 1, 0, 0, // m10, m11, m12, m13
        0, 0, 1, 0, // m20, m21, m22, m23
        -camera->position.x, -camera->position.y, -camera->position.z, 1 //  m30, m31, m32, m33
        );

    // extract out the euler angles
    float thetaX = camera->direction.x;
    float thetaY = camera->direction.y;
    float thetaZ = camera->direction.z;

    // step 2: create the inverse rotation sequence for the camera.
    // Either the transpose of the normal rotation matrix or
    // plugging negative values into each of the rotations will result
    // in an inverse matrix.

    // compute the sine and cosine of the X angle
    float cosTheta = Fast_Cos(thetaX); // no change since cos(-x) = cos(x)
    float sinTheta = -Fast_Sin(thetaX); // sin(-x) = -sin(x)

    Mat_Init_4X4(&inversedCameraXAxisRotationMatrix,
        1, 0, 0, 0,                   // m00, m01, m02, m03
        0, cosTheta, sinTheta, 0,     // m10, m11, m12, m13
        0, -sinTheta, cosTheta, 0,    // m20, m21, m22, m23
        0, 0, 0, 1                    // m30, m31, m32, m33
        );

    cosTheta = Fast_Cos(thetaY); // no change since cos(-y) = cos(y)
    sinTheta = Fast_Cos(thetaY); // sin(-y) = -sin(y) 

    Mat_Init_4X4(&inversedCameraYAxisRotationMatrix, 
        cosTheta, 0, -sinTheta, 0,     // m00, m01, m02, m03
        0, 1, 0, 0,                    // m10, m11, m12, m13
        sinTheta, 0, cosTheta, 0,      // m20, m21, m22, m23
        0, 0, 0, 1                     // m30, m31, m32, m33
        );

    cosTheta = Fast_Cos(thetaZ);  // no change since cos(-z) = cos(z)
    sinTheta = -Fast_Sin(thetaZ); // sin(-z) = -sin(z)

    Mat_Init_4X4(&inversedCameraZAxisRotationMatrix, 
        cosTheta, sinTheta, 0, 0,       // m00, m01, m02, m03
        -sinTheta, cosTheta, 0, 0,      // m10, m11, m12, m13
        0, 0, 1, 0,                     // m20, m21, m22, m23
        0, 0, 0, 1                      // m30, m31, m32, m33
        );  

    // compute inverse camera rotation sequence
    switch(cameraRotationSequence)
    {
        case CAMERA_ROTATE_SEQUENCE_XYZ:
            Mat_Mul_4X4(&inversedCameraXAxisRotationMatrix, &inversedCameraYAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraZAxisRotationMatrix, &concatenatedInverseRotationMatrix);
            break;

        case CAMERA_ROTATE_SEQUENCE_YXZ:
        {
            Mat_Mul_4X4(&inversedCameraYAxisRotationMatrix, &inversedCameraXAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraZAxisRotationMatrix, &concatenatedInverseRotationMatrix);
        } break;

        case CAMERA_ROTATE_SEQUENCE_XZY:
        {
            Mat_Mul_4X4(&inversedCameraXAxisRotationMatrix, &inversedCameraZAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraYAxisRotationMatrix, &concatenatedInverseRotationMatrix);
        } break;

        case CAMERA_ROTATE_SEQUENCE_YZX:
        {
            Mat_Mul_4X4(&inversedCameraYAxisRotationMatrix, &inversedCameraZAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraXAxisRotationMatrix, &concatenatedInverseRotationMatrix);
        } break;

        case CAMERA_ROTATE_SEQUENCE_ZYX:
        {
            Mat_Mul_4X4(&inversedCameraZAxisRotationMatrix, &inversedCameraYAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraXAxisRotationMatrix, &concatenatedInverseRotationMatrix);
        } break;

        case CAMERA_ROTATE_SEQUENCE_ZXY:
        {
            Mat_Mul_4X4(&inversedCameraZAxisRotationMatrix, &inversedCameraXAxisRotationMatrix, &tempMatrix);
            Mat_Mul_4X4(&tempMatrix, &inversedCameraYAxisRotationMatrix, &concatenatedInverseRotationMatrix);

        } break;
        default:
            break;
    }

    // now concatenatedInverseRotationMatrix holds the concatenated product of inverse rotation matrices
    // multiply the inverse translation matrix against it and store in the 
    // camera objects' camera transform matrix we are done!
    Mat_Mul_4X4(&inversedCameraRotationMatrix, &concatenatedInverseRotationMatrix, &camera->worldToCameraTransform);
}

void Build_CAMERA4DV1_Matrix_UVN(CAMERA4DV1* camera, int mode)
{
    MATRIX4X4 inverseCameraTranslationMatrix;
    MATRIX4X4 finalUVNMatrix;

    // step 1: create the inverse translation matrix for the camera position
    Mat_Init_4X4(&inverseCameraTranslationMatrix, 
        1, 0, 0, 0,   // m00, m01, m02, m03
        0, 1, 0, 0,   // m10, m11, m12, m13
        0, 0, 1, 0,   // m20, m21, m22, m23
        -camera->position.x, -camera->position.y, -camera->position.z, 1); // m30, m31, m32, m33

    // step 2: determine how the target point will be computed
    if(mode == UVN_MODE_SPHERICAL)
    {
        // use spherical construction; target needs to be recomputed

        float phi = camera->direction.x; // elevation
        float theta = camera->direction.y; // heading

        float sinPhi = Fast_Sin(phi);
        float cosPhi = Fast_Cos(phi);

        float sinTheta = Fast_Sin(theta);
        float cosTheta = Fast_Cos(theta);

        // now compute the target point on a unit sphere x,y,z
        camera->target.x = -1 * sinPhi * sinTheta;
        camera->target.y = 1 * cosPhi;
        camera->target.z = 1 * sinPhi * cosTheta;
    }

    // at this point, we have the view reference point, the target and that's
    // all we need to recompute u,v,n

    // Step 1: n = <target position - view reference point>
    VECTOR4D_Build(&camera->position, &camera->target, &camera->n);

    // Step 2: Let v = <0,1,0>
    VECTOR4D_INITXYZ(&camera->v, 0, 1, 0);

    // Step 3: u = (v x n)
    VECTOR4D_Cross(&camera->v, &camera->n, &camera->u);

    // Step 4: v = (n x u)
    VECTOR4D_Cross(&camera->n, &camera->u, &camera->v);

    // Step 5: normalize all vectors
    VECTOR4D_Normalize(&camera->u);
    VECTOR4D_Normalize(&camera->v);
    VECTOR4D_Normalize(&camera->n);

    // build the UVN matrix by placing u,v,n as the columns of the matrix
    Mat_Init_4X4(&finalUVNMatrix, 
        camera->u.x, camera->v.x, camera->n.x, 0,  // m00, m01, m02, m03
        camera->u.y, camera->v.y, camera->n.y, 0,  // m10, m11, m12, m13
        camera->u.z, camera->v.z, camera->n.z, 0,  // m20, m21, m22, m23
        0, 0, 0, 1); // m30, m31, m32, 33

    // now multiply the translation matrix and the uvn matrix and store in the 
    // final camera matrix CAMERA4DV1::worldToCameraTransform
    Mat_Mul_4X4(&inverseCameraTranslationMatrix, &finalUVNMatrix, &camera->worldToCameraTransform);
}

void Init_CAMERA4DV1(CAMERA4DV1* camera, int cameraAttributes, POINT4D* cameraPosition, VECTOR4D* cameraDirection,
    POINT4D* cameraTarget, float nearZClippingPlane, float farZClippingPlane, float fieldOfViewInDegrees,
    float viewportWidth, float viewportHeight)
{
    camera->attributes = cameraAttributes;

    VECTOR4D_COPY(&camera->position, cameraPosition);
    VECTOR4D_COPY(&camera->direction, cameraDirection);

    VECTOR4D_INITXYZ(&camera->u, 1, 0, 0);
    VECTOR4D_INITXYZ(&camera->v, 0, 1, 0);
    VECTOR4D_INITXYZ(&camera->n, 0, 0, 1);

    if(cameraTarget != nullptr)
    {
        VECTOR4D_COPY(&camera->target, cameraTarget);
    }
    else
    {
        VECTOR4D_ZERO(&camera->target);
    }

    camera->nearZClippingPlane = nearZClippingPlane;
    camera->farZClippingPlane = farZClippingPlane;
    camera->viewportWidth = viewportWidth;
    camera->viewportHeight = viewportHeight;
    camera->viewportCenterX = (viewportWidth - 1) / 2;
    camera->viewportCenterY = (viewportHeight - 1) / 2;
    camera->aspectRatio = (float) viewportWidth / (float) viewportHeight;

    MAT_IDENTITY_4X4(&camera->worldToCameraTransform);
    MAT_IDENTITY_4X4(&camera->cameraToPerspectiveTransform);
    MAT_IDENTITY_4X4(&camera->perspectiveToScreenTransform);

    camera->fieldOfView = fieldOfViewInDegrees;

    // set the viewplane dimensions up, they will be 2 x (2/ar)
    camera->viewPlaneWidth = 2.0;
    camera->viewPlaneHeight = 2.0 / camera->aspectRatio;

    // now we know fov and we know the viewplane dimensions plug into formula and
    // solve for view distance parameters
    float tangentFieldOfView = tan(DEG_TO_RAD(fieldOfViewInDegrees / 2));

    camera->viewDistance = (0.5f) * camera->viewPlaneWidth * tangentFieldOfView;

    if(fieldOfViewInDegrees == 90.0f)
    {
        // set up the clipping planes -- easy for 90 degrees!

        POINT3D originPoint;
        VECTOR3D_INITXYZ(&originPoint, 0, 0, 0);

        VECTOR3D planeNormal;

        // right clipping plane
        VECTOR3D_INITXYZ(&planeNormal, 1, 0, -1); // x = z
        PLANE3D_Init(&camera->rightClippingPlane, &originPoint, &planeNormal, 1);

        // left clipping plane
        VECTOR3D_INITXYZ(&planeNormal, -1, 0, -1); // -x = z
        PLANE3D_Init(&camera->leftClippingPlane, &originPoint, &planeNormal, 1);

        // top clipping plane
        VECTOR3D_INITXYZ(&planeNormal, 0, 1, -1); // y = z
        PLANE3D_Init(&camera->topClippingPlane, &originPoint, &planeNormal, 1);

        // bottom clipping plane
        VECTOR3D_INITXYZ(&planeNormal, 0, -1, -1); // -y = z plane
        PLANE3D_Init(&camera->bottomClippingPlane, &originPoint, &planeNormal, 1);
    }
    else
    {
        // now compute clipping planes

        POINT3D originPoint;
        VECTOR3D_INITXYZ(&originPoint, 0, 0, 0);

        VECTOR3D  planeNormal;

        // since we don't have a 90 degree field of view, computing the normals
        // are a bit tricky; there are a number of geometric constructions
        // that solve the problem, but I'm going to solve for the
        // vectors that represent the 2D projections of the frustrum planes
        // on the x-z and y-z planes and then find perpendiculars to them

        // right clipping plane
        VECTOR3D_INITXYZ(&planeNormal, camera->viewDistance, 0, -camera->viewPlaneWidth / 2.0f);
        PLANE3D_Init(&camera->rightClippingPlane, &originPoint, &planeNormal, 1);

        // left clipping plane, we can simply reflect the right normal about
        // the z axis since the planes are symetric about the z axis
        // thus invert x only
        VECTOR3D_INITXYZ(&planeNormal, -camera->viewDistance, 0, -camera->viewPlaneWidth / 2.0f);
        PLANE3D_Init(&camera->leftClippingPlane, &originPoint, &planeNormal, 1);

        // top clipping plane, same construction
        VECTOR3D_INITXYZ(&planeNormal, 0, camera->viewDistance, -camera->viewPlaneWidth / 2.0);
        PLANE3D_Init(&camera->topClippingPlane, &originPoint, &planeNormal, 1);

        // bottom clipping plane, same inversion
        VECTOR3D_INITXYZ(&planeNormal, 0, -camera->viewDistance, -camera->viewPlaneWidth / 2.0);
        PLANE3D_Init(&camera->bottomClippingPlane, &originPoint, &planeNormal, 1);
    }
}