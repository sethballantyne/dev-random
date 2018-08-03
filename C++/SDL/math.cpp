#include <math.h>
#include <stdio.h>
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

// computes the distance from 0, 0 to x, y with 3.5% error
int Fast_Distance_2D(int x, int y)
{
    // first compute the absolute value of x,y
    x = abs(x);
    y = abs(y);

    // compute the minimum of x,y
    int mn = MIN(x, y);

    // return the distance
    return(x + y - (mn >> 1) - (mn >> 2) + (mn >> 4));
}

// computes the distance from the origin to x,y,z
float Fast_Distance_3D(float fx, float fy, float fz)
{
    int temp;  // used for swaping
    int x,y,z; // used for algorithm

    // make sure values are all positive
    x = fabs(fx) * 1024;
    y = fabs(fy) * 1024;
    z = fabs(fz) * 1024;

    // sort values
    if(y < x)
    {
        SWAP(x, y, temp);
    }

    if(z < y)
    {
        SWAP(y, z, temp);
    }

    if(y < x)
    {
        SWAP(x, y, temp);
    }

    int dist = (z + 11 * (y >> 5) + (x >> 2) );

    // compute distance with 8% error
    return((float)(dist >> 10));

}

// computes the product fp_prod = fp1*fp2 using 64 bit math, so as not to loose precission
FIXP16 FIXP16_MUL(FIXP16 fp1, FIXP16 fp2)
{
    FIXP16 fp_prod; // return the product

    _asm
    {
        mov eax, fp1
        imul fp2
        shrd eax, edx, 16
    }
}

// computes the quotient fp1/fp2 using 64 bit math, so as not to loose precision
FIXP16 FIXP16_DIV(FIXP16 fp1, FIXP16 fp2)
{
    _asm
    {
        mov eax, fp1
        cdq
        shld edx, eax, 16
        sal eax, 16
        idiv fp2
    }
}

void FIXP16_Print(FIXP16 fp)
{
    // this function prints out a fixed point number
    printf("\nfp=%f", (float)(fp) / FIXP16_MAG);

} // end FIXP16_Print

// adds to quaternions
void QUAT_Add(QUAT* q1, QUAT* q2, QUAT* qsum)
{
    qsum->qv.x = q1->qv.x + q2->qv.x;
    qsum->qv.y = q1->qv.y + q2->qv.y;
    qsum->qv.z = q1->qv.z + q2->qv.z;
    qsum->q0 = q1->q0 + q2->q0;

}

// subtracts two quaternions, q1 - q2
void QUAT_Sub(QUAT* q1, QUAT* q2, QUAT* qdiff)
{
    qdiff->qv.x = q1->qv.x - q2->qv.x;
    qdiff->qv.y = q1->qv.y - q2->qv.y;
    qdiff->qv.z = q1->qv.z - q2->qv.z;
    qdiff->q0 = q1->q0 - q2->q0;

}

// computes the conjugate of a quaternion
void QUAT_Conjugate(QUAT* q, QUAT* qconj)
{
    qconj->qv.x = -q->qv.x;
    qconj->qv.y = -q->qv.y;
    qconj->qv.z = -q->qv.z;
    qconj->q0 = q->q0;
} 

// scales a quaternion and returns it
void QUAT_Scale(QUAT* q, float scale, QUAT* qs)
{ 
    qs->qv.x = scale * q->qv.x;
    qs->qv.y = scale * q->qv.y;
    qs->qv.z = scale * q->qv.z;
    qs->q0 = scale * q->q0;
}

// scales a quaternion in place
void QUAT_Scale(QUAT* q, float scale)
{
    q->qv.x *= scale;
    q->qv.y *= scale;
    q->qv.z *= scale;
    q->q0 *= scale;
}

// returns length or norm of a quaterion
float QUAT_Norm(QUAT* q)
{
    return(sqrtf(q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z));
}

// returns the length or norm of a quaterion squared
float QUAT_Norm2(QUAT* q)
{
    return(q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z);
}

// normalizes the sent quaternion and returns it via qn
void QUAT_Normalize(QUAT* q, QUAT* qn)
{
    // this functions normalizes the sent quaternion and 
    // returns it

    // compute 1/length
    float qlength_inv = 1.0 / (sqrtf(q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z));

    // now normalize
    qn->q0 = q->q0 * qlength_inv;
    qn->qv.x = q->qv.x * qlength_inv;
    qn->qv.y = q->qv.y * qlength_inv;
    qn->qv.z = q->qv.z * qlength_inv;
}

// normalizes the sent quaternion in place
void QUAT_Normalize(QUAT* q)
{
    // compute length
    float qlength_inv = 1.0 / (sqrtf(q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z));

    // now normalize
    q->q0 *= qlength_inv;
    q->qv.x *= qlength_inv;
    q->qv.y *= qlength_inv;
    q->qv.z *= qlength_inv;
}

// computes the inverse of a unit quaternion and returns the result via qi
void QUAT_Unit_Inverse(QUAT* q, QUAT* qi)
{
    qi->q0 = q->q0;
    qi->qv.x = -q->qv.x;
    qi->qv.y = -q->qv.y;
    qi->qv.z = -q->qv.z;
}

// computes the inverse of a unit quaternion in place
void QUAT_Unit_Inverse(QUAT* q)
{
    q->qv.x = -q->qv.x;
    q->qv.y = -q->qv.y;
    q->qv.z = -q->qv.z;
} 

// computes the inverse of a general quaternion and returns result in general, q-1 = *q/|q|2
void QUAT_Inverse(QUAT* q, QUAT* qi)
{
    float norm2_inv = 1.0 / (q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z);

    qi->q0 = q->q0 * norm2_inv;
    qi->qv.x = -q->qv.x * norm2_inv;
    qi->qv.y = -q->qv.y * norm2_inv;
    qi->qv.z = -q->qv.z * norm2_inv;

}

// computes the inverse of a general quaternion in place in general, q-1 = *q/|q|2
void QUAT_Inverse(QUAT* q)
{
    float norm2_inv = 1.0 / (q->q0 * q->q0 + q->qv.x * q->qv.x + q->qv.y * q->qv.y + q->qv.z * q->qv.z);

    q->q0 = q->q0 * norm2_inv;
    q->qv.x = -q->qv.x * norm2_inv;
    q->qv.y = -q->qv.y * norm2_inv;
    q->qv.z = -q->qv.z * norm2_inv;
}

// multiplies two quaternions and returns the result in qprod
void QUAT_Mul(QUAT* q1, QUAT* q2, QUAT* qprod)
{
    // this is the brute force method
    //qprod->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
    //qprod->x = q1->w*q2->x + q1->x*q2->w + q1->y*q2->z - q1->z*q2->y;
    //qprod->y = q1->w*q2->y - q1->x*q2->z + q1->y*q2->w - q1->z*q2->x;
    //qprod->z = q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w;

    // this method was arrived at basically by trying to factor the above
    // expression to reduce the # of multiplies

    float prd_0 = (q1->qv.z - q1->qv.y) * (q2->qv.y - q2->qv.z);
    float prd_1 = (q1->q0 + q1->qv.x) * (q2->q0 + q2->qv.x);
    float prd_2 = (q1->q0 - q1->qv.x) * (q2->qv.y + q2->qv.z);
    float prd_3 = (q1->qv.y + q1->qv.z) * (q2->q0 - q2->qv.x);
    float prd_4 = (q1->qv.z - q1->qv.x) * (q2->qv.x - q2->qv.y);
    float prd_5 = (q1->qv.z + q1->qv.x) * (q2->qv.x + q2->qv.y);
    float prd_6 = (q1->q0 + q1->qv.y) * (q2->q0 - q2->qv.z);
    float prd_7 = (q1->q0 - q1->qv.y) * (q2->q0 + q2->qv.z);

    float prd_8 = prd_5 + prd_6 + prd_7;
    float prd_9 = 0.5 * (prd_4 + prd_8);

    // and finally build up the result with the temporary product
    qprod->q0 = prd_0 + prd_9 - prd_5;
    qprod->qv.x = prd_1 + prd_9 - prd_8;
    qprod->qv.y = prd_2 + prd_9 - prd_7;
    qprod->qv.z = prd_3 + prd_9 - prd_6;
}

