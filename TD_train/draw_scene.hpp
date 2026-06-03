#pragma once

#include "global.hpp"
#include "custom_shapes.hpp"
#include "scene_items.hpp"
#include "train_items.hpp"
#include "railroad.hpp"

/* Camera parameters and functions */
static const float Z_NEAR{0.1f};
static const float Z_FAR{500.f};
extern float angle_theta; // Angle between x axis and viewpoint
extern float angle_phy;   // Angle between z axis and viewpoint
extern float dist_zoom;   // Distance between origin and viewpoint
extern bool flag_anim_rot_scale;
extern bool flag_anim_rot_arm;

void initScene(GridConfig const &gridConfig);

void drawGround(bool displayGrid = false);

void drawFrame();

void drawSphere(double time_ellapsed);

void rotateSphere(double time_ellapsed, float radius, STP3D::Vector3D origin_point = {0.0, 0.0, 0.0});

void drawLantern(int posX, int posY, float scale = 1.f);

void drawCompleteLantern(int posX, int posY);

void drawScene(double time_ellapsed, bool displayGrid = false);
