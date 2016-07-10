
#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "linalg.h"
#include "373assignment.h"

/* Constants */
#define RAY_EPS   (WALL_SIZE / 3.0f)

/* Enums */
typedef enum {HORIZONTAL, VERTICAL} RayType;

/* Datatypes */
typedef struct {
    Vector3f vec;
    RayType intersectionType;
} Ray;

/* Global data */
extern Vector3f viewplaneDir;
extern float distFromViewplane;
extern Matrix3f counterClockwiseRotation;
extern Matrix3f clockwiseRotation;
extern Ray rays[VIEWPLANE_LENGTH];

/* Functions */
static void initializeRayDirections();
static Vector3f findInitialVerticalRayStepVector(Vector3f rayVec, Vector3f rayStart);
static Vector3f findInitialHorizontalRayStepVector(Vector3f rayVec, Vector3f rayStart);
static Vector3f findVerticalRayStepVector(Vector3f rayVec);
static Vector3f findHorizontalRayStepVector(Vector3f rayVec);
static Ray raycast(Ray ray, Vector3f rayStart);
Vector3f getTileCoordinateForVerticalRay(Vector3f ray, Vector3f rayStart);
Vector3f getTileCoordinateForHorizontalRay(Vector3f ray, Vector3f rayStart);
void updateRaycaster();
void setupRaycaster();



#endif /* RAYCASTER_H */
