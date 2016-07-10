

#ifndef LINALG_H
#define LINALG_H

#include <math.h>

/* Constants */
#define EPS   0.00000001

/* Floating point operations */
#define MAKE_NONZERO(A)  ((fabs((A)) < EPS) ? EPS : A) /* Make any value less than epsilon equal to epsilon */

/* Type templates */
#define IDENTITY_M     {{1,0,0},{0,1,0},{0,0,1}}
#define HOMOGENEOUS_V3 {0, 0, 1}

/* Types (provided for students) */
typedef struct {
    float x;
    float y;
    float z;
} Vector3f;

typedef float Matrix3f[3][3];

/* Functions to be implemented by students */
Vector3f homogeneousVectorAdd(Vector3f vec1, Vector3f vec2);
Vector3f homogeneousVectorSubtract(Vector3f vec1, Vector3f vec2);
Vector3f homogeneousVectorScale(Vector3f vec, float scalar);
float homogeneousVectorMagnitude(Vector3f vec);
Vector3f normalizeHomogeneousVector(Vector3f vec);
float homogeneousVectorDotProduct(Vector3f vec1, Vector3f vec2);
Vector3f homogeneousVectorProjection(Vector3f vec1, Vector3f vec2);
Vector3f homogeneousReciprocalVectorProjection(Vector3f vec1, Vector3f vec2);

void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec);


/* Functions provided for students */
void    matrix3fCopy(Matrix3f* dst, Matrix3f* src);


#endif /* LINALG_H */
