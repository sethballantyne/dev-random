#pragma once
#include <cstring>

#define M_PI       3.14159265//358979323846

#define EPSILON_E3 (float)(1E-3)
#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

// parametric line intersections
#define PARM_LINE_NO_INTERSECT          0
#define PARM_LINE_INTERSECT_IN_SEGMENT  1
#define PARM_LINE_INTERSECT_OUT_SEGMENT 2
#define PARM_LINE_INTERSECT_EVERYWHERE  3

// defines for fixed point math
#define FIXP16_SHIFT     16
#define FIXP16_MAG       65536
#define FIXP16_DP_MASK   0x0000ffff
#define FIXP16_WP_MASK   0xffff0000
#define FIXP16_ROUND_UP  0x00008000

#define SWAP(a,b,t) {t=a; a=b; b=t;}
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (a) : (b)) 

// 2x2 matrix
struct MATRIX2X2
{
    float matrix[2][2];
};

struct MATRIX1X4
{
    float matrix[4];
};

struct MATRIX1X2
{
    float matrix[2];
};

struct MATRIX1X3
{
    float matrix[3];
};

struct MATRIX4X4
{
    float matrix[4][4];
};

struct MATRIX4X3
{
    float matrix[4][3];
};

struct MATRIX3X3
{
    float matrix[3][3];
};

struct VECTOR2D
{
    float x;
    float y;
};

struct VECTOR3D
{
    float x;
    float y;
    float z;
};

// homogenous vector; a 3D vector with a w component.
struct VECTOR4D
{
    float x;
    float y;
    float z;
    float w;
};

using POINT2D = VECTOR2D;
using POINT3D = VECTOR3D;
using POINT4D = VECTOR4D;

// 2D Parametric line
struct PARMLINE2D
{
    // start point of the parametric line
    POINT2D p0;

    // end point of the parametric line
    POINT2D p1;

    // direction vector of the line segment
    VECTOR2D v;

};

// 3D parametric line
struct PARMLINE3D
{
    // start point of the parametric line
    POINT3D p0;

    // end point of the parametric line
    POINT3D p1;

    // direction of the line segment 
    VECTOR3D v;
};

// 3D plane
struct PLANE3D
{
    // point of the plane
    POINT3D p0;

    // normal to the plane
    VECTOR3D n;
};

// 2D polar coordinates
struct POLAR2D
{
    // the radi of the point
    float r; 

    // the angle in radians
    float theta;
};

// 3D cylindrical coordinates
struct CYLINDRICAL3D
{
    // the radi of the point
    float r;

    // the angle in degrees about the z axis
    float theta;

    // the z-height of the point
    float z;
};

// 3D spherical coordinates
struct SPHERICAL3D
{
    // rho, the distance to the point from the origin
    float p;

    //  the angle from the z-axis and the line segment o->p
    float theta;

    // the angle from the projection if o->p onto the x-y axis
    float phi; 
};

// 4D Quaternion
struct QUAT
{
    // the real component
    float q0; 

    // the imaginary component
    VECTOR3D qv;
};

using FIXP16 = int;

// 4X4 identity matrix
const MATRIX4X4 IMAT_4X4 = { 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             0, 0, 0, 1 };

// 4x3 indentity matrix
// not correct mathematically.
// but later we may use 4x3 matrices with the assumption that 
// the last column is always [0 0 0 1]t
const MATRIX4X3 IMAT_4X3 = { 1, 0, 0,
                             0, 1, 0,
                             0, 0, 1,
                             0, 0, 0 };

// 3x3 identity matrix
const MATRIX3X3 IMAT_3X3 = { 1, 0, 0,
                             0, 1, 0,
                             0, 0, 1 };

// 2x2 identity matrix
const MATRIX2X2 IMAT_2X2 = { 1, 0,
0, 1 };

