#ifndef RENDERER_H
#define RENDERER_H

#include "373gfx.h"
#include "linalg.h"
#include "raycaster.h"

/* Macros */
#define XY_TO_SCREEN_INDEX(X, Y)   (((Y) * WINDOW_WIDTH) + (X))
#define XY_TO_TEXTURE_INDEX(X, Y)   (((Y) * TEXTURE_SIZE) + (X))

/* Functions */
static float calculateWallColumnHeight(float rayLength);
static float getUndistortedRayLength(Ray ray);
static void drawTexturedWallColumn(int x, float wallYStart, float length, Uint32* texture, int textureCol, float illumination);
static void drawUntexturedWallColumn(int x, float wallYStart, float length, Uint32 ABGRColor, float illumination);
static int getTextureColumnNumberForRay(Ray ray, Vector3f rayStart);
void renderProjectedScene();

#endif /* RENDERER_H */