// computes q1*q2*q3 in that order and returns the results in qprod
void QUAT_Triple_Product(QUAT* q1, QUAT* q2, QUAT* q3, QUAT* qprod)
{
    QUAT qtmp;
    QUAT_Mul(q1, q2, &qtmp);
    QUAT_Mul(&qtmp, q3, qprod);
}

// initializes a quaternion based on a 3d direction vector and angle
// note the direction vector must be a unit vector
// and the angle is in radians
void VECTOR3D_Theta_To_QUAT(QUAT* q, VECTOR3D* v, float theta)
{
    float theta_div_2 = (0.5) * theta;
    float sinf_theta = sinf(theta_div_2);

    q->qv.x = sinf_theta * v->x;
    q->qv.y = sinf_theta * v->y;
    q->qv.z = sinf_theta * v->z;
    q->q0 = cosf(theta_div_2);
}

// initializes a quaternion based on a 4d direction vector and angle
// note the direction vector must be a unit vector
// and the angle is in rads
void VECTOR4D_Theta_To_QUAT(QUAT* q, VECTOR4D* v, float theta)
{
    float theta_div_2 = (0.5) * theta; 
    float sinf_theta = sinf(theta_div_2);

    q->qv.x = sinf_theta * v->x;
    q->qv.y = sinf_theta * v->y;
    q->qv.z = sinf_theta * v->z;
    q->q0 = cosf(theta_div_2);
}

// intializes a quaternion based on the zyx multiplication order of 
// the angles that are parallel to the zyx axis respectively
void EulerZYX_To_QUAT(QUAT* q, float theta_z, float theta_y, float theta_x)
{
    float cos_z_2 = 0.5 * cosf(theta_z);
    float cos_y_2 = 0.5 * cosf(theta_y);
    float cos_x_2 = 0.5 * cosf(theta_x);

    float sin_z_2 = 0.5 * sinf(theta_z);
    float sin_y_2 = 0.5 * sinf(theta_y);
    float sin_x_2 = 0.5 * sinf(theta_x);

    // and now compute quaternion
    q->q0 = cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2;
    q->qv.x = cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2  *cos_x_2;
    q->qv.y = cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2;
    q->qv.z = sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2;
}

// converts a unit quaternion into a unit direction vector and rotation angle about that vector
void QUAT_To_VECTOR3D_Theta(QUAT* q, VECTOR3D* v, float *theta)
{
    *theta = acosf(q->q0);

    float sinf_theta_inv = 1.0 / sinf(*theta);

    v->x = q->qv.x * sinf_theta_inv;
    v->y = q->qv.y * sinf_theta_inv;
    v->z = q->qv.z * sinf_theta_inv;

    *theta *= 2;
}

void QUAT_Print(QUAT* q, char *name = "q")
{
    // this function prints out a quaternion
    printf("\n%s=%f+[(%f)i + (%f)j + (%f)k]",
        name, q->q0, q->qv.x, q->qv.y, q->qv.z);
}

// uses g_SineLookupTable[], but has logic to handle 
// negative angles as well as fractional angles via interpolation
float Fast_Sin(float theta)
{
    // convert angle to 0-359
    theta = fmodf(theta, 360);

    // make angle positive
    if(theta < 0)
    {
        theta += 360.0;
    }

    // compute floor of theta and fractional part to interpolate
    int theta_int = (int)theta;
    float theta_frac = theta - theta_int;

    // now compute the value of sin(angle) using the lookup tables
    // and interpolating the fractional part, note that if theta_int
    // is equal to 359 then theta_int+1=360, but this is fine since the
    // table was made with the entries 0-360 inclusive
    return(g_SineLookupTable[theta_int] +
        theta_frac * (g_SineLookupTable[theta_int + 1] - g_SineLookupTable[theta_int]));

}

// uses the g_CosineLookupTable[], but has logic to handle negative 
// angles as well as fractional angles via interpolation
float Fast_Cos(float theta)
{
    // convert angle to 0-359
    theta = fmodf(theta, 360);

    // make angle positive
    if(theta < 0)
    {
        theta += 360.0;
    }

    // compute floor of theta and fractional part to interpolate
    int theta_int = (int)theta;
    float theta_frac = theta - theta_int;

    // now compute the value of sin(angle) using the lookup tables
    // and interpolating the fractional part, note that if theta_int
    // is equal to 359 then theta_int+1=360, but this is fine since the
    // table was made with the entries 0-360 inclusive
    return(g_CosineLookupTable[theta_int] + 
        theta_frac * (g_CosineLookupTable[theta_int + 1] - g_CosineLookupTable[theta_int]));
}

// convert polar to rectangular
void POLAR2D_To_POINT2D(POLAR2D* polar, POINT2D* rect)
{
    rect->x = polar->r * cosf(polar->theta);
    rect->y = polar->r * sinf(polar->theta);
}

// convert polar to rectangular
void POLAR2D_To_RectXY(POLAR2D* polar, float *x, float *y)
{
    *x = polar->r * cosf(polar->theta);
    *y = polar->r * sinf(polar->theta);
} 

// convert rectangular to polar
void POINT2D_To_POLAR2D(POINT2D* rect, POLAR2D* polar)
{
    polar->r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    polar->theta = atanf(rect->y / rect->x);
}

// convert rectangular to polar coordinates
void POINT2D_To_PolarRTh(POINT2D* rect, float *r, float *theta)
{
    *r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    *theta = atanf(rect->y / rect->x);
}

// convert cylindrical to rectangular coordinates
void CYLINDRICAL3D_To_POINT3D(CYLINDRICAL3D* cyl, POINT3D* rect)
{
    rect->x = cyl->r * cosf(cyl->theta);
    rect->y = cyl->r * sinf(cyl->theta);
    rect->z = cyl->z;
}

// convert cylindrical to rectangular coordinates
void CYLINDRICAL3D_To_RectXYZ(CYLINDRICAL3D* cyl, float *x, float *y, float *z)
{
    *x = cyl->r * cosf(cyl->theta);
    *y = cyl->r * sinf(cyl->theta);
    *z = cyl->z;
}

// convert 3d rectangular coordinates to cylindrical coordinates
void POINT3D_To_CYLINDRICAL3D(POINT3D* rect, CYLINDRICAL3D* cyl)
{
    cyl->r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    cyl->theta = atanf(rect->y / rect->x);
    cyl->z = rect->z;
}

// convert 3d rectangular coordinates to cylindrical coordinates
void POINT3D_To_CylindricalRThZ(POINT3D* rect, float *r, float *theta, float *z)
{
    *r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    *theta = atanf(rect->y / rect->x);
    *z = rect->z;
}

// convert from spherical coordinates to 3d rectangular coordinates
void SPHERICAL3D_To_POINT3D(SPHERICAL3D* sph, POINT3D* rect)
{
    // pre-compute r, and z
    float r = sph->p * sinf(sph->phi);
    rect->z = sph->p * cosf(sph->phi);

    // use r to simplify computation of x,y
    rect->x = r * cosf(sph->theta);
    rect->y = r * sinf(sph->theta);
}

