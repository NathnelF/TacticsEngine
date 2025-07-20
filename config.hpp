
#ifndef CONFIG_H
#define CONFIG_H

#include "raylib.h"

#define GRID_WIDTH 30
#define GRID_HEIGHT 30
#define TILE_SIZE 4

const Color tileDefaultColorFaded = Fade(LIGHTGRAY, 0.02f);
const Color tileDefaultColor = Fade(LIGHTGRAY, 0.50f);

const Color tileScootColorFaded = Fade(LIME, 0.02f);
const Color tileScootColor = Fade(LIME, 0.50f);
const Color tileScootColorBright = Fade(GREEN, 1.0f);

const Color tileDashColorFaded = Fade(ORANGE, 0.02f);
const Color tileDashColor = Fade(ORANGE, 0.50f);

const Color tileUnavailableColor = Fade(MAROON, 0.50f);

#endif

