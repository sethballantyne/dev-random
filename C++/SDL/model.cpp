#include "model.h"
#include <cstdlib>
#include <cstring>
#include <cctype>

char* PLG_GetLine(char* buffer, int maxLength, FILE* fp)
{
    int length = 0;
    int index = 0;

    while(1)
    {
        if(!fgets(buffer, maxLength, fp))
        {
            return nullptr;
        }

        for(length = strlen(buffer), index = 0; isspace(buffer[index]); index++);

        if(index >= length || buffer[index] == '#')
            continue;

        return &buffer[index];
    }
}

int Load_OBJECT4DV1_PLG(SDL_PixelFormat* pixelFormat, OBJECT4DV1* obj, char *filename, VECTOR4D* scale, VECTOR4D* position, VECTOR4D* rotation)
{
    FILE *fp;
    char buffer[256];
    char *tokenString; // pointer to text that's ready for parsing.

    // PLG format:
    // # object descriptor
    // object_name_string num_verts_int num_polys_int

    // # vertex list
    // x0_float y0_float z0_float
    // x1_float y1_float z1_float
    // x2_float y2_float z2_float
    // .
    // .
    // xn_float yn_float zn_float
    //
    // # polygon list
    // surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int
    // .
    // .
    // surface_description_ushort num_verts_int v0_index_int v1_index_int ..  vn_index_int

    // assumes one file element per line.

    memset(obj, 0, sizeof(OBJECT4DV1));

    // object is active and visible to the player
    obj->state = OBJECT4DV1_STATE_ACTIVE | OBJECT4DV1_STATE_VISIBLE;

    obj->worldPosition.x = position->x;
    obj->worldPosition.y = position->y;
    obj->worldPosition.z = position->z;
    obj->worldPosition.w = position->w;

    errno_t error = fopen_s(&fp, filename, "r");
    if(error != 0)
    {
        fprintf(stderr, "Couldn't open PLG file %s.", filename);
        return 0;
    }

    // get the first token string, which should be the file descriptor.
    tokenString = PLG_GetLine(buffer, 255, fp);
    if(!tokenString)
    {
        fprintf(stderr, "PLG file %s has an invalid file descriptor.", filename);
        fclose(fp);
        return 0;
    }

    fprintf(stderr, "Object descriptor: %s", tokenString);
    
    // parse the models name along with the vertex and polygon count
    sscanf(tokenString, "%s %d %d", obj->name, &obj->numVertices, &obj->numPolygons);

    // load the vertex list
    for(int vertex = 0; vertex < obj->numVertices; vertex++)
    {
        // get the next vertex
        tokenString = PLG_GetLine(buffer, 255, fp);
        if(!tokenString)
        {
            fprintf(stderr, "PLG file %s has an invalid vertex list.", filename);
            fclose(fp);
            return 0;
        }

        // parse vertex list
        sscanf(tokenString, "%f %f %f", &obj->vlistLocalVertices[vertex].x, 
            &obj->vlistLocalVertices[vertex].y,
            &obj->vlistLocalVertices[vertex].z);
        obj->vlistLocalVertices[vertex].w = 1;

        // scale the vertices
        obj->vlistLocalVertices[vertex].x *= scale->x;
        obj->vlistLocalVertices[vertex].y *= scale->y;
        obj->vlistLocalVertices[vertex].z *= scale->z;

        fprintf(stderr,"\nVertex %d = %f, %f, %f, %f", 
            vertex, 
            obj->vlistLocalVertices[vertex].x,
            obj->vlistLocalVertices[vertex].y, 
            obj->vlistLocalVertices[vertex].z, 
            obj->vlistLocalVertices[vertex].w);
    }

    Compute_OBJECT4DV1_Radius(obj);

    fprintf(stderr, "\nAverage radius = %f, max radius = %f", obj->averageRadius, obj->maximumRadius);

    int polygonSurfaceDescription = 0; 
    int polygonNumberOfVertices = 0;
    char tempString[8];

    for(int polygon = 0; polygon < obj->numPolygons; polygon++)
    {
        // get the next polygon descriptor
        tokenString = PLG_GetLine(buffer, 255, fp);
        if(!tokenString)
        {
            fprintf(stderr, "PLG file %s has an invalid polygon descriptor.", filename);
            fclose(fp);
            return 0;
        }

        fprintf(stderr, "\nPolygon %d:", polygon);

        // each vertex list MUST have 3 vertices since that's what the engine
        // is supporting; All models must be constructed of triangles.
        sscanf(tokenString, "%s %d %d %d", 
            tempString,
            &polygonNumberOfVertices,
            &obj->plist[polygon].vertices[0],
            &obj->plist[polygon].vertices[1],
            &obj->plist[polygon].vertices[2]);

        // test if the surface descriptor is in hex format
        if(tempString[0] == '0' && toupper(tempString[1]) == 'X')
        {
            sscanf(tempString, "%x", &polygonSurfaceDescription);
        }
        else
        {
            polygonSurfaceDescription = atoi(tempString);
        }

        // point the polygon's vertex list to the object's vertex list
        // note that this is redundant since the polylist is contained
        // within the object in this case and its up to the user to select
        // whether the local or transformed vertex list is used when building up
        // polygon geometry, might be a better idea to set to NULL in the context
        // of polygons that are part of an object
        obj->plist[polygon].vertexList = obj->vlistLocalVertices;

        fprintf(stderr, "\nSurface Description = 0x%.4x, number of vertices = %d, vertex indices = [%d, %d, %d]",
            polygonSurfaceDescription,
            polygonNumberOfVertices,
            obj->plist[polygon].vertices[0],
            obj->plist[polygon].vertices[1],
            obj->plist[polygon].vertices[2]);

        // now we that we have the vertex list and we have entered the polygon
        // vertex index data into the polygon itself, now let's analyze the surface
        // descriptor and set the fields for the polygon based on the description

        // extract each field from the surface descriptor
        if(polygonSurfaceDescription & PLX_TWOSIDED_FLAG)
        {
            obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_TWOSIDED;
            fprintf(stderr, "\nPOLYGON4DV1_ATTR_TWOSIDED set.");
        }
        else
        {
            fprintf(stderr, "\nPOLYGON4DV1_ATTR_TWOSIDED not set. Polygon is one sided.");
        }

        // set the colour and colour type
        if(polygonSurfaceDescription & PLX_COLOUR_MODE_RGB_FLAG)
        {
            // set the polygon as a RGB 4-4-4 surface.
            obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_RGB16;

            // extract the colour and copy into the polygon's colour variable.
            int red = ((polygonSurfaceDescription & 0x0f00) >> 8);
            int green = ((polygonSurfaceDescription & 0x00f0) >> 4);
            int blue = (polygonSurfaceDescription & 0x000f);

            // despite using 32 bit types we're only aiming for 16 bit colour.
            obj->plist[polygon].colour = SDL_MapRGB(pixelFormat, red * 16, green * 16, blue * 16);

            fprintf(stderr, "\nRGB colour = [R:%d, G:%d, B:%d]", red, green, blue);
        }
        else
        {
            // surface is using 8-bit colour. 
            //obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_8BITCOLOUR;

            // last 8 bits are the colour index
            //obj->plist[polygon].colour = (polygonSurfaceDescription & 0x00ff);

            //fprintf(stderr, "\n8-bit colour index = &d", obj->plist[polygon].colour);

            fprintf(stderr, "\n8-bit colour not supported.");
            fclose(fp);
            return 0; 
        }

        // handle shading mode
        int shadeMode = (polygonSurfaceDescription & PLX_SHADE_MODE_MASK);

        switch(shadeMode)
        {
            case PLX_SHADE_MODE_PURE_FLAG:
                obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_SHADE_MODE_PURE;
                fprintf(stderr, "\nPOLYGON4DV1_ATTR_SHADE_MODE_PURE set.");
                break;

            case PLX_SHADE_MODE_FLAT_FLAG:
                obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_SHADE_MODE_FLAT;
                fprintf(stderr, "\nPOLYGON4DV1_ATTR_SHADE_MODE_FLAT set.");
                break;

            case PLX_SHADE_MODE_GOURAUD_FLAG:
                obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_SHADE_MODE_GOURAUD;
                fprintf(stderr, "\nPOLYGON4DV1_ATTR_SHADE_MODE_GOURAUD set.");
                break;

            case PLX_SHADE_MODE_PHONG_FLAG:
                obj->plist[polygon].attributes |= POLYGON4DV1_ATTR_SHADE_MODE_PHONG;
                fprintf(stderr, "\nPOLYGON4DV1_ATTR_SHADE_MODE_PHONG set.");
                break;

            default:
                break;
        }

        obj->plist[polygon].state = POLYGON4DV1_STATE_ACTIVE;
    }

    fclose(fp);

    return 1;
}