// convert spherical to 3d rectangular coordinates
void SPHERICAL3D_To_RectXYZ(SPHERICAL3D* sph, float *x, float *y, float *z)
{
    // pre-compute r, and z
    float r = sph->p * sinf(sph->phi);
    *z = sph->p * cosf(sph->phi);

    // use r to simplify computation of x,y
    *x = r * cosf(sph->theta);
    *y = r * sinf(sph->theta);
}

// convert 3d rectangular coordinates to spherical coordinates
void POINT3D_To_SPHERICAL3D(POINT3D* rect, SPHERICAL3D* sph)
{
    sph->p = sqrtf((rect->x * rect->x) + (rect->y * rect->y) + (rect->z * rect->z));

    sph->theta = atanf(rect->y / rect->x);

    // we need r to compute phi
    float r = sph->p * sinf(sph->phi);

    sph->phi = asinf(r / sph->p);
}

// convert rectangular 3d coordinates to spherical
void POINT3D_To_SphericalPThPh(POINT3D* rect, float *p, float *theta, float *phi)
{
    // convert rectangular to spherical
    *p = sqrtf((rect->x * rect->x) + (rect->y * rect->y) + (rect->z * rect->z));
    *theta = atanf(rect->y / rect->x);

    // we need r to compute phi
    float r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    *phi = asinf(r / (*p));
}

// adds va + vb and return it in vsum
void VECTOR2D_Add(VECTOR2D* va, VECTOR2D* vb, VECTOR2D* vsum)
{
    vsum->x = va->x + vb->x;
    vsum->y = va->y + vb->y;
}

// adds va + vb and returns the result on the stack
VECTOR2D VECTOR2D_Add(VECTOR2D* va, VECTOR2D* vb)
{ 
    VECTOR2D vsum;

    vsum.x = va->x + vb->x;
    vsum.y = va->y + vb->y;

    return vsum;
}

// subtracts va - vb and return it in vdiff
void VECTOR2D_Sub(VECTOR2D* va, VECTOR2D* vb, VECTOR2D* vdiff)
{
    vdiff->x = va->x - vb->x;
    vdiff->y = va->y - vb->y;
}

// subtracts va - vb and returns the result on the stack
VECTOR2D VECTOR2D_Sub(VECTOR2D* va, VECTOR2D* vb)
{
    VECTOR2D vdiff;

    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;

    return vdiff;
}

// scales a vector by the constant k, leaves the original unchanged, and returns the result
// in vscaled
void VECTOR2D_Scale(float k, VECTOR2D* va, VECTOR2D* vscaled)
{
    // multiply each component by scaling factor
    vscaled->x = k * va->x;
    vscaled->y = k * va->y;
}

// scales a vector by the constant k, by scaling the original
void VECTOR2D_Scale(float k, VECTOR2D* va)
{
    // multiply each component by scaling factor
    va->x *= k;
    va->y *= k;
}

// computes the dot product between va and vb
float VECTOR2D_Dot(VECTOR2D* va, VECTOR2D* vb)
{
    return((va->x * vb->x) + (va->y * vb->y));
}

float VECTOR2D_Length(VECTOR2D* va)
{
    // computes the magnitude of a vector, slo
    return(sqrtf(va->x * va->x + va->y * va->y));
}

// computes the magnitude of a vector using an approximation
float VECTOR2D_Length_Fast(VECTOR2D* va)
{
    // very fast
    return((float)Fast_Distance_2D(va->x, va->y));
}

// normalizes the sent vector in place
void VECTOR2D_Normalize(VECTOR2D* va)
{
    // compute length
    float length = sqrtf(va->x * va->x + va->y * va->y);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1 / length;

    // compute normalized version of vector
    va->x = va->x * length_inv;
    va->y = va->y * length_inv;
}

// normalizes the sent vector and returns the result in vn
void VECTOR2D_Normalize(VECTOR2D* va, VECTOR2D* vn)
{
    VECTOR2D_ZERO(vn);

    // compute length
    float length = (float)sqrtf(va->x * va->x + va->y * va->y);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1 / length;

    // compute normalized version of vector
    vn->x = va->x * length_inv;
    vn->y = va->y * length_inv;
}

// function creates a vector from two vectors (or points) in 3D space
void VECTOR2D_Build(VECTOR2D* init, VECTOR2D* term, VECTOR2D* result)
{
    result->x = term->x - init->x;
    result->y = term->y - init->y;
}

// returns the cosine of the angle between two vectors.
float VECTOR2D_CosTh(VECTOR2D* va, VECTOR2D* vb)
{
    return(VECTOR2D_Dot(va, vb) / (VECTOR2D_Length(va) * VECTOR2D_Length(vb)));
}

// prints out a VECTOR2D to stdout
void VECTOR2D_Print(VECTOR2D* va, char *name = "v")
{
    printf("\n%s=[%f, %f]", name, va->x, va->y);
}

// adds va+vb and return it in vsum
void VECTOR3D_Add(VECTOR3D* va, VECTOR3D* vb, VECTOR3D* vsum)
{
    vsum->x = va->x + vb->x;
    vsum->y = va->y + vb->y;
    vsum->z = va->z + vb->z;
}

// adds va+vb and returns the result on the stack
VECTOR3D VECTOR3D_Add(VECTOR3D* va, VECTOR3D* vb)
{
    VECTOR3D vsum;

    vsum.x = va->x + vb->x;
    vsum.y = va->y + vb->y;
    vsum.z = va->z + vb->z;

    return vsum;
}

// subtracts va-vb and return it in vdiff the stack
void VECTOR3D_Sub(VECTOR3D* va, VECTOR3D* vb, VECTOR3D* vdiff)
{
    vdiff->x = va->x - vb->x;
    vdiff->y = va->y - vb->y;
    vdiff->z = va->z - vb->z;
}

// subtracts va - vb and returns the result on the stack
VECTOR3D VECTOR3D_Sub(VECTOR3D* va, VECTOR3D* vb)
{
    VECTOR3D vdiff;

    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;
    vdiff.z = va->z - vb->z;

    return vdiff;
}

// scales a vector by the constant k, and modifies the original
void VECTOR3D_Scale(float k, VECTOR3D* va)
{
    va->x *= k;
    va->y *= k;
    va->z *= k;
}

// scales a vector by the constant k, leaves the original unchanged, 
// and returns the result in vscaled as well as on the stack
void VECTOR3D_Scale(float k, VECTOR3D* va, VECTOR3D* vscaled)
{
    // multiply each component by scaling factor
    vscaled->x = k*va->x;
    vscaled->y = k*va->y;
    vscaled->z = k*va->z;
}

// computes the dot product between va and vb
float VECTOR3D_Dot(VECTOR3D* va, VECTOR3D* vb)
{
    return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
}

// computes the cross product between va and vb
// and returns the vector that is perpendicular to each in vn
void VECTOR3D_Cross(VECTOR3D* va, VECTOR3D* vb, VECTOR3D*vn)
{
    vn->x = ((va->y * vb->z) - (va->z * vb->y));
    vn->y = -((va->x * vb->z) - (va->z * vb->x));
    vn->z = ((va->x * vb->y) - (va->y * vb->x));
}

// computes the magnitude of a vector, slow
float VECTOR3D_Length(VECTOR3D* va)
{
    return((float)sqrtf(va->x * va->x + va->y * va->y + va->z * va->z));
}

// computes the magnitude of a vector using an approximation, very fast
float VECTOR3D_Length_Fast(VECTOR3D* va)
{
    return Fast_Distance_3D(va->x, va->y, va->z);
}

