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
