#include <stdlib.h>

#include "373assignment.h"
#include "renderer.h"
#include "player.h"


/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE ABOVE THIS LINE                    */
/* ============================================================================ */

/**
 * Calculate the height in pixels of a wall column, given the
 * length of a ray that hits it.
 *
 * rayLength: The length of the ray which hits the wall.
 *
 * Returns: The height (in pixels) of the wall column to be drawn.
 */
float calculateWallColumnHeight(float rayLength) {
	float wallColSize;
	wallColSize = distFromViewplane * (WALL_SIZE / rayLength);

	return wallColSize;
}

/**
 * Given a ray, get the length of the undistorted ray.
 *
 * ray: An uncorrected ray.
 *
 * Returns: The undistorted ray length
 */
float getUndistortedRayLength(Ray ray) {
	float undistortedLength;
    Vector3f undistortedRay = HOMOGENEOUS_V3;

	undistortedRay = homogeneousVectorSubtract(ray.vec, homogeneousVectorProjection(ray.vec, viewplaneDir));
	undistortedLength = homogeneousVectorMagnitude(undistortedRay);

    return undistortedLength;
}

/**
 * Draw an untextured wall column strip to the viewport.
 *
 * x: The x coordinate of the viewport to draw the strip to.
 * wallYStart: The y coordinate of the viewport to start drawing a wall strip at.
 * length: The length of the wall strip to be drawn.
 * ABGRColor: The color to use for the wall column.
 * illumination: The shading factor for the wall strip, where 0 is the darkest, and 1 is the brightest.
 */
void drawUntexturedWallColumn(int x, float wallYStart, float length, Uint32 ABGRColor, float illumination) {
    int y;
	Uint32 shadedColor;
    if(wallYStart < 0)
        wallYStart = 0;

	/* Apply shading factor to the color */
	shadedColor = shadeColor(ABGRColor, illumination);

	for (y = 0; y < WINDOW_HEIGHT; y++) {
		if (y < wallYStart) {
			screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = CEILING_COLOR;

		} else if (y > (wallYStart + length)) {
			screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = FLOOR_COLOR;
		} else {
			screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = shadedColor;
        }
    }
}

/**
 * Draw a textured wall column strip to the viewport.
 *
 * x: The x coordinate of the viewport to draw the strip to.
 * wallYStart: The y coordinate of the viewport to start drawing at.
 * length: The length of the wall strip to be drawn.
 * texture: A pointer to the texture to be drawn.
 * textureCol: The column number of the texture strip being drawn.
 * illumination: The shading factor for the wall strip, where 0 is the darkest, and 1 is the brightest.
 */
void drawTexturedWallColumn(int x, float wallYStart, float length, Uint32* texture, int textureCol, float illumination) {
    int y;
    float d, ty;
    Uint32 color;

    if(wallYStart < 0)
        wallYStart = 0;

    for(y = 0; y < WINDOW_HEIGHT; y++) {
		/* scale the texture (ty is the y coordinate of the scaled texture that we want to draw) */
        d = y - (WINDOW_HEIGHT / 2.0f) + length / 2.0f;
        ty = d * (float)(TEXTURE_SIZE-EPS) / length;

		if (y < wallYStart) {
			/* ? */

		} else if (y > (wallYStart + length)) {
			/* ? */

		} else {
			/* Uint32 textureColor = texture[XY_TO_TEXTURE_INDEX(?, ?)]; */
            color = shadeColor(color, illumination);

            screenBuffer[XY_TO_SCREEN_INDEX(x, y)] = color;
        }
    }

}

/**
 * Get the texture column number for a given ray intersection.
 *
 * ray: The ray to get the texture column number for.
 * rayStart: The start position of the ray (the camera).
 *
 * Returns: The texture column number.
 */
int getTextureColumnNumberForRay(Ray ray, Vector3f rayStart) {
	/**World x,y coordinate of the ray intersection**/
    /**Vector3f rayHitPos = **/

    if(ray.intersectionType == HORIZONTAL) {
		int texCol;
		/**The texture column number we want for this ray**/
        /* texCol = ? */

		/* Reverse the texture if the ray is facing down */
		if (ray.vec.y > 0)
			texCol = TEXTURE_SIZE - texCol - 1;

		return texCol;
    } else {
		int texCol;
		/**The texture column number we want for this ray**/
        /* texCol = ? */

		/* Reverse the texture if the ray is facing to the left */
		if (ray.vec.x < 0)
			texCol = TEXTURE_SIZE - texCol - 1;

		return texCol;
    }
}

/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE BELOW THIS LINE                    */
/* ============================================================================ */


/**
 * Draw the current view to the screen.
 */
void renderProjectedScene() {
    int i;

    for(i = 0; i < WINDOW_WIDTH; i++) {
        Ray ray = rays[i];
        int textureX = 0;
        int mapx, mapy;
        float drawLength;
        float illumination;
        Uint32* texture;

        Vector3f coords;
        if(ray.intersectionType == VERTICAL)
            coords = getTileCoordinateForVerticalRay(ray.vec, playerPos);
        else
            coords = getTileCoordinateForHorizontalRay(ray.vec, playerPos);
        mapx = coords.x;
        mapy = coords.y;


        /* Calculate shading */
        if(rays[i].intersectionType == VERTICAL)
            illumination = 0.5f;
        else
            illumination = 1.0f;

        if(texturesOn)
            textureX = getTextureColumnNumberForRay(ray, playerPos);

        if(distortion)
            drawLength = calculateWallColumnHeight(homogeneousVectorMagnitude(ray.vec));
        else
            drawLength = calculateWallColumnHeight(getUndistortedRayLength(ray));

        if(texturesOn) {
            int texnum = WORLD_GRID[mapy][mapx];
            if(texnum < 1 || texnum > 4)
                texnum = 4;
            drawTexturedWallColumn(i, (WINDOW_HEIGHT / 2.0f) - (drawLength / 2.0f), drawLength, TEXTURES[texnum - 1], textureX, illumination);

        } else {
            int color = WORLD_GRID[mapy][mapx];
            if(color < 1 || color > 4)
                color = 4;
            drawUntexturedWallColumn(i, (WINDOW_HEIGHT / 2.0f) - (drawLength / 2.0f), drawLength, COLORS[color - 1], illumination);
        }
    }

    clearRenderer();
    displayFullscreenTexture(screenBuffer);
}