// normalizes the sent vector
void VECTOR3D_Normalize(VECTOR3D* va)
{
    // compute length
    float length = sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1 / length;

    // compute normalized version of vector
    va->x *= length_inv;
    va->y *= length_inv;
    va->z *= length_inv;
}

// normalizes the sent vector and returns the result in vn
void VECTOR3D_Normalize(VECTOR3D* va, VECTOR3D* vn)
{
    VECTOR3D_ZERO(vn);

    // compute length
    float length = VECTOR3D_Length(va);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1.0 / length;

    // compute normalized version of vector
    vn->x = va->x*length_inv;
    vn->y = va->y*length_inv;
    vn->z = va->z*length_inv;
}

// creates a vector from two vectors (or points) in 3D space
void VECTOR3D_Build(VECTOR3D* init, VECTOR3D* term, VECTOR3D* result)
{
    result->x = term->x - init->x;
    result->y = term->y - init->y;
    result->z = term->z - init->z;

}

// returns the cosine of the angle between two vectors. 
float VECTOR3D_CosTh(VECTOR3D* va, VECTOR3D* vb)
{
    return(VECTOR3D_Dot(va, vb) / (VECTOR3D_Length(va)*VECTOR3D_Length(vb)));
}

// prints the vector to stdout
void VECTOR3D_Print(VECTOR3D* va, char *name = "v")
{
    printf("\n%s=[%f, %f, %f]", name, va->x, va->y, va->z);
}

// these are the 4D version of the vector functions, they
// assume that the vectors are 3D with a w, so w is left
// out of all the operations

// builds a 4D vector
void VECTOR4D_Build(VECTOR4D* init, VECTOR4D* term, VECTOR4D* result)
{
    result->x = term->x - init->x;
    result->y = term->y - init->y;
    result->z = term->z - init->z;
    result->w = 1;
}

// this function adds va+vb and return it in vsum
void VECTOR4D_Add(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vsum)
{
    vsum->x = va->x + vb->x;
    vsum->y = va->y + vb->y;
    vsum->z = va->z + vb->z;
    vsum->w = 1;
}

// subtracts va-vb and return it in vdiff the stack
void VECTOR4D_Sub(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vdiff)
{
    vdiff->x = va->x - vb->x;
    vdiff->y = va->y - vb->y;
    vdiff->z = va->z - vb->z;
    vdiff->w = 1;
}

// subtracts va-vb and returns the result on the stack
VECTOR4D VECTOR4D_Sub(VECTOR4D* va, VECTOR4D* vb)
{
    VECTOR4D vdiff;

    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;
    vdiff.z = va->z - vb->z;
    vdiff.w = 1;

    return vdiff;
}

// scales a vector by the constant k, in place , note w is left unchanged
void VECTOR4D_Scale(float k, VECTOR4D* va)
{
    va->x *= k;
    va->y *= k;
    va->z *= k;
    va->w = 1;
}

// scales a vector by the constant k, leaves the original unchanged, 
// and returns the result in vscaled
void VECTOR4D_Scale(float k, VECTOR4D* va, VECTOR4D* vscaled)
{
    vscaled->x = k * va->x;
    vscaled->y = k * va->y;
    vscaled->z = k * va->z;
    vscaled->w = 1;
}

// computes the dot product between va and vb
float VECTOR4D_Dot(VECTOR4D* va, VECTOR4D* vb)
{
    return((va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z));
}

// computes the cross product between va and vb
// and returns the vector that is perpendicular to each in vn
void VECTOR4D_Cross(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vn)
{
    vn->x = ((va->y * vb->z) - (va->z * vb->y));
    vn->y = -((va->x * vb->z) - (va->z * vb->x));
    vn->z = ((va->x * vb->y) - (va->y * vb->x));
    vn->w = 1;
}

// computes the cross product between va and vb
// and returns the vector that is perpendicular to each
VECTOR4D VECTOR4D_Cross(VECTOR4D* va, VECTOR4D* vb)
{
    VECTOR4D vn;

    vn.x = ((va->y * vb->z) - (va->z * vb->y));
    vn.y = -((va->x * vb->z) - (va->z * vb->x));
    vn.z = ((va->x * vb->y) - (va->y * vb->x));
    vn.w = 1;

    return vn;
}

// computes the magnitude of a vector, slow
float VECTOR4D_Length(VECTOR4D* va)
{
    return(sqrtf(va->x * va->x + va->y * va->y + va->z * va->z));
}

// computes the magnitude of a vector using an approximation
// very fast
float VECTOR4D_Length_Fast(VECTOR4D* va)
{
    return Fast_Distance_3D(va->x, va->y, va->z);
}

// normalizes the sent vector and returns the result
void VECTOR4D_Normalize(VECTOR4D* va)
{
    // compute length
    float length = sqrtf(va->x*va->x + va->y*va->y + va->z*va->z);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1.0 / length;

    // compute normalized version of vector
    va->x *= length_inv;
    va->y *= length_inv;
    va->z *= length_inv;
    va->w = 1;
}

// normalizes the sent vector and returns the result in vn
void VECTOR4D_Normalize(VECTOR4D* va, VECTOR4D* vn)
{
    VECTOR4D_ZERO(vn);

    // compute length
    float length = sqrt(va->x * va->x + va->y * va->y + va->z * va->z);

    // test for zero length vector
    // if found return zero vector
    if(length < EPSILON_E5)
        return;

    float length_inv = 1.0 / length;

    // compute normalized version of vector
    vn->x = va->x * length_inv;
    vn->y = va->y * length_inv;
    vn->z = va->z * length_inv;
    vn->w = 1;
}

// returns the cosine of the angle between two vectors.
float VECTOR4D_CosTh(VECTOR4D* va, VECTOR4D* vb)
{
    return(VECTOR4D_Dot(va, vb) / (VECTOR4D_Length(va)*VECTOR4D_Length(vb)));
}

// prints out a VECTOR4D to stdout
void VECTOR4D_Print(VECTOR4D* va, char *name = "v")
{
    printf("\n%s[%f, %f, %f, %f]", name, va->x, va->y, va->z, va->w);
}

// fills a 2x2 matrix with the sent data in row major form
void Mat_Init_2X2(MATRIX2X2* ma, float m00, float m01, float m10, float m11)
{
    ma->matrix[0][0] = m00; 
    ma->matrix[0][1] = m01;
    ma->matrix[1][0] = m10; 
    ma->matrix[1][1] = m11;
}

// adds two 2x2 matrices together and stores the result in msum
void Mat_Add_2X2(MATRIX2X2* ma, MATRIX2X2* mb, MATRIX2X2* msum)
{ 
    msum->matrix[0][0] = ma->matrix[0][0] + mb->matrix[0][0];
    msum->matrix[0][1] = ma->matrix[0][1] + mb->matrix[0][1];
    msum->matrix[1][0] = ma->matrix[1][0] + mb->matrix[1][0];
    msum->matrix[1][1] = ma->matrix[1][1] + mb->matrix[1][1];
}

// multiplies two 2x2 matrices together and stores the result in mprod
void Mat_Mul_2X2(MATRIX2X2* ma, MATRIX2X2* mb, MATRIX2X2* mprod)
{
    mprod->matrix[0][0] = ma->matrix[0][0] * mb->matrix[0][0] + ma->matrix[0][1] * mb->matrix[1][0];
    mprod->matrix[0][1] = ma->matrix[0][0] * mb->matrix[0][1] + ma->matrix[0][1] * mb->matrix[1][1];
    mprod->matrix[1][0] = ma->matrix[1][0] * mb->matrix[0][0] + ma->matrix[1][1] * mb->matrix[1][0];
    mprod->matrix[1][1] = ma->matrix[1][0] * mb->matrix[0][1] + ma->matrix[1][1] * mb->matrix[1][1];
}