// matrix copying macros
#define MAT_COPY_2X2(src_mat, dest_mat) { memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX2X2) ); }
#define MAT_COPY_3X3(src_mat, dest_mat) { memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX3X3) ); }
#define MAT_COPY_4X4(src_mat, dest_mat) { memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X4) ); }
#define MAT_COPY_4X3(src_mat, dest_mat) { memcpy((void *)(dest_mat), (void *)(src_mat), sizeof(MATRIX4X3) ); }

// macros that zero out matrices
#define MAT_ZERO_2X2(m) { memset((void *)(m), 0, sizeof(MATRIX2X2)); }
#define MAT_ZERO_3X3(m) { memset((void *)(m), 0, sizeof(MATRIX3X3)); }
#define MAT_ZERO_4X4(m) { memset((void *)(m), 0, sizeof(MATRIX4X4)); }
#define MAT_ZERO_4X3(m) { memset((void *)(m), 0, sizeof(MATRIX4X3)); }

// macros to set the identity matrix
#define MAT_IDENTITY_2X2(m) { memcpy((void *)(m), (void *)&IMAT_2X2, sizeof(MATRIX2X2)); }
#define MAT_IDENTITY_3X3(m) { memcpy((void *)(m), (void *)&IMAT_3X3, sizeof(MATRIX3X3)); }
#define MAT_IDENTITY_4X4(m) { memcpy((void *)(m), (void *)&IMAT_4X4, sizeof(MATRIX4X4)); }
#define MAT_IDENTITY_4X3(m) { memcpy((void *)(m), (void *)&IMAT_4X3, sizeof(MATRIX4X3)); }

int Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);

// matrix transposing macros
inline void MAT_TRANSPOSE_3X3(MATRIX3X3* m)
{
    MATRIX3X3 mt;

    mt.matrix[0][0] = m->matrix[0][0];
    mt.matrix[0][1] = m->matrix[1][0];
    mt.matrix[0][2] = m->matrix[2][0];
    mt.matrix[1][0] = m->matrix[0][1];
    mt.matrix[1][1] = m->matrix[1][1];
    mt.matrix[1][2] = m->matrix[2][1];
    mt.matrix[2][0] = m->matrix[0][2]; 
    mt.matrix[2][1] = m->matrix[1][2]; 
    mt.matrix[2][2] = m->matrix[2][2];

    memcpy((void *)m, (void *)&mt, sizeof(MATRIX3X3));
}

inline void MAT_TRANSPOSE_4X4(MATRIX4X4* m)
{
    MATRIX4X4 mt;

    mt.matrix[0][0] = m->matrix[0][0]; 
    mt.matrix[0][1] = m->matrix[1][0]; 
    mt.matrix[0][2] = m->matrix[2][0]; 
    mt.matrix[0][3] = m->matrix[3][0];
    mt.matrix[1][0] = m->matrix[0][1]; 
    mt.matrix[1][1] = m->matrix[1][1]; 
    mt.matrix[1][2] = m->matrix[2][1]; 
    mt.matrix[1][3] = m->matrix[3][1];
    mt.matrix[2][0] = m->matrix[0][2]; 
    mt.matrix[2][1] = m->matrix[1][2]; 
    mt.matrix[2][2] = m->matrix[2][2]; 
    mt.matrix[2][3] = m->matrix[3][2];
    mt.matrix[3][0] = m->matrix[0][3]; 
    mt.matrix[3][1] = m->matrix[1][3]; 
    mt.matrix[3][2] = m->matrix[2][2]; 
    mt.matrix[3][3] = m->matrix[3][3];

    memcpy((void *)m, (void *)&mt, sizeof(MATRIX4X4));
}

inline void MAT_TRANSPOSE_3X3(MATRIX3X3* m, MATRIX3X3* mt)
{
    mt->matrix[0][0] = m->matrix[0][0]; 
    mt->matrix[0][1] = m->matrix[1][0]; 
    mt->matrix[0][2] = m->matrix[2][0];
    mt->matrix[1][0] = m->matrix[0][1]; 
    mt->matrix[1][1] = m->matrix[1][1]; 
    mt->matrix[1][2] = m->matrix[2][1];
    mt->matrix[2][0] = m->matrix[0][2]; 
    mt->matrix[2][1] = m->matrix[1][2]; 
    mt->matrix[2][2] = m->matrix[2][2];
}

