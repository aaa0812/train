#pragma once

#include "global.hpp"
#include "custom_shapes.hpp"

enum Orientation
{
    BottomRight,
    BottomLeft,
    TopRight,
    TopLeft,
};

void drawOneRail();

void drawOneCurvedRail(int maxIteration = 10, float position = 3);

void drawBallast();

void drawCompleteRail(int posX, int posY, float rotation);

void drawCompleteCurvedRail(int posX, int posY, float rotation, STP3D::Vector3D offset = {0.f, 0.f, 0.f});

void drawRailRoad();

Orientation defineCurveDir(Position prev, Position current, Position next);