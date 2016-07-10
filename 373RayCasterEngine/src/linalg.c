
#include "linalg.h"

/**
 * Add two homogeneous vectors.
 *
 * vec1: The first vector
 * vec2: The second vector
 *
 * Returns: A homogeneous vector for the addition of the two input vectors.
 */
Vector3f homogeneousVectorAdd(Vector3f vec1, Vector3f vec2) {
    Vector3f retVec = {vec1.x + vec2.x, vec1.y + vec2.y, 1};
    return retVec;
}

/**
 * Subtract two homogeneous vectors.
 *
 * vec1: The first vector
 * vec2: The second vector
 *
 * Returns: A homogeneous vector for the subtraction of the two input vectors.
 */
Vector3f homogeneousVectorSubtract(Vector3f vec1, Vector3f vec2) {
	Vector3f retVec = { vec1.x - vec2.x, vec1.y - vec2.y, 1 };
    return retVec;
}

/**
 * Scale a homogeneous vector by a scalar.
 *
 * vec: The vector to scale.
 * scalar: The scalar to use.
 *
 * Returns: A homogeneous vector for the scaling of the input vector.
 */
Vector3f homogeneousVectorScale(Vector3f vec, float scalar) {
	Vector3f retVec = { (vec.x * scalar), (vec.y * scalar), 1 };
    return retVec;
}

/**
 * Get the magnitude of a given homogeneous vector.
 *
 * vec: The vector to get the magnitude for.
 *
 * Returns: The magnitude of the input vector.
 */
float homogeneousVectorMagnitude(Vector3f vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

/**
 * Normaize a homogeneous vector.
 *
 * vec: The vector to be normalized.
 *
 * Returns: A homogeneous vector that is the result of normalizing the input vector.
 */
Vector3f normalizeHomogeneousVector(Vector3f vec) {
	float magnitude = homogeneousVectorMagnitude(vec);
	Vector3f retVec = { vec.x / magnitude, vec.y / magnitude, 1 };
    return retVec;
}

/**
 * Get the dot product of two homogeneous vectors.
 *
 * vec1: The first vector
 * vec2: The second vector
 *
 * Returns: The dot product of the two homogeneous vectors.
 */
float homogeneousVectorDotProduct(Vector3f vec1, Vector3f vec2) {
	return (vec1.x * vec2.x + vec1.y * vec2.y);
}

/**
 * Perform vector projection of one homogeneous vector onto another.
 *
 * vec1: The vector to be projected.
 * vec2: The vector to be projected onto.
 *
 * Returns: A homogeneous vector for the projection of vec1 onto vec2.
 */
Vector3f homogeneousVectorProjection(Vector3f vec1, Vector3f vec2) {
	float dotProduct = homogeneousVectorDotProduct(vec1, vec2);
	float vec2squared = homogeneousVectorDotProduct(vec2, vec2);
	float toMultiply = dotProduct / vec2squared;
	Vector3f retVec = { vec2.x * toMultiply, vec2.y * toMultiply, 1 };
    return retVec;
}

/**
 * Perform reciprocal vector projection of one homogeneous vector onto another.
 *
 * vec1: The vector to be projected.
 * vec2: The vector to be projected onto.
 *
 * Returns: A homogeneous vector for the reciprocal projection of vec1 onto vec2.
 */
Vector3f homogeneousReciprocalVectorProjection(Vector3f vec1, Vector3f vec2) {
	float dotProduct = homogeneousVectorDotProduct(vec1, vec2);
	float vec2squared = homogeneousVectorDotProduct(vec1, vec1);
	float toMultiply = vec2squared / dotProduct;
	Vector3f retVec = { vec2.x * toMultiply, vec2.y * toMultiply, 1 };
    return retVec;
}

/**
 * Multiply a vector by a matrix.
 *
 * mat: The matrix to multiply by.
 * vec: The vector to be multiplied.
 */
void matrixVectorMultiply(Matrix3f* mat, Vector3f* vec) {
    Vector3f newVec = {0, 0, 1};
	
	newVec.x = ((*mat)[0][0] * (float)(*vec).x) + ((*mat)[0][1] * (float)(*vec).y) + ((*mat)[0][2] * (float)(*vec).z);
	newVec.y = ((*mat)[1][0] * (float)(*vec).x) + ((*mat)[1][1] * (float)(*vec).y) + ((*mat)[1][2] * (float)(*vec).z);
	newVec.z = ((*mat)[2][0] * (float)(*vec).x) + ((*mat)[2][1] * (float)(*vec).y) + ((*mat)[2][2] * (float)(*vec).z);

    *vec = newVec;
}


/**
 * Copy the values from a 3x3 matrix 'src' into another 3x3 matrix 'dst'.
 */
void matrix3fCopy(Matrix3f* dst, Matrix3f* src) {
    int row, col;

    for(row = 0; row < 3; row++) {
        for(col = 0; col < 3; col++) {
            (*dst)[row][col] = (*src)[row][col];
        }
    }
}
