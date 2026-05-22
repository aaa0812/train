#pragma once

#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/basic_mesh.hpp"
#include "grid.hpp"

using namespace glbasimac;

/* Camera parameters and functions */
static const float Z_NEAR{0.1f};
static const float Z_FAR{500.f};
extern float angle_theta; // Angle between x axis and viewpoint
extern float angle_phy;   // Angle between z axis and viewpoint
extern float dist_zoom;   // Distance between origin and viewpoint
extern bool flag_anim_rot_scale;
extern bool flag_anim_rot_arm;

/* OpenGL Engine */
extern GLBI_Engine myEngine;

enum Orientation
{
    BottomRight,
    BottomLeft,
    TopRight,
    TopLeft,
};

void initScene(GridConfig const &gridConfig);

void drawGround(bool displayGrid = false);

void drawOneRail();

void drawOneCurvedRail(int maxIteration = 10, float position = 3);

void drawBallast();

void drawCompleteRail(int posX, int posY, float rotation);

void drawCompleteCurvedRail(int posX, int posY, float rotation, STP3D::Vector3D offset = {0.f, 0.f, 0.f});

void drawFrame();

void drawSphere(double time_ellapsed);

void rotateSphere(double time_ellapsed, float radius, STP3D::Vector3D origin_point = {0.0, 0.0, 0.0});

void drawRailRoad();

void drawScene(double time_ellapsed, bool displayGrid = false);

Orientation defineCurveDir(Position prev, Position current, Position next);