inline void MAT_TRANSPOSE_4X4(MATRIX4X4* m, MATRIX4X4* mt)
{
    mt->matrix[0][0] = m->matrix[0][0]; 
    mt->matrix[0][1] = m->matrix[1][0]; 
    mt->matrix[0][2] = m->matrix[2][0]; 
    mt->matrix[0][3] = m->matrix[3][0];
    mt->matrix[1][0] = m->matrix[0][1]; 
    mt->matrix[1][1] = m->matrix[1][1]; 
    mt->matrix[1][2] = m->matrix[2][1]; 
    mt->matrix[1][3] = m->matrix[3][1];
    mt->matrix[2][0] = m->matrix[0][2]; 
    mt->matrix[2][1] = m->matrix[1][2]; 
    mt->matrix[2][2] = m->matrix[2][2]; 
    mt->matrix[2][3] = m->matrix[3][2];
    mt->matrix[3][0] = m->matrix[0][3]; 
    mt->matrix[3][1] = m->matrix[1][3]; 
    mt->matrix[3][2] = m->matrix[2][2]; 
    mt->matrix[3][3] = m->matrix[3][3];
}

// matrix and vector column swaping macros
inline void MAT_COLUMN_SWAP_2X2(MATRIX2X2* matrix, int column, MATRIX1X2* v)
{
    matrix->matrix[0][column] = v->matrix[0]; 
    matrix->matrix[1][column] = v->matrix[1];
}

inline void MAT_COLUMN_SWAP_3X3(MATRIX3X3* m, int c, MATRIX1X3* v)
{
    m->matrix[0][c] = v->matrix[0]; 
    m->matrix[1][c] = v->matrix[1]; 
    m->matrix[2][c] = v->matrix[2];
}

inline void MAT_COLUMN_SWAP_4X4(MATRIX4X4* m, int c, MATRIX1X4* v)
{
    m->matrix[0][c] = v->matrix[0]; 
    m->matrix[1][c] = v->matrix[1]; 
    m->matrix[2][c] = v->matrix[2]; 
    m->matrix[3][c] = v->matrix[3];
}

inline void MAT_COLUMN_SWAP_4X3(MATRIX4X3* m, int c, MATRIX1X4* v)
{
    m->matrix[0][c] = v->matrix[0]; 
    m->matrix[1][c] = v->matrix[1]; 
    m->matrix[2][c] = v->matrix[2]; 
    m->matrix[3][c] = v->matrix[3];
}

// vector macros, note the 4D vector sets w=1
// vector zeroing macros
inline void VECTOR2D_ZERO(VECTOR2D* v)
{
    (v)->x = (v)->y = 0.0;
}

inline void VECTOR3D_ZERO(VECTOR3D* v)
{
    (v)->x = (v)->y = (v)->z = 0.0;
}

inline void VECTOR4D_ZERO(VECTOR4D* v)
{
    (v)->x = (v)->y = (v)->z = 0.0; 
    (v)->w = 1.0;
}


// macros to initialize vectors with explicit components
inline void VECTOR2D_INITXY(VECTOR2D* v, float x, float y)
{
    (v)->x = (x); (v)->y = (y);
}

inline void VECTOR3D_INITXYZ(VECTOR3D* v, float x, float y, float z)
{
    (v)->x = (x);
    (v)->y = (y); 
    (v)->z = (z);
}

inline void VECTOR4D_INITXYZ(VECTOR4D* v, float x, float y, float z)
{
    (v)->x = (x); 
    (v)->y = (y); 
    (v)->z = (z); 
    (v)->w = 1.0;
}