// computes the inverse of a 2x2 matrix and stores the result in mi
int Mat_Inverse_2X2(MATRIX2X2* m, MATRIX2X2* mi)
{
    // compute determinate
    float det = (m->matrix[0][0] * m->matrix[1][1] - m->matrix[0][1] * m->matrix[1][0]);

    // if determinate is 0 then inverse doesn't exist
    if(fabs(det) < EPSILON_E5)
        return 0;

    float det_inv = 1.0 / det;

    // fill in inverse by formula
    mi->matrix[0][0] = m->matrix[1][1] * det_inv;
    mi->matrix[0][1] = -m->matrix[0][1] * det_inv;
    mi->matrix[1][0] = -m->matrix[1][0] * det_inv;
    mi->matrix[1][1] = m->matrix[0][0] * det_inv;

    // return sucess
    return 1;

}

// prints out a 2x2 matrix
void Print_Mat_2X2(MATRIX2X2* ma, char *name = "M")
{
    printf("\n%s=\n", name);

    for(int r = 0; r < 2; r++, printf("\n"))
        for(int c = 0; c < 2; c++)
            printf("%f ", ma->matrix[r][c]);
}

// computes the determinate of a 2x2 matrix
float Mat_Det_2X2(MATRIX2X2* m)
{
    return(m->matrix[0][0] * m->matrix[1][1] - m->matrix[0][1] * m->matrix[1][0]);
}

// solves the system AX = B and computes X = A(-1)*B
// by using cramers rule and determinates
int Solve_2X2_System(MATRIX2X2* A, MATRIX1X2* X, MATRIX1X2* B)
{
    // step 1: compute determinate of A
    float det_A = Mat_Det_2X2(A);

    // test if det(a) is zero, if so then there is no solution
    if(fabs(det_A) < EPSILON_E5)
        return(0);

    // step 2: create x,y numerator matrices by taking A and
    // replacing each column of it with B(transpose) and solve
    MATRIX2X2 work_mat; // working matrix

    // solve for x /////////////////

    // copy A into working matrix
    MAT_COPY_2X2(A, &work_mat);

    // swap out column 0 (x column)
    MAT_COLUMN_SWAP_2X2(&work_mat, 0, B);

    // compute determinate of A with B swapped into x column
    float det_ABx = Mat_Det_2X2(&work_mat);

    // now solve for X00
    X->matrix[0] = det_ABx / det_A;

    // solve for y /////////////////

    // copy A into working matrix
    MAT_COPY_2X2(A, &work_mat);

    // swap out column 1 (y column)
    MAT_COLUMN_SWAP_2X2(&work_mat, 1, B);

    // compute determinate of A with B swapped into y column
    float det_ABy = Mat_Det_2X2(&work_mat);

    // now solve for X01
    X->matrix[1] = det_ABy / det_A;

    // return success
    return(1);
}

// adds two 3x3 matrices together and stores the result
void Mat_Add_3X3(MATRIX3X3* ma, MATRIX3X3* mb, MATRIX3X3* msum)
{
    for(int row = 0; row<3; row++)
    {
        for(int col = 0; col<3; col++)
        {
            // insert resulting row,col element
            msum->matrix[row][col] = ma->matrix[row][col] + mb->matrix[row][col];
        } 
    } 
}

// multiplies a VECTOR3D against a 3x3 matrix - ma*mb and stores the result in mprod
void Mat_Mul_VECTOR3D_3X3(VECTOR3D* va, MATRIX3X3* mb, VECTOR3D*  vprod)
{
    vprod->x = (va->x * mb->matrix[0][0]) + (va->y * mb->matrix[1][0]) + (va->z * mb->matrix[2][0]);
    vprod->y = (va->x * mb->matrix[0][1]) + (va->y * mb->matrix[1][1]) + (va->z * mb->matrix[2][1]);
    vprod->z = (va->x * mb->matrix[0][2]) + (va->y * mb->matrix[1][2]) + (va->z * mb->matrix[2][2]);
}

// fills a 3x3 matrix with the sent data in row major form
void Mat_Init_3X3(MATRIX3X3* ma, float m00, float m01, float m02,
                  float m10, float m11, float m12,
                  float m20, float m21, float m22)
{
    ma->matrix[0][0] = m00; 
    ma->matrix[0][1] = m01; 
    ma->matrix[0][2] = m02;
    ma->matrix[1][0] = m10; 
    ma->matrix[1][1] = m11; 
    ma->matrix[1][2] = m12;
    ma->matrix[2][0] = m20; 
    ma->matrix[2][1] = m21; 
    ma->matrix[2][2] = m22;
}

// this function computes the inverse of a 3x3
int Mat_Inverse_3X3(MATRIX3X3* m, MATRIX3X3* mi)
{
    // first compute the determinate to see if there is 
    // an inverse
    float det = m->matrix[0][0] * (m->matrix[1][1] * m->matrix[2][2] - m->matrix[2][1] * m->matrix[1][2]) -
                                   m->matrix[0][1] * (m->matrix[1][0] * m->matrix[2][2] - m->matrix[2][0] * m->matrix[1][2]) +
                                   m->matrix[0][2] * (m->matrix[1][0] * m->matrix[2][1] - m->matrix[2][0] * m->matrix[1][1]);

    if(fabs(det) < EPSILON_E5)
        return(0);

    // compute inverse to save divides
    float det_inv = 1.0 / det;

    // compute inverse using m-1 = adjoint(m)/det(m)
    mi->matrix[0][0] = det_inv * (m->matrix[1][1] * m->matrix[2][2] - m->matrix[2][1] * m->matrix[1][2]);
    mi->matrix[1][0] = -det_inv * (m->matrix[1][0] * m->matrix[2][2] - m->matrix[2][0] * m->matrix[1][2]);
    mi->matrix[2][0] = det_inv * (m->matrix[1][0] * m->matrix[2][1] - m->matrix[2][0] * m->matrix[1][1]);

    mi->matrix[0][1] = -det_inv * (m->matrix[0][1] * m->matrix[2][2] - m->matrix[2][1] * m->matrix[0][2]);
    mi->matrix[1][1] = det_inv * (m->matrix[0][0] * m->matrix[2][2] - m->matrix[2][0] * m->matrix[0][2]);
    mi->matrix[2][1] = -det_inv * (m->matrix[0][0] * m->matrix[2][1] - m->matrix[2][0] * m->matrix[0][1]);

    mi->matrix[0][2] = det_inv * (m->matrix[0][1] * m->matrix[1][2] - m->matrix[1][1] * m->matrix[0][2]);
    mi->matrix[1][2] = -det_inv * (m->matrix[0][0] * m->matrix[1][2] - m->matrix[1][0] * m->matrix[0][2]);
    mi->matrix[2][2] = det_inv * (m->matrix[0][0] * m->matrix[1][1] - m->matrix[1][0] * m->matrix[0][1]);

    return 1;

}

// prints the specified 3x3 matrix
void Print_Mat_3X3(MATRIX3X3* ma, char *name = "M")
{
    // prints out a 3x3 matrix
    printf("\n%s=\n", name);

    for(int r = 0; r < 3; r++, printf("\n"))
        for(int c = 0; c < 3; c++)
            printf("%f ", ma->matrix[r][c]);
}