float Compute_OBJECT4DV1_Radius(OBJECT4DV1* obj)
{
    obj->averageRadius = 0;
    obj->maximumRadius = 0;

    for(int vertex = 0; vertex < obj->numVertices; vertex++)
    {
        float x = obj->vlistLocalVertices[vertex].x * obj->vlistLocalVertices[vertex].x;
        float y = obj->vlistLocalVertices[vertex].y * obj->vlistLocalVertices[vertex].y;
        float z = obj->vlistLocalVertices[vertex].z * obj->vlistLocalVertices[vertex].z;

        float distanceToVertex = sqrt(x + y + z);

        obj->averageRadius += distanceToVertex;

        if(distanceToVertex > obj->maximumRadius)
        {
            obj->maximumRadius = distanceToVertex;
        }
    }

    obj->averageRadius /= obj->numVertices;

    return obj->maximumRadius;
}

// converts the local model coordinates of the sent object into world coordinates.
void Model_To_World_OBJECT4DV1(OBJECT4DV1* object, int transformType)
{
    if(transformType == TRANSFORM_LOCAL_TO_TRANS)
    {
        for(int vertex = 0; vertex < object->numVertices; vertex++)
        {
            VECTOR4D_Add(&object->vlistLocalVertices[vertex], &object->worldPosition, &object->vlistTransformedVertices[vertex]);
        }
    }
    else
    {
        for(int vertex = 0; vertex < object->numVertices; vertex++)
        {
            VECTOR4D_Add(&object->vlistTransformedVertices[vertex], &object->worldPosition, &object->vlistTransformedVertices[vertex]);
        }
    }
}