// used to convert from 4D homogenous to 4D non-homogenous
inline void VECTOR4D_DIV_BY_W(VECTOR4D* v)
{
    (v)->x /= (v)->w; 
    (v)->y /= (v)->w; 
    (v)->z /= (v)->w;
}

inline void VECTOR4D_DIV_BY_W_VECTOR3D(VECTOR4D* v4, VECTOR3D* v3)
{
    (v3)->x = (v4)->x / (v4)->w; 
    (v3)->y = (v4)->y / (v4)->w; 
    (v3)->z = (v4)->z / (v4)->w;
}

// vector intialization macros to initialize with other vectors
inline void VECTOR2D_INIT(VECTOR2D* vdst, VECTOR2D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
}

inline void VECTOR3D_INIT(VECTOR3D* vdst, VECTOR3D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;  
    (vdst)->z = (vsrc)->z;
}

inline void VECTOR4D_INIT(VECTOR4D* vdst, VECTOR4D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
    (vdst)->z = (vsrc)->z; 
    (vdst)->w = (vsrc)->w;
}

// vector copying macros
inline void VECTOR2D_COPY(VECTOR2D* vdst, VECTOR2D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
}


inline void VECTOR3D_COPY(VECTOR3D* vdst, VECTOR3D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;  
    (vdst)->z = (vsrc)->z;
}

inline void VECTOR4D_COPY(VECTOR4D* vdst, VECTOR4D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
    (vdst)->z = (vsrc)->z; 
    (vdst)->w = (vsrc)->w;
}

// point initialization macros
inline void POINT2D_INIT(POINT2D* vdst, POINT2D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
}

inline void POINT3D_INIT(POINT3D* vdst, POINT3D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;  
    (vdst)->z = (vsrc)->z;
}

inline void POINT4D_INIT(POINT4D* vdst, POINT4D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
    (vdst)->z = (vsrc)->z; 
    (vdst)->w = (vsrc)->w;
}

// point copying macros
inline void POINT2D_COPY(POINT2D* vdst, POINT2D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
}

inline void POINT3D_COPY(POINT3D* vdst, POINT3D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;  
    (vdst)->z = (vsrc)->z;
}

inline void POINT4D_COPY(POINT4D* vdst, POINT4D* vsrc)
{
    (vdst)->x = (vsrc)->x; 
    (vdst)->y = (vsrc)->y;
    (vdst)->z = (vsrc)->z; 
    (vdst)->w = (vsrc)->w;
}

// quaternion macros
inline void QUAT_ZERO(QUAT* q)
{
    q->qv.x = 0.0f;
    q->qv.y = 0.0f;
    q->qv.z = 0.0f;
    q->q0 = 0.0f;
}

inline void QUAT_INITWXYZ(QUAT* q, float w, float x, float y, float z)
{
    q->q0 = w;
    q->qv.x = x;
    q->qv.y = y;
    q->qv.z = z;
}

inline void QUAT_INIT_VECTOR3D(QUAT* q, VECTOR3D* v)
{
    q->q0 = 0; 
    q->qv.x = v->x; 
    q->qv.y = v->y; 
    q->qv.z = v->z;
}

inline void QUAT_INIT(QUAT* qdst, QUAT* qsrc)
{
    qdst->q0 = qsrc->q0;  
    qdst->qv.x = qsrc->qv.x;
    qdst->qv.y = qsrc->qv.y;  
    qdst->qv.z = qsrc->qv.z;
}

inline void QUAT_COPY(QUAT* qdst, QUAT* qsrc)
{
    qdst->qv.x = qsrc->qv.x; 
    qdst->qv.y = qsrc->qv.y;
    qdst->qv.z = qsrc->qv.z; 
    qdst->q0 = qsrc->q0;
}

// convert integer and float to fixed point 16.16
#define INT_TO_FIXP16(i) ((i) <<  FIXP16_SHIFT)
#define FLOAT_TO_FIXP16(f) (((float)(f) * (float)FIXP16_MAG+0.5))

