#pragma once

#include "global.hpp"
#include "custom_shapes.hpp"

enum Orientation
{
    Left,
    Right,
    Top,
    Bottom,
    BottomRight,
    BottomLeft,
    TopRight,
    TopLeft,
};

struct Rail
{
    Position pos;
    Orientation dir;
};

extern std::vector<Rail> railroad;

void drawOneRail();

void drawOneCurvedRail(int maxIteration = 10, float position = 3);

void drawBallast();

void drawCompleteRail(int posX, int posY, float rotation);

void drawCompleteCurvedRail(int posX, int posY, float rotation, STP3D::Vector3D offset = {0.f, 0.f, 0.f});

void drawRailRoad();

void drawTrain(int posX = 0, int posY = 0, float angle = 0);

void moveTrain();

Orientation defineCurveDir(Position prev, Position current, Position next);

Orientation defineDir(Position current, Position next);