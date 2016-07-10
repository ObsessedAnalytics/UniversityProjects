#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include "373gfx.h"
#include "linalg.h"

/* Misc. constants */
#define FALSE 0
#define TRUE  1
#define PI    3.14159265358979323846 /* C89 does not seem to define this constant */


/* Window parameters*/
#define WINDOW_WIDTH  640  /* The width of the rendering viewport */
#define WINDOW_HEIGHT 480  /* The height of the rendering viewport */

/* Raycaster parameters */
#define TEXTURE_SIZE           64  /* The size of a square wall texture in pixels */
#define WALL_SIZE              64  /* The size of a wall cube face in the world in pixels */
#define HUD_MAP_SIZE           WINDOW_HEIGHT
#define FOV                    (PI / 3.0f)               /* 60 degrees */
#define PLAYER_MOVEMENT_SPEED  5.0f                      /* 5 pixels per frame */
#define PLAYER_ROT_SPEED       ((3.0f * (PI)) / 180.0f)  /* 3 degrees per frame */
#define PLAYER_SIZE            20                        /* Player is 20 pixels wide*/

/* Projection parameters */
#define VIEWPLANE_LENGTH  WINDOW_WIDTH                   /* The viewplane length is the same as the window width */
#define VIEWPLANE_DIR     {-1, 0, 1}                     /* The initial value of the viewplane direction vector */
#define PLAYER_DIR        {0, 1, 1} /* The initial value of the player direction vector */
#define PLAYER_START      {(2.5f * WALL_SIZE), (2.5f * WALL_SIZE), 1} /* Default player start coordinate */

/* World map constants */
#define WORLD_GRID_COLUMNS  10 /* Number of columns in the world grid */
#define WORLD_GRID_ROWS     10 /* Number of rows in the world grid */
#define WORLD_PIXEL_WIDTH   (WORLD_GRID_COLUMNS * WALL_SIZE) /* The width of the world in pixels */
#define WORLD_PIXEL_HEIGHT  (WORLD_GRID_ROWS * WALL_SIZE)    /* The height of the world in pixels */

/* Map wall types */
#define P            -1  /* Player start */
#define R             1  /* Red wall */
#define G             2  /* Green wall */
#define B             3  /* Blue wall */
#define W             4  /* Gray wall */

#define CEILING_COLOR  RGBtoABGR(0x65, 0x65, 0x65)
#define FLOOR_COLOR    RGBtoABGR(0xAA, 0xAA, 0xAA)


/* Globals */
extern const short WORLD_GRID[WORLD_GRID_ROWS][WORLD_GRID_COLUMNS];
extern char distortion;
extern char texturesOn;
extern Uint32* screenBuffer;
extern const Uint32 COLORS[];
extern Uint32* TEXTURES[];

#endif /* ASSIGNMENT_H */