// computes the determinate of a 3x3 matrix using co-factor
// expansion
float Mat_Det_3X3(MATRIX3X3* m)
{
    return(m->matrix[0][0] * (m->matrix[1][1] * m->matrix[2][2] - m->matrix[2][1] * m->matrix[1][2]) -
        m->matrix[0][1] * (m->matrix[1][0] * m->matrix[2][2] - m->matrix[2][0] * m->matrix[1][2]) +
        m->matrix[0][2] * (m->matrix[1][0] * m->matrix[2][1] - m->matrix[2][0] * m->matrix[1][1]));
}

// solves the system AX=B and computes X=A(-1)*B
// by using cramers rule and determinates
int Solve_3X3_System(MATRIX3X3* A, MATRIX1X3* X, MATRIX1X3* B)
{
    // step 1: compute determinate of A
    float det_A = Mat_Det_3X3(A);

    // test if det(a) is zero, if so then there is no solution
    if(fabs(det_A) < EPSILON_E5)
        return(0);

    // step 2: create x,y,z numerator matrices by taking A and
    // replacing each column of it with B(transpose) and solve
    MATRIX3X3 work_mat; // working matrix

    // solve for x /////////////////

    // copy A into working matrix
    MAT_COPY_3X3(A, &work_mat);

    // swap out column 0 (x column)
    MAT_COLUMN_SWAP_3X3(&work_mat, 0, B);

    // compute determinate of A with B swapped into x column
    float det_ABx = Mat_Det_3X3(&work_mat);

    // now solve for X00
    X->matrix[0] = det_ABx / det_A;

    // solve for y /////////////////

    // copy A into working matrix
    MAT_COPY_3X3(A, &work_mat);

    // swap out column 1 (y column)
    MAT_COLUMN_SWAP_3X3(&work_mat, 1, B);

    // compute determinate of A with B swapped into y column
    float det_ABy = Mat_Det_3X3(&work_mat);

    // now solve for X01
    X->matrix[1] = det_ABy / det_A;

    // solve for z /////////////////

    // copy A into working matrix
    MAT_COPY_3X3(A, &work_mat);

    // swap out column 2 (z column)
    MAT_COLUMN_SWAP_3X3(&work_mat, 2, B);

    // compute determinate of A with B swapped into z column
    float det_ABz = Mat_Det_3X3(&work_mat);

    // now solve for X02
    X->matrix[2] = det_ABz / det_A;

    // return success
    return 1;
}

// adds two 4x4 matrices together and stores the result
void Mat_Add_4X4(MATRIX4X4* ma, MATRIX4X4* mb, MATRIX4X4* msum)
{
    for(int row = 0; row < 4; row++)
    {
        for(int col = 0; col < 4; col++)
        {
            // insert resulting row,col element
            msum->matrix[row][col] = ma->matrix[row][col] + mb->matrix[row][col];
        } 
    }
}

// multiplies two 4x4 matrices together and stores the result in mprod
void Mat_Mul_4X4(MATRIX4X4* ma, MATRIX4X4* mb, MATRIX4X4* mprod)
{
    // note later we will take advantage of the fact that we know
    // that w=1 always, and that the last column of a 4x4 is
    // always 0

    for(int row = 0; row<4; row++)
    {
        for(int col = 0; col<4; col++)
        {
            // compute dot product from row of ma 
            // and column of mb

            float sum = 0; // used to hold result

            for(int index = 0; index<4; index++)
            {
                // add in next product pair
                sum += (ma->matrix[row][index] * mb->matrix[index][col]);
            } // end for index

            // insert resulting row,col element
            mprod->matrix[row][col] = sum;
        }
    }
}

// multiplies a 1x4 matrix against a
// 4x4 matrix - ma*mb and stores the result
// no tricks or assumptions here, just a straight multiply
void Mat_Mul_1X4_4X4(MATRIX1X4* ma, MATRIX4X4* mb, MATRIX1X4* mprod)
{
    for(int col = 0; col<4; col++)
    {
        // compute dot product from row of ma 
        // and column of mb
        float sum = 0; // used to hold result

        for(int row = 0; row<4; row++)
        {
            // add in next product pair
            sum += (ma->matrix[row] * mb->matrix[row][col]);
        }

        // insert resulting col element
        mprod->matrix[col] = sum;
    }
}

// multiplies a VECTOR3D against a
// 4x4 matrix - ma*mb and stores the result in mprod
// the function assumes that the vector refers to a 
// 4D homogenous vector, thus the function assumes that
// w=1 to carry out the multiply, also the function
// does not carry out the last column multiply since
// we are assuming w=1, there is no point
void Mat_Mul_VECTOR3D_4X4(VECTOR3D* va, MATRIX4X4* mb, VECTOR3D* vprod)
{
    vprod->x = (va->x * mb->matrix[0][0]) + (va->y * mb->matrix[1][0]) + (va->z * mb->matrix[2][0]) + mb->matrix[3][0];
    vprod->y = (va->x * mb->matrix[0][1]) + (va->y * mb->matrix[1][1]) + (va->z * mb->matrix[2][1]) + mb->matrix[3][1];
    vprod->z = (va->x * mb->matrix[0][2]) + (va->y * mb->matrix[1][2]) + (va->z * mb->matrix[2][2]) + mb->matrix[3][2];
}

// this function multiplies a VECTOR3D against a 
// 4x3 matrix - ma*mb and stores the result in mprod
// the function assumes that the vector refers to a 
// 4D homogenous vector, thus the function assumes that
// w=1 to carry out the multiply, also the function
// does not carry out the last column multiply since
// we are assuming w=1, there is no point
void Mat_Mul_VECTOR3D_4X3(VECTOR3D* va, MATRIX4X3* mb, VECTOR3D* vprod)
{
    vprod->x = (va->x * mb->matrix[0][0]) + (va->y * mb->matrix[1][0]) + (va->z * mb->matrix[2][0]) + mb->matrix[3][0];
    vprod->y = (va->x * mb->matrix[0][1]) + (va->y * mb->matrix[1][1]) + (va->z * mb->matrix[2][1]) + mb->matrix[3][1];
    vprod->z = (va->x * mb->matrix[0][2]) + (va->y * mb->matrix[1][2]) + (va->z * mb->matrix[2][2]) + mb->matrix[3][2];
}

// this function multiplies a VECTOR4D against a 
// 4x4 matrix - ma*mb and stores the result in mprod
// the function makes no assumptions
void Mat_Mul_VECTOR4D_4X4(VECTOR4D* va, MATRIX4X4* mb, VECTOR4D* vprod)
{
    vprod->x = (va->x * mb->matrix[0][0]) + (va->y * mb->matrix[1][0]) + (va->z * mb->matrix[2][0]) + (va->w * mb->matrix[3][0]);
    vprod->y = (va->x * mb->matrix[0][1]) + (va->y * mb->matrix[1][1]) + (va->z * mb->matrix[2][1]) + (va->w * mb->matrix[3][1]);
    vprod->z = (va->x * mb->matrix[0][2]) + (va->y * mb->matrix[1][2]) + (va->z * mb->matrix[2][2]) + (va->w * mb->matrix[3][2]);
    vprod->w = (va->x * mb->matrix[0][3]) + (va->y * mb->matrix[1][3]) + (va->z * mb->matrix[2][3]) + (va->w * mb->matrix[3][3]);
}

