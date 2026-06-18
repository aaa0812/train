#pragma once

#include "global.hpp"
#include "custom_shapes.hpp"
#include "scene_items.hpp"
#include "textures.hpp"
#include "railroad.hpp"
#include "train.hpp"

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

void drawTrainStation(int posX, int posY, float angle = 0);

void drawCrate(int posX, int posY);

void drawScene(double time_ellapsed, bool displayGrid = false);
