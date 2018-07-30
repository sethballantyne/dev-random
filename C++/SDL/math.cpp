#include <math.h>
#include "math.h"

float g_CosineLookupTable[361];
float g_SineLookupTable[361];

void Math_BuildLookupTables()
{
    for(int angle = 0; angle <= 360; angle++)
    {
        float theta = (float)angle * M_PI / (float)180;

        g_CosineLookupTable[angle] = cos(theta);
        g_SineLookupTable[angle] = sin(theta);
    }
}