// multiplies a VECTOR4D against a
// 4x3 matrix - ma*mb and stores the result in mprod
// the function assumes that the last column of
// mb is [0 0 0 1]t , thus w just gets replicated
// from the vector [x y z w]
void Mat_Mul_VECTOR4D_4X3(VECTOR4D* va, MATRIX4X3* mb, VECTOR4D* vprod)
{
    vprod->x = (va->x * mb->matrix[0][0]) + (va->y * mb->matrix[1][0]) + (va->z * mb->matrix[2][0]) + (va->w * mb->matrix[3][0]);
    vprod->y = (va->x * mb->matrix[0][1]) + (va->y * mb->matrix[1][1]) + (va->z * mb->matrix[2][1]) + (va->w * mb->matrix[3][1]);
    vprod->z = (va->x * mb->matrix[0][2]) + (va->y * mb->matrix[1][2]) + (va->z * mb->matrix[2][2]) + (va->w * mb->matrix[3][2]);
    vprod->w = va->w;
}

// fills a 4x4 matrix with the sent data in row major form
void Mat_Init_4X4(MATRIX4X4* ma,
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33)

{
    ma->matrix[0][0] = m00; 
    ma->matrix[0][1] = m01; 
    ma->matrix[0][2] = m02; 
    ma->matrix[0][3] = m03;
    ma->matrix[1][0] = m10; 
    ma->matrix[1][1] = m11; 
    ma->matrix[1][2] = m12; 
    ma->matrix[1][3] = m13;
    ma->matrix[2][0] = m20; 
    ma->matrix[2][1] = m21; 
    ma->matrix[2][2] = m22; 
    ma->matrix[2][3] = m23;
    ma->matrix[3][0] = m30; 
    ma->matrix[3][1] = m31; 
    ma->matrix[3][2] = m32; 
    ma->matrix[3][3] = m33;
}

// computes the inverse of a 4x4, assumes that the last
// column is [0 0 0 1]t
int Mat_Inverse_4X4(MATRIX4X4* m, MATRIX4X4* mi)
{
    float det = (m->matrix[0][0] * (m->matrix[1][1] * m->matrix[2][2] - m->matrix[1][2] * m->matrix[2][1]) -
                 m->matrix[0][1] * (m->matrix[1][0] * m->matrix[2][2] - m->matrix[1][2] * m->matrix[2][0]) +
                 m->matrix[0][2] * (m->matrix[1][0] * m->matrix[2][1] - m->matrix[1][1] * m->matrix[2][0]));

    // test determinate to see if it's 0
    if(fabs(det) < EPSILON_E5)
        return(0);

    float det_inv = 1.0f / det;

    mi->matrix[0][0] = det_inv * (m->matrix[1][1] * m->matrix[2][2] - m->matrix[1][2] * m->matrix[2][1]);
    mi->matrix[0][1] = -det_inv * (m->matrix[0][1] * m->matrix[2][2] - m->matrix[0][2] * m->matrix[2][1]);
    mi->matrix[0][2] = det_inv * (m->matrix[0][1] * m->matrix[1][2] - m->matrix[0][2] * m->matrix[1][1]);
    mi->matrix[0][3] = 0.0f; // always 0

    mi->matrix[1][0] = -det_inv * (m->matrix[1][0] * m->matrix[2][2] - m->matrix[1][2] * m->matrix[2][0]);
    mi->matrix[1][1] = det_inv * (m->matrix[0][0] * m->matrix[2][2] - m->matrix[0][2] * m->matrix[2][0]);
    mi->matrix[1][2] = -det_inv * (m->matrix[0][0] * m->matrix[1][2] - m->matrix[0][2] * m->matrix[1][0]);
    mi->matrix[1][3] = 0.0f; // always 0

    mi->matrix[2][0] = det_inv * (m->matrix[1][0] * m->matrix[2][1] - m->matrix[1][1] * m->matrix[2][0]);
    mi->matrix[2][1] = -det_inv * (m->matrix[0][0] * m->matrix[2][1] - m->matrix[0][1] * m->matrix[2][0]);
    mi->matrix[2][2] = det_inv * (m->matrix[0][0] * m->matrix[1][1] - m->matrix[0][1] * m->matrix[1][0]);
    mi->matrix[2][3] = 0.0f; // always 0

    mi->matrix[3][0] = -(m->matrix[3][0] * mi->matrix[0][0] + m->matrix[3][1] * mi->matrix[1][0] + m->matrix[3][2] * mi->matrix[2][0]);
    mi->matrix[3][1] = -(m->matrix[3][0] * mi->matrix[0][1] + m->matrix[3][1] * mi->matrix[1][1] + m->matrix[3][2] * mi->matrix[2][1]);
    mi->matrix[3][2] = -(m->matrix[3][0] * mi->matrix[0][2] + m->matrix[3][1] * mi->matrix[1][2] + m->matrix[3][2] * mi->matrix[2][2]);
    mi->matrix[3][3] = 1.0f; // always 0
 
    return 1;
}

void Print_Mat_4X4(MATRIX4X4* ma, char *name = "M")
{
    // prints out a 4x4 matrix
    printf("\n%s=\n", name);
    for(int r = 0; r < 4; r++, printf("\n"))
        for(int c = 0; c < 4; c++)
            printf("%f ", ma->matrix[r][c]);

}

// initializes a parametric 2d line, note that the function
// computes v=p_pend - p_init, thus when t=0 the line p=p0+v*t = p0
// and whne t=1, p=p1, this way the segement is traced out from
// p0 to p1 via 0<= t <= 1
void Init_Parm_Line2D(POINT2D* p_init, POINT2D* p_term, PARMLINE2D* p)
{
    // start point
    VECTOR2D_INIT(&(p->p0), p_init);

    // end point
    VECTOR2D_INIT(&(p->p1), p_term);

    // now compute direction vector from p0->p1
    VECTOR2D_Build(p_init, p_term, &(p->v));
}

// initializes a parametric 3d line, note that the function
// computes v=p_pend - p_init, thus when t=0 the line p=p0+v*t = p0
// and whne t=1, p=p1, this way the segement is traced out from
// p0 to p1 via 0<= t <= 1
void Init_Parm_Line3D(POINT3D* p_init, POINT3D* p_term, PARMLINE3D* p)
{
    // start point
    VECTOR3D_INIT(&(p->p0), p_init);

    // end point
    VECTOR3D_INIT(&(p->p1), p_term);

    // now compute direction vector from p0->p1
    VECTOR3D_Build(p_init, p_term, &(p->v));
}

// computes the value of the sent parametric line at the value of t
void Compute_Parm_Line2D(PARMLINE2D* p, float t, POINT2D* pt)
{
    // this function 
    pt->x = p->p0.x + p->v.x * t;
    pt->y = p->p0.y + p->v.y * t;
}

// function computes the value of the sent parametric line at the value of t
void Compute_Parm_Line3D(PARMLINE3D* p, float t, POINT3D* pt)
{
    pt->x = p->p0.x + p->v.x * t;
    pt->y = p->p0.y + p->v.y * t;
    pt->z = p->p0.z + p->v.z * t;
}