// convert fixed point to float
#define FIXP16_TO_FLOAT(fp) ( ((float)fp)/FIXP16_MAG)

// extract the whole part and decimal part from a fixed point 16.16
#define FIXP16_WP(fp) ((fp) >> FIXP16_SHIFT)
#define FIXP16_DP(fp) ((fp) && FIXP16_DP_MASK)

void Math_BuildLookupTables();

// trig functions
float Fast_Sin(float theta);
float Fast_Cos(float theta);

// polar, cylindrical, spherical functions
void POLAR2D_To_POINT2D(POLAR2D* polar, POINT2D* rect);
void POLAR2D_To_RectXY(POLAR2D* polar, float *x, float *y);
void POINT2D_To_POLAR2D(POINT2D* rect, POLAR2D* polar);
void POINT2D_To_PolarRTh(POINT2D* rect, float *r, float *theta);
void CYLINDRICAL3D_To_POINT3D(CYLINDRICAL3D* cyl, POINT3D* rect);
void CYLINDRICAL3D_To_RectXYZ(CYLINDRICAL3D* cyl, float *x, float *y, float *z);
void POINT3D_To_CYLINDRICAL3D(POINT3D* rect, CYLINDRICAL3D* cyl);
void POINT3D_To_CylindricalRThZ(POINT3D* rect, float *r, float *theta, float *z);
void SPHERICAL3D_To_POINT3D(SPHERICAL3D* sph, POINT3D* rect);
void SPHERICAL3D_To_RectXYZ(SPHERICAL3D* sph, float *x, float *y, float *z);
void POINT3D_To_SPHERICAL3D(POINT3D* rect, SPHERICAL3D* sph);
void POINT3D_To_SphericalPThPh(POINT3D* rect, float *p, float *theta, float *phi);

// 2d vector functions
void VECTOR2D_Add(VECTOR2D* va, VECTOR2D* vb, VECTOR2D* vsum);
VECTOR2D VECTOR2D_Add(VECTOR2D* va, VECTOR2D* vb);
void VECTOR2D_Sub(VECTOR2D* va, VECTOR2D* vb, VECTOR2D* vdiff);
VECTOR2D VECTOR2D_Sub(VECTOR2D* va, VECTOR2D* vb);
void VECTOR2D_Scale(float k, VECTOR2D* va);
void VECTOR2D_Scale(float k, VECTOR2D* va, VECTOR2D* vscaled);
float VECTOR2D_Dot(VECTOR2D* va, VECTOR2D* vb);
float VECTOR2D_Length(VECTOR2D* va);
float VECTOR2D_Length_Fast(VECTOR2D* va);
void VECTOR2D_Normalize(VECTOR2D* va);
void VECTOR2D_Normalize(VECTOR2D* va, VECTOR2D* vn);
void VECTOR2D_Build(VECTOR2D* init, VECTOR2D* term, VECTOR2D* result);
float VECTOR2D_CosTh(VECTOR2D* va, VECTOR2D* vb);
void VECTOR2D_Print(VECTOR2D* va, char *name);

// 3d vector functions
void VECTOR3D_Add(VECTOR3D* va, VECTOR3D* vb, VECTOR3D* vsum);
VECTOR3D VECTOR3D_Add(VECTOR3D* va, VECTOR3D* vb);
void VECTOR3D_Sub(VECTOR3D* va, VECTOR3D* vb, VECTOR3D* vdiff);
VECTOR3D VECTOR3D_Sub(VECTOR3D* va, VECTOR3D* vb);
void VECTOR3D_Scale(float k, VECTOR3D* va);
void VECTOR3D_Scale(float k, VECTOR3D* va, VECTOR3D* vscaled);
float VECTOR3D_Dot(VECTOR3D* va, VECTOR3D* vb);
void VECTOR3D_Cross(VECTOR3D* va, VECTOR3D* vb, VECTOR3D* vn);
VECTOR3D VECTOR3D_Cross(VECTOR3D* va, VECTOR3D* vb);
float VECTOR3D_Length(VECTOR3D* va);
float VECTOR3D_Length_Fast(VECTOR3D* va);
void VECTOR3D_Normalize(VECTOR3D* va);
void VECTOR3D_Normalize(VECTOR3D* va, VECTOR3D* vn);
void VECTOR3D_Build(VECTOR3D* init, VECTOR3D* term, VECTOR3D* result);
float VECTOR3D_CosTh(VECTOR3D* va, VECTOR3D* vb);
void VECTOR3D_Print(VECTOR3D* va, char *name);

