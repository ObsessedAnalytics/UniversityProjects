#include <stdio.h>

#include "373assignment.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"

/**
 * Draw the overhead map.
 */
void renderOverheadMap() {
    int i, row, col;
    float mapGridSquareSize = (float)HUD_MAP_SIZE / (float)WORLD_GRID_COLUMNS;
    int mapXOffset = (WINDOW_WIDTH - HUD_MAP_SIZE) / 2;
    int mapYOffset = (WINDOW_HEIGHT - HUD_MAP_SIZE) / 2;


    /* Draw map tiles */
    for(row = 0; row < WORLD_GRID_ROWS; row++) {
        for(col = 0; col < WORLD_GRID_COLUMNS; col++) {
            switch(WORLD_GRID[row][col]) {
                case R:
                    setDrawColor(255, 0, 0, 255);
                    break;
                case G:
                    setDrawColor(0, 255, 0, 255);
                    break;
                case B:
                    setDrawColor(0, 0, 255, 255);
                    break;
                case W:
                    setDrawColor(128, 128, 128, 255);
                    break;
                default:
                    setDrawColor(255, 255, 255, 255);
                    break;
            }
            fillRect((int)(mapGridSquareSize * col) + mapXOffset, (int)(mapGridSquareSize * row) + mapYOffset, mapGridSquareSize, mapGridSquareSize);
        }
    }

    /* Draw rays */
    setDrawColor(200, 100, 50, 255);
    for(i = 0; i < WINDOW_WIDTH; i++) {
        Vector3f ray = rays[i].vec;
        drawLine((int)(playerPos.x * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapXOffset, (int)(playerPos.y * HUD_MAP_SIZE / (float)WORLD_PIXEL_HEIGHT + mapYOffset),
                (int)((playerPos.x + ray.x) * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapXOffset, (int)((playerPos.y + ray.y) * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapYOffset);
    }

    /* Draw player line */
    setDrawColor(200, 0, 0, 255);
    drawLine((int)(playerPos.x * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapXOffset, (int)(playerPos.y * HUD_MAP_SIZE / (float)WORLD_PIXEL_HEIGHT + mapYOffset),
            (int)((playerPos.x + PLAYER_SIZE * playerDir.x) * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapXOffset, (int)((playerPos.y + PLAYER_SIZE * playerDir.y) * HUD_MAP_SIZE / (float)WORLD_PIXEL_WIDTH) + mapYOffset);

    setDrawColor(128, 128, 128, 255);
	presentRenderer();
}
