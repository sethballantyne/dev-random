#include <algorithm>
#include <SDL_image.h>
#include "graphics.h"

extern float g_SineLookupTable[361];
extern float g_CosineLookupTable[361];

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
            xc1 = x1 + 0.5 + (clipRegionY - y1) * (x2 - x1) / (y2 - y1);
            break;

        case ClipCodeS:
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5 + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);
            break;

        case ClipCodeW:
            xc1 = clipRegionX;
            yc1 = y1 + 0.5 + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
            break;

        case ClipCodeE:
            xc1 = clipRegionX2;
            yc1 = y1 + 0.5 + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            break;

            // these cases are more complex, must compute 2 intersections
        case ClipCodeNE:

            // north hline intersection
            yc1 = clipRegionY;
            xc1 = x1 + 0.5 + (clipRegionY - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                // east vline intersection
                xc1 = clipRegionX2;
                yc1 = y1 + 0.5 + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeSE:

            // south hline intersection
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5 + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                // east vline intersection
                xc1 = clipRegionX2;
                yc1 = y1 + 0.5 + (clipRegionX2 - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeNW:

            // north hline intersection
            yc1 = clipRegionY;
            xc1 = x1 + 0.5 + (clipRegionY - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                xc1 = clipRegionX;
                yc1 = y1 + 0.5 + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
            } // end if

            break;

        case ClipCodeSW:

            // south hline intersection
            yc1 = clipRegionY2;
            xc1 = x1 + 0.5 + (clipRegionY2 - y1)*(x2 - x1) / (y2 - y1);

            // test if intersection is valid, of so then done, else compute next
            if(xc1 < clipRegionX || xc1 > clipRegionX2)
            {
                xc1 = clipRegionX;
                yc1 = y1 + 0.5 + (clipRegionX - x1)*(y2 - y1) / (x2 - x1);
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
            xc2 = x2 + 0.5 + (clipRegionY - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                // east vline intersection
                xc2 = clipRegionX2;
                yc2 = y2 + 0.5 + (clipRegionX2 - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeSE:
        {
            // south hline intersection
            yc2 = clipRegionY2;
            xc2 = x2 + 0.5 + (clipRegionY2 - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                // east vline intersection
                xc2 = clipRegionX2;
                yc2 = y2 + 0.5 + (clipRegionX2 - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeNW:
        {
            // north hline intersection
            yc2 = clipRegionY;
            xc2 = x2 + 0.5 + (clipRegionY - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                xc2 = clipRegionX;
                yc2 = y2 + 0.5 + (clipRegionX - x2)*(y1 - y2) / (x1 - x2);
            } // end if

        } break;

        case ClipCodeSW:
        {
            // south hline intersection
            yc2 = clipRegionY2;
            xc2 = x2 + 0.5 + (clipRegionY2 - y2)*(x1 - x2) / (y1 - y2);

            // test if intersection is valid, of so then done, else compute next
            if(xc2 < clipRegionX || xc2 > clipRegionX2)
            {
                xc2 = clipRegionX;
                yc2 = y2 + 0.5 + (clipRegionX - x2)*(y1 - y2) / (x1 - x2);
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
                POINT4D transformationResult;


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