// 4d vector functions
void VECTOR4D_Add(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vsum);
VECTOR4D VECTOR4D_Add(VECTOR4D* va, VECTOR4D* vb);
void VECTOR4D_Sub(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vdiff);
VECTOR4D VECTOR4D_Sub(VECTOR4D* va, VECTOR4D* vb);
void VECTOR4D_Scale(float k, VECTOR4D* va);
void VECTOR4D_Scale(float k, VECTOR4D* va, VECTOR4D* vscaled);
float VECTOR4D_Dot(VECTOR4D* va, VECTOR4D* vb);
void VECTOR4D_Cross(VECTOR4D* va, VECTOR4D* vb, VECTOR4D* vn);
VECTOR4D VECTOR4D_Cross(VECTOR4D* va, VECTOR4D* vb);
float VECTOR4D_Length(VECTOR4D* va);
float VECTOR4D_Length_Fast(VECTOR4D* va);
void VECTOR4D_Normalize(VECTOR4D* va);
void VECTOR4D_Normalize(VECTOR4D* va, VECTOR4D* vn);
void VECTOR4D_Build(VECTOR4D* init, VECTOR4D* term, VECTOR4D* result);
float VECTOR4D_CosTh(VECTOR4D* va, VECTOR4D* vb);
void VECTOR4D_Print(VECTOR4D* va, char *name);


// 2x2 matrix functions
void Mat_Init_2X2(MATRIX2X2* ma, float m00, float m01, float m10, float m11);
void Print_Mat_2X2(MATRIX2X2* ma, char *name);
float Mat_Det_2X2(MATRIX2X2* m);
void Mat_Add_2X2(MATRIX2X2* ma, MATRIX2X2* mb, MATRIX2X2* msum);
void Mat_Mul_2X2(MATRIX2X2* ma, MATRIX2X2* mb, MATRIX2X2* mprod);
int Mat_Inverse_2X2(MATRIX2X2* m, MATRIX2X2* mi);
int Solve_2X2_System(MATRIX2X2* A, MATRIX1X2* X, MATRIX1X2* B);

// 3x3 matrix functions (note there others in T3DLIB1.CPP|H)
void Mat_Add_3X3(MATRIX3X3* ma, MATRIX3X3* mb, MATRIX3X3* msum);
void Mat_Mul_VECTOR3D_3X3(VECTOR3D* va, MATRIX3X3* mb, VECTOR3D* vprod);
int Mat_Inverse_3X3(MATRIX3X3* m, MATRIX3X3* mi);
void Mat_Init_3X3(MATRIX3X3* ma,
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22);
void Print_Mat_3X3(MATRIX3X3* ma, char *name);
float Mat_Det_3X3(MATRIX3X3* m);
int Solve_3X3_System(MATRIX3X3* A, MATRIX1X3* X, MATRIX1X3* B);

