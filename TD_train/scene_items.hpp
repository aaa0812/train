#pragma once

#include "global.hpp"
#include "grid.hpp"
#include "custom_shapes.hpp"
#include "textures.hpp"

extern std::vector<Position> lightsPos;

void drawLever(int posX = 0, int posY = 0, float angle = 0);

void drawLantern(int posX, int posY, float scale = 1.f, bool simpleLantern = true);

void drawCompleteLantern(int posX, int posY, float angle = 0);

void drawCrate(int posX, int posY);