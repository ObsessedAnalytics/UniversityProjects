/*
 * ===================================================
 * ===================================================
 *              373 SDL Graphics Wrapper
 *                     Revision 2
 *
 *            (C) 2015  Nick Stones-Havas
 *             nsto032@aucklanduni.ac.nz
 * ===================================================
 *
 *  C library for drawing to the screen.
 *
 *  Written for use in Compsci 373
 *
 * ===================================================
 *
 *   !!!!  DO NOT MODIFY ANY OF THE BELOW CODE  !!!!
 *
 * ===================================================
 * ===================================================
 */
#ifndef GFX373_H
#define GFX373_H

#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif


/*========================================================
 * Macros
 *========================================================
 */

/**
 * Macro to convert from RGB color components to an ABGR integer
 */
#define RGBtoABGR(R,G,B)   (0xFF000000 | ((B) << 16) | ((G) << 8) | (R))

/**
 * Macro to retrieve the red color component from an ABGR integer
 */
#define ABGRtoR(A)         ((A) & 0x0FF)

/**
 * Macro to retrieve the green color component from an ABGR integer
 */
#define ABGRtoG(A)         (((A) >> 8) & 0x0FF)

/**
 * Macro to retrieve the blue color component from an ABGR integer
 */
#define ABGRtoB(A)         (((A) >> 16) & 0x0FF)

/**
 * Macro to darken an ABGR color
 */
#define DARKEN_COLOR(C)     ((((C) >> 1) & 0x7F7F7F7F) | 0xFF000000)


/*========================================================
 * Library debug functions
 *========================================================
 */

const char* gfxGetError();


/*========================================================
 * GFX management functions
 *========================================================
 */


int initGFX(char* title, unsigned int width, unsigned int height);
void* createTexture(unsigned int width, unsigned int height);
int destroyTexture(void* texture);
void displayFullscreenTexture(void* texture);
void destroyGFX();


/*========================================================
 * Drawing functions
 *========================================================
 */

void setDrawColor(int r, int g, int b, int a);
void drawLine(int x1, int y1, int x2, int y2);
void fillRect(int x, int y, int w, int h);
void drawRect(int x, int y, int w, int h);
void presentRenderer();
void clearRenderer();


/*========================================================
 * Utility functions
 *========================================================
 */

Uint32* generateXorTexture(int size, int redmask, int greenmask, int bluemask);
Uint32* generateRedXorTexture(int size);
Uint32* generateGreenXorTexture(int size);
Uint32* generateBlueXorTexture(int size);
Uint32* generateGrayXorTexture(int size);
Uint32 shadeColor(Uint32 abgr, float percent);

#endif /* GFX373_H */