// 4x4 matrix functions
void Mat_Add_4X4(MATRIX4X4* ma, MATRIX4X4* mb, MATRIX4X4* msum);
void Mat_Mul_4X4(MATRIX4X4* ma, MATRIX4X4* mb, MATRIX4X4* mprod);
void Mat_Mul_1X4_4X4(MATRIX1X4* ma, MATRIX4X4* mb, MATRIX1X4* mprod);
void Mat_Mul_VECTOR3D_4X4(VECTOR3D* va, MATRIX4X4* mb, VECTOR3D* vprod);
void Mat_Mul_VECTOR3D_4X3(VECTOR3D* va, MATRIX4X3* mb, VECTOR3D* vprod);
void Mat_Mul_VECTOR4D_4X4(VECTOR4D* va, MATRIX4X4* mb, VECTOR4D* vprod);
void Mat_Mul_VECTOR4D_4X3(VECTOR4D* va, MATRIX4X4* mb, VECTOR4D* vprod);
int Mat_Inverse_4X4(MATRIX4X4* m, MATRIX4X4* mi);
void Mat_Init_4X4(MATRIX4X4* ma,
    float m00, float m01, float m02, float m03,
    float m10, float m11, float m12, float m13,
    float m20, float m21, float m22, float m23,
    float m30, float m31, float m32, float m33);
void Print_Mat_4X4(MATRIX4X4* ma, char *name);

// helper function takes a set if euler angles and computes
// a rotation matrix from them.
void Build_XYZ_Rotation_MATRIX4X4(float theta_x, float theta_y, float theta_z, MATRIX4X4* mrot);

// quaternion functions
void QUAT_Add(QUAT* q1, QUAT* q2, QUAT* qsum);
void QUAT_Sub(QUAT* q1, QUAT* q2, QUAT* qdiff);
void QUAT_Conjugate(QUAT* q, QUAT* qconj);
void QUAT_Scale(QUAT* q, float scale, QUAT* qs);
void QUAT_Scale(QUAT* q, float scale);
float QUAT_Norm(QUAT* q);
float QUAT_Norm2(QUAT* q);
void QUAT_Normalize(QUAT* q, QUAT* qn);
void QUAT_Normalize(QUAT* q);
void QUAT_Unit_Inverse(QUAT* q, QUAT* qi);
void QUAT_Unit_Inverse(QUAT* q);
void QUAT_Inverse(QUAT* q, QUAT* qi);
void QUAT_Inverse(QUAT* q);
void QUAT_Mul(QUAT* q1, QUAT* q2, QUAT* qprod);
void QUAT_Triple_Product(QUAT* q1, QUAT* q2, QUAT* q3, QUAT* qprod);
void VECTOR3D_Theta_To_QUAT(QUAT* q, VECTOR3D* v, float theta);
void VECTOR4D_Theta_To_QUAT(QUAT* q, VECTOR4D* v, float theta);
void EulerZYX_To_QUAT(QUAT* q, float theta_z, float theta_y, float theta_x);
void QUAT_To_VECTOR3D_Theta(QUAT* q, VECTOR3D* v, float *theta);
void QUAT_Print(QUAT* q, char *name);

// 2d parametric line functions
void Init_Parm_Line2D(POINT2D* p_init, POINT2D* p_term, PARMLINE2D* p);
void Compute_Parm_Line2D(PARMLINE2D* p, float t, POINT2D* pt);
int Intersect_Parm_Lines2D(PARMLINE2D* p1, PARMLINE2D* p2, float *t1, float *t2);
int Intersect_Parm_Lines2D(PARMLINE2D* p1, PARMLINE2D* p2, POINT2D* pt);

// 3d parametric line functions
void Init_Parm_Line3D(POINT3D* p_init, POINT3D* p_term, PARMLINE3D* p);
void Compute_Parm_Line3D(PARMLINE3D* p, float t, POINT3D* pt);

// 3d plane functions
void PLANE3D_Init(PLANE3D* plane, POINT3D* p0, VECTOR3D* normal, int normalize);
float Compute_Point_In_Plane3D(POINT3D* pt, PLANE3D* plane);
int Intersect_Parm_Line3D_Plane3D(PARMLINE3D* pline, PLANE3D* plane,
    float *t, POINT3D* pt);

// fixed point functions
FIXP16 FIXP16_MUL(FIXP16 fp1, FIXP16 fp2);
FIXP16 FIXP16_DIV(FIXP16 fp1, FIXP16 fp2);
void FIXP16_Print(FIXP16 fp);