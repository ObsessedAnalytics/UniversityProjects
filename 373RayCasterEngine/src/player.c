#include <stdlib.h>

#include "player.h"
#include "raycaster.h"
#include "373assignment.h"



/* Global data */
Vector3f playerPos    = PLAYER_START;
Vector3f playerDir    = PLAYER_DIR;

/* Control toggles */
char movingForward    = FALSE;
char movingBack       = FALSE;
char turningLeft      = FALSE;
char turningRight     = FALSE;
char playerIsRunning  = FALSE;

/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE ABOVE THIS LINE                    */
/* ============================================================================ */

/**
 * Rotate the projection environment for the player
 * given a rotation matrix.
 *
 * rotMatrix: The rotation matrix to be applied.
 */
void rotatePlayer(Matrix3f* rotMatrix) {
	matrixVectorMultiply(rotMatrix, &playerDir);
	matrixVectorMultiply(rotMatrix, &viewplaneDir);

}


/* ============================================================================ */
/*                    DO NOT MODIFY ANY CODE BELOW THIS LINE                    */
/* ============================================================================ */

/**
 * Update the players position for the current frame.
 */
void updatePlayer() {
    float moveSpeed = PLAYER_MOVEMENT_SPEED;

    if(playerIsRunning)
        moveSpeed *= 2;

    if(movingForward) {
        movePlayer(playerDir.x * moveSpeed, playerDir.y * moveSpeed);
    } if(movingBack) {
        movePlayer(-1 * playerDir.x * moveSpeed, -1 * playerDir.y * moveSpeed);
    } if(turningLeft) {
        rotatePlayer(&clockwiseRotation);
        if(playerIsRunning)
            rotatePlayer(&clockwiseRotation);
    } if(turningRight) {
        rotatePlayer(&counterClockwiseRotation);
        if(playerIsRunning)
            rotatePlayer(&counterClockwiseRotation);
    }

}

/**
 * Move the player by an offset.
 *
 * dx: The x offset
 * dy: The y offset
 */
void movePlayer(float dx, float dy) {

    /* Don't clip if the player doesn't intersect anything */
    if(!clipMovement(dx, dy)) {
        playerPos.x += dx;
        playerPos.y += dy;
        return;
    }

    /* Try clipping off only the x translation */
    if(!clipMovement(0.0f, dy)) {
        playerPos.y += dy;
        return;
    }

    /* Try clipping off only the y translation */
    if(!clipMovement(dx, 0.0f)) {
        playerPos.x += dx;
        return;
    }
}

/**
 * Check if the players movement should be clipped
 * given an x and y offset.
 *
 * dx: The x offset.
 * dy: The y offset.
 *
 * Returns TRUE if the player's movement should be clipped, FALSE otherwise.
 */
int clipMovement(float dx, float dy) {
    float newx = playerPos.x + dx;
    float newy = playerPos.y + dy;
    int x1 = (newx - PLAYER_SIZE) / WALL_SIZE;
    int y1 = (newy - PLAYER_SIZE) / WALL_SIZE;
    int x2 = (newx + PLAYER_SIZE) / WALL_SIZE;
    int y2 = (newy + PLAYER_SIZE) / WALL_SIZE;
    int i, j;

    /* Check all tiles the player occupies */
    for(i = y1; i <= y2; i++) {
        for(j = x1; j <= x2; j++) {
            if(i < 0 || j < 0 || i > WORLD_GRID_ROWS || j > WORLD_GRID_COLUMNS || WORLD_GRID[i][j] > 0) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/**
 * Initialize the player.
 */
void setupPlayer() {
    int row, col;

    /* Search for player position in map */
    for(row = 0; row < WORLD_GRID_ROWS; row++) {
        for(col = 0; col < WORLD_GRID_COLUMNS; col++) {
            if(WORLD_GRID[row][col] == P) {
                playerPos.x = (WALL_SIZE * col) + (WALL_SIZE / 2.0f);
                playerPos.y = (WALL_SIZE * row) + (WALL_SIZE / 2.0f);
                break;
            }
        }
    }
}