// this function computes the interesection of the two parametric 
// line segments the function returns true if the segments 
// interesect and sets the values of t1 and t2 to the t values that 
// the intersection occurs on the lines p1 and p2 respectively, 
// however, the function may send back t value not in the range [0,1]
// this means that the segments don't intersect, but the lines that 
// they represent do, thus a retun of 0 means no intersection, a 
// 1 means intersection on the segments and a 2 means the lines 
// intersect, but not necessarily the segments and 3 means that 
// the lines are the same, thus they intersect everywhere
int Intersect_Parm_Lines2D(PARMLINE2D* p1, PARMLINE2D* p2, float *t1, float *t2)
{
    // basically we have a system of 2d linear equations, we need
    // to solve for t1, t2 when x,y are both equal (if that's possible)

    // step 1: test for parallel lines, if the direction vectors are 
    // scalar multiples then the lines are parallel and can't possible
    // intersect unless the lines overlap

    float det_p1p2 = (p1->v.x * p2->v.y - p1->v.y * p2->v.x);
    if(fabs(det_p1p2) <= EPSILON_E5)
    {
        // at this point, the lines either don't intersect at all
        // or they are the same lines, in which case they may intersect
        // at one or many points along the segments, at this point we 
        // will assume that the lines don't intersect at all, but later
        // we may need to rewrite this function and take into 
        // consideration the overlap and singular point exceptions
        return(PARM_LINE_NO_INTERSECT);

    } // end if

    // step 2: now compute the t1, t2 values for intersection
    // we have two lines of the form
    // p    = p0    +  v*t, specifically
    // p1   = p10   + v1*t1

    // p1.x = p10.x + v1.x*t1
    // p1.y = p10.y + v1.y*t1

    // p2 = p20 + v2*t2
    // p2   = p20   + v2*t2

    // p2.x = p20.x + v2.x*t2
    // p2.y = p20.y + v2.y*t2
    // solve the system when x1 = x2 and y1 = y2
    // explained in chapter 4
    *t1 = (p2->v.x * (p1->p0.y - p2->p0.y) - p2->v.y * (p1->p0.x - p2->p0.x))
        / det_p1p2;

    *t2 = (p1->v.x  * (p1->p0.y - p2->p0.y) - p1->v.y * (p1->p0.x - p2->p0.x))
        / det_p1p2;

    // test if the lines intersect on the segments
    if((*t1 >= 0) && (*t1 <= 1) && (*t2 >= 0) && (*t2 <= 1))
        return(PARM_LINE_INTERSECT_IN_SEGMENT);
    else
        return(PARM_LINE_INTERSECT_OUT_SEGMENT);
}

// this function computes the interesection of the two 
// parametric line segments the function returns true if 
// the segments interesect and sets the values of pt to the 
// intersection point, however, the function may send back a 
// value not in the range [0,1] on the segments this means 
// that the segments don't intersect, but the lines that 
// they represent do, thus a retun of 0 means no intersection, 
// a 1 means intersection on the segments and a 2 means 
// the lines intersect, but not necessarily the segments
int Intersect_Parm_Lines2D(PARMLINE2D* p1, PARMLINE2D* p2, POINT2D* pt)
{
    // basically we have a system of 2d linear equations, we need
    // to solve for t1, t2 when x,y are both equal (if that's possible)

    // step 1: test for parallel lines, if the direction vectors are 
    // scalar multiples then the lines are parallel and can't possible
    // intersect

    float t1, t2, det_p1p2 = (p1->v.x * p2->v.y - p1->v.y * p2->v.x);

    if(fabs(det_p1p2) <= EPSILON_E5)
    {
        // at this point, the lines either don't intersect at all
        // or they are the same lines, in which case they may intersect
        // at one or many points along the segments, at this point we 
        // will assume that the lines don't intersect at all, but later
        // we may need to rewrite this function and take into 
        // consideration the overlap and singular point exceptions
        return(PARM_LINE_NO_INTERSECT);

    } // end if

    // step 2: now compute the t1, t2 values for intersection
    // we have two lines of the form
    // p    = p0    +  v*t, specifically
    // p1   = p10   + v1*t1

    // p1.x = p10.x + v1.x*t1
    // p1.y = p10.y + v1.y*t1

    // p2 = p20 + v2*t2
    // p2   = p20   + v2*t2

    // p2.x = p20.x + v2.x*t2
    // p2.y = p20.y + v2.y*t2
    // solve the system when x1 = x2 and y1 = y2
    // explained in chapter 4
    t1 = (p2->v.x * (p1->p0.y - p2->p0.y) - p2->v.y * (p1->p0.x - p2->p0.x))
        / det_p1p2;

    t2 = (p1->v.x * (p1->p0.y - p2->p0.y) - p1->v.y * (p1->p0.x - p2->p0.x))
        / det_p1p2;

    // now compute point of intersection
    pt->x = p1->p0.x + p1->v.x * t1;
    pt->y = p1->p0.y + p1->v.y * t1;

    // test if the lines intersect on the segments
    if((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1))
        return(PARM_LINE_INTERSECT_IN_SEGMENT);
    else
        return(PARM_LINE_INTERSECT_OUT_SEGMENT);
}

// initializes a 3d plane
void PLANE3D_Init(PLANE3D* plane, POINT3D* p0, VECTOR3D* normal, int normalize = 0)
{
    // copy the point
    POINT3D_COPY(&plane->p0, p0);

    // if normalize is 1 then the normal is made into a unit vector
    if(!normalize)
        VECTOR3D_COPY(&plane->n, normal);
    else
    {
        // make normal into unit vector
        VECTOR3D_Normalize(normal, &plane->n);
    } // end else

}

// test if the point in on the plane, in the positive halfspace
// or negative halfspace
float Compute_Point_In_Plane3D(POINT3D* pt, PLANE3D* plane)
{
    float halfSpaceValue = plane->n.x * (pt->x - plane->p0.x) +
        plane->n.y * (pt->y - plane->p0.y) +
        plane->n.z * (pt->z - plane->p0.z);

    // return half space value
    return halfSpaceValue;
}

// determines where the sent parametric line
// intersects the plane the function will project the line 
// infinitely in both directions, to compute the intersection, 
// but the line segment defined by p intersected the plane iff t e [0,1]
// also the function returns 0 for no intersection, 1 for 
// intersection of the segment and the plane and 2 for intersection 
// of the line along the segment and the plane 3, the line lies 
// in the plane
int Intersect_Parm_Line3D_Plane3D(PARMLINE3D* pline, PLANE3D* plane, float *t, POINT3D* pt)
{
    // first test of the line and the plane are parallel, if so 
    // then they can't intersect unless the line lies in the plane!

    float plane_dot_line = VECTOR3D_Dot(&pline->v, &plane->n);

    if(fabs(plane_dot_line) <= EPSILON_E5)
    {
        // the line and plane are co-planer, does the line lie 
        // in the plane?
        if(fabs(Compute_Point_In_Plane3D(&pline->p0, plane)) <= EPSILON_E5)
            return(PARM_LINE_INTERSECT_EVERYWHERE);
        else
            return(PARM_LINE_NO_INTERSECT);
    } // end if

    // from chapter 4 we know that we can solve for the t where 
    // intersection occurs by
    // a*(x0+vx*t) + b*(y0+vy*t) + c*(z0+vz*t) + d =0
    // t = -(a*x0 + b*y0 + c*z0 + d)/(a*vx + b*vy + c*vz)
    // x0,y0,z0, vx,vy,vz, define the line
    // d = (-a*xp0 - b*yp0 - c*zp0), xp0, yp0, zp0, define the point on the plane 

    *t = -(plane->n.x * pline->p0.x +
        plane->n.y * pline->p0.y +
        plane->n.z * pline->p0.z -
        plane->n.x * plane->p0.x -
        plane->n.y * plane->p0.y -
        plane->n.z * plane->p0.z) / (plane_dot_line);

    // now plug t into the parametric line and solve for x,y,z
    pt->x = pline->p0.x + pline->v.x * (*t);
    pt->y = pline->p0.y + pline->v.y * (*t);
    pt->z = pline->p0.z + pline->v.z * (*t);

    // test interval of t
    if(*t >= 0.0 && *t <= 1.0)
        return(PARM_LINE_INTERSECT_IN_SEGMENT);
    else
        return(PARM_LINE_INTERSECT_OUT_SEGMENT);

}