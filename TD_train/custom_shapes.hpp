#pragma once

#include "global.hpp"

extern GLBI_Convex_2D_Shape trapezoid;
extern IndexedMesh *cylinder;
extern GLBI_Convex_2D_Shape disk;
extern IndexedMesh *cube;
extern IndexedMesh *sphere;
extern StandardMesh *cone;

void initShapes();
void drawTrapezoid();
void drawClosedCylinder(float const lenght = 1, float const width = 1, float const height = 1);