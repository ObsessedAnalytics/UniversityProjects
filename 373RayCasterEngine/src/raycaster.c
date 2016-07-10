#include <stdio.h>

#include "373assignment.h"
#include "raycaster.h"
#include "player.h"

/* Globals */
Vector3f viewplaneDir = VIEWPLANE_DIR;
float distFromViewplane = 1.0f;
Matrix3f counterClockwiseRotation = IDENTITY_M;
Matrix3f clockwiseRotation = IDENTITY_M;
Ray rays[VIEWPLANE_LENGTH];

/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE ABOVE THIS LINE                    */
/* ============================================================================ */


/**
 * Initialise all ray directions for the current frame.
 */
void initializeRayDirections() {
    int n;
    Vector3f vdd = HOMOGENEOUS_V3;
    Vector3f vn = HOMOGENEOUS_V3;
    Vector3f rn = HOMOGENEOUS_V3;

    for(n = 0; n < VIEWPLANE_LENGTH; n++) {
		vdd = homogeneousVectorScale(playerDir, (float)distFromViewplane);
		vn = homogeneousVectorScale(normalizeHomogeneousVector(viewplaneDir), (VIEWPLANE_LENGTH/2 - (float)n));
		rn = homogeneousVectorSubtract(vdd, vn);
		rays[n].vec = normalizeHomogeneousVector(rn);
    }
}

/**
 * Find the initial vertical ray intersection step vector.
 *
 * rayVec: The normalized ray to find the initial step vector for.
 *
 * Returns: The initial vertical ray intersection step vector.
 */
Vector3f findInitialVerticalRayStepVector(Vector3f rayVec, Vector3f rayStart) {
    Vector3f sVec = HOMOGENEOUS_V3;
    Vector3f retVec = HOMOGENEOUS_V3;

    /**x value of the west vetical wall of the grid square the player occupies**/
    int westX = ((int)(rayStart.x / (float)WALL_SIZE)) * WALL_SIZE;

    /**Find x value for s**/
    if(rayVec.x < 0) { /**Ray is facing left**/
		sVec.x =  westX - rayStart.x;
    } else { /**Ray is facing right**/
		sVec.x = westX + WALL_SIZE - rayStart.x;
    }

    /**Calculate the initial stepping vector**/
	retVec = homogeneousReciprocalVectorProjection(sVec, rayVec);

    return retVec;
}

/**
 * Find the initial horizontal ray intersection step vector.
 *
 * rayVec: The normalized ray to find the initial step vector for.
 *
 * Returns: The initial horizontal ray intersection step vector.
 */
Vector3f findInitialHorizontalRayStepVector(Vector3f rayVec, Vector3f rayStart) {
	Vector3f sVec = HOMOGENEOUS_V3;
	Vector3f retVec = HOMOGENEOUS_V3;

	/**y value of the north vetical wall of the grid square the player occupies**/
	int northY = ((int)(rayStart.y / WALL_SIZE)) * WALL_SIZE;

	/**Find y value for s**/
	if (rayVec.y < 0) { /* Ray is facing up */
		sVec.y = northY - rayStart.y;

	}
	else { /* Ray is facing down */
		sVec.y =  northY + WALL_SIZE - rayStart.y;

	}

	/**Calculate the initial stepping vector**/
	retVec = homogeneousReciprocalVectorProjection(sVec, rayVec);

	return retVec;
}

/**
 * Find the main vertical ray intersection step vector.
 *
 * rayVec: The normalized ray to find the main step vector for.
 *
 * Returns: The main vertical ray intersection step vector.
 */
Vector3f findVerticalRayStepVector(Vector3f rayVec) {
	Vector3f sVec = HOMOGENEOUS_V3;
	Vector3f retVec = HOMOGENEOUS_V3;

	/**Find x value for s**/
	if (rayVec.x < 0) { /* Ray is facing left */
		sVec.x = -WALL_SIZE;
	}
	else { /* Ray is facing right */
		sVec.x = WALL_SIZE;
	}

	/**Calculate the main stepping vector**/
	retVec = homogeneousReciprocalVectorProjection(sVec, rayVec);

	return retVec;
}

/**
 * Find the main horizontal ray intersection step vector.
 *
 * rayVec: The normalized ray to find the main step vector for.
 *
 * Returns: The main horizontal ray intersection step vector.
 */
Vector3f findHorizontalRayStepVector(Vector3f rayVec) {
    Vector3f sVec = HOMOGENEOUS_V3;
    Vector3f retVec = HOMOGENEOUS_V3;

    /**Find y value for s**/
    if(rayVec.y < 0) { /* Ray is facing up */
		sVec.y = -WALL_SIZE;
    } else { /* Ray is facing down */
		sVec.y = WALL_SIZE;
    }

    /**Calculate the main stepping vector**/
	retVec = homogeneousReciprocalVectorProjection(sVec, rayVec);;

    return retVec;
}

/**
 * Perform the raycaster stepping algorithm on a ray.
 *
 * ray: The ray to perform raycasting on.
 * rayStart: The start point of the ray (the camera).
 *
 * Returns: The ray after raycasting has been performed.
 */
Ray raycast(Ray ray, Vector3f rayStart) {
    Vector3f vray = HOMOGENEOUS_V3;
	Vector3f hray = HOMOGENEOUS_V3;
	Vector3f vinitialStep = findInitialVerticalRayStepVector(ray.vec, rayStart);
	Vector3f hinitialStep = findInitialHorizontalRayStepVector(ray.vec, rayStart);
	Vector3f vstep = findVerticalRayStepVector(ray.vec);
	Vector3f hstep = findHorizontalRayStepVector(ray.vec);
    Vector3f mapCoord;

    /**Cast the vertical ray until it hits something**/

    /**Set the initial vray length**/
	vray = vinitialStep;

    /**Get the row,col coordinate of the grid square the ray occupies**/
    mapCoord = getTileCoordinateForVerticalRay(vray, rayStart);
    while(mapCoord.x > 0 && mapCoord.y > 0 && mapCoord.x < WORLD_GRID_COLUMNS && mapCoord.y < WORLD_GRID_ROWS && WORLD_GRID[(int)mapCoord.y][(int)mapCoord.x] < 1) {
		

        /**Update the ray**/
		vray = homogeneousVectorAdd(vray, vstep);

        /**Update the row,col coordinate of the grid square the ray occupies**/
        mapCoord = getTileCoordinateForVerticalRay(vray, rayStart);
    }




    /**Cast the horizontal ray until it hits something**/

    /**Set the initial hray length**/
	hray = hinitialStep;

    /**Get the grid row,col coordinate of the ray**/
    mapCoord = getTileCoordinateForHorizontalRay(hray, rayStart);
    while(mapCoord.x > 0 && mapCoord.y > 0 && mapCoord.x < WORLD_GRID_COLUMNS && mapCoord.y < WORLD_GRID_ROWS && WORLD_GRID[(int)mapCoord.y][(int)mapCoord.x] < 1) {
		

        /**Update the ray**/
		hray = homogeneousVectorAdd(hray, hstep);

        /**Update the grid row,col coordinate of the ray**/
        mapCoord = getTileCoordinateForHorizontalRay(hray, rayStart);
    }

    /**Set Rn**/
    
    if(homogeneousVectorMagnitude( vray) < homogeneousVectorMagnitude( hray)) {
        ray.vec = vray;
        ray.intersectionType = VERTICAL;
    } else {
        ray.vec = hray;
        ray.intersectionType = HORIZONTAL;
    }
    

    return ray;
}

/**
 * Initialize the raycasting environment.
 */
void setupRaycaster() {

    /**Infer viewplane distance from a given field of view angle (theta = FOV)**/
	distFromViewplane = (VIEWPLANE_LENGTH) / (2* tan (FOV/2));

    /**Setup player rotation matrices (theta = PLAYER_ROT_SPEED)**/
	clockwiseRotation[0][0] = cos(PLAYER_ROT_SPEED);
	clockwiseRotation[0][1] = sin(PLAYER_ROT_SPEED);
	clockwiseRotation[1][0] = -sin(PLAYER_ROT_SPEED);
	clockwiseRotation[1][1] = cos(PLAYER_ROT_SPEED);
	clockwiseRotation[2][2] = 1;

	counterClockwiseRotation[0][0] = cos((float)PLAYER_ROT_SPEED);
	counterClockwiseRotation[0][1] = -sin((float)PLAYER_ROT_SPEED);
	counterClockwiseRotation[1][0] = sin((float)PLAYER_ROT_SPEED);;
	counterClockwiseRotation[1][1] = cos((float)PLAYER_ROT_SPEED);
	counterClockwiseRotation[2][2] = (float)1;
}


/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE BELOW THIS LINE                    */
/* ============================================================================ */

/**
 * Update the raycaster for the current frame.
 */
void updateRaycaster() {
    int i;

    /* Update the ray directions */
    initializeRayDirections();

    /* Perform raycasting */
    for(i = 0; i < VIEWPLANE_LENGTH; i++)
        rays[i] = raycast(rays[i], playerPos);

}

/**
 * Get the row and column number of the cube that a vertical
 * ray is intersecting. (Assuming the ray has already been cast)
 *
 * ray: The vertical ray
 * rayStart: The start position of the ray (camera position)
 *
 * Returns: A vector v where v.x is the column of the intersected cube, and v.y is the row of the intersected cube.
 */
Vector3f getTileCoordinateForVerticalRay(Vector3f ray, Vector3f rayStart) {
    Vector3f pos = homogeneousVectorAdd(rayStart, ray);
    Vector3f coord;
    coord.x = (int)(pos.x + ((ray.x < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;
    coord.y = (int)(pos.y + ((ray.y < 0) ? (-1 * EPS) : (EPS))) / WALL_SIZE;

    return coord;
}

/**
 * Get the row and column number of the cube that a horizontal
 * ray is intersecting. (Assuming the ray has already been cast)
 *
 * ray: The horizontal ray
 * rayStart: The start position of the ray (camera position)
 *
 * Returns: A vector v where v.x is the column of the intersected cube, and v.y is the row of the intersected cube.
 */
Vector3f getTileCoordinateForHorizontalRay(Vector3f ray, Vector3f rayStart) {
    Vector3f pos = homogeneousVectorAdd(rayStart, ray);
    Vector3f coord;
    coord.x = (int)(pos.x + ((ray.x < 0) ? (-1 * EPS) : EPS)) / WALL_SIZE;
    coord.y = (int)(pos.y + ((ray.y < 0) ? (-1 * RAY_EPS) : (RAY_EPS))) / WALL_SIZE;

    return coord;
}
