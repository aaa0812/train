#include "draw_scene.hpp"

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint
bool flag_anim_rot_scale{false};
bool flag_anim_rot_arm{false};

GridConfig config;

double timeEllapsed;

GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape grid{3};

IndexedMesh *sphere;
StandardMesh *cone;
IndexedMesh *cube;

const int CELLSIZE = 10.f;

std::vector<Position> lightsPos;

void initScene(GridConfig const &gridConfig)
{
	myEngine.switchToPhongShading();
	myEngine.setLightPosition({0, 0, 20, 1});
	myEngine.setLightIntensity({600, 500, 300});
	myEngine.switchToPhongShading();

	config = gridConfig;

	// add positions of lanterns in the vec lightsPos
	lightsPos.emplace_back(Position(-2, 2));
	lightsPos.emplace_back(Position(-3, -3));
	lightsPos.emplace_back(Position(-1, -1));

	// loop on lightsPos to place lights
	for (Position pos : lightsPos)
	{
		myEngine.addALight(Vector4D(pos.x * CELLSIZE + CELLSIZE / 2, pos.y * CELLSIZE + CELLSIZE / 2, 4, 1), Vector3D(100, 50, 10));
	}

	myEngine.addALight(Vector4D(0, 0, 0, 1), Vector3D(10, 10, 5));
	myEngine.addALight(Vector4D(0, 0, 0, 1), Vector3D(10, 10, 5));

	std::vector<float> points{0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0};
	std::vector<float> colors{0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

	somePoints.initSet(points, colors);

	std::vector<float> groundSquare{0, 0, 0.0,
									1, 0, 0.0,
									1, 1, 0.0,
									0, 1, 0.0};
	std::vector<float> gridSquare{0, 0, 0.0,
								  1, 0, 0.0,
								  1, 0, 0.0,
								  1, 1, 0.0,
								  1, 1, 0.0,
								  0, 1, 0.0};

	ground.initShape(groundSquare);
	ground.changeNature(GL_TRIANGLE_FAN);
	grid.initShape(gridSquare);
	grid.changeNature(GL_LINES);

	sphere = basicSphere(1.0);
	sphere->createVAO();

	cone = basicCone(1.0, 1.0);
	cone->createVAO();

	cube = basicCube(1.0);
	cube->createVAO();

	loadTextures();
}

void drawGround(bool displayGrid)
{
	myEngine.setNormalForConvex2DShape({0, 0, 1});
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(CELLSIZE);
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-config.size_grid / 2, -config.size_grid / 2, 0}); // to center the ground
		for (int i = 0; i < config.size_grid; i++)
		{
			for (int j = 0; j < config.size_grid; j++)
			{
				myEngine.mvMatrixStack.pushMatrix();
				{
					myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{i, j, 0});
					myEngine.setFlatColor(0.5, 0.5, 0.5);
					myEngine.activateTexturing(true);
					groundTexture.attachTexture();
					myEngine.mvMatrixStack.pushMatrix();
					myEngine.mvMatrixStack.addHomothety(Vector3D(1, 1, 1 / CELLSIZE));
					myEngine.updateMvMatrix();
					cube->draw();
					myEngine.mvMatrixStack.popMatrix();
					groundTexture.detachTexture();
					myEngine.activateTexturing(false);
					if (displayGrid)
					{
						myEngine.setFlatColor(1, 0, 0);
						grid.drawShape();
					}
				}
				myEngine.mvMatrixStack.popMatrix();
			}
		}
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.setNormalForConvex2DShape({0, 0, 0});
}

void drawOneRail()
{
	const float SR = 1.f; // rail weight and heigth
	const int LENGTH = CELLSIZE;
	myEngine.setFlatColor(0.3, 0.3, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{SR, LENGTH, SR});
		myEngine.updateMvMatrix();
		cube->draw();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawOneCurvedRail(int maxIteration /*default = 10*/, float position /*default = 3*/)
{
	const float SR = 1.f; // rail weight and heigth
	const int LENGTH = SR;
	myEngine.setFlatColor(0.3, 0.3, 0.3);

	for (int i = 0; i < maxIteration; i++)
	{
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addRotation(-M_PI / 2 * i / maxIteration, STP3D::Vector3D{0, 0, 1});
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, position, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{SR, LENGTH, SR});
			myEngine.updateMvMatrix();
			cube->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
	}
	myEngine.updateMvMatrix();
}

void drawBallast()
{
	const float RR = 0.5f; // ballast radius
	const int LENGTH = 6;
	myEngine.setFlatColor(0.6, 0.4, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.updateMvMatrix();
		drawClosedCylinder(LENGTH, RR, RR);
	}
	myEngine.mvMatrixStack.popMatrix();
}

void drawCompleteRail(int posX, int posY, float rotation)
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{5, 5, 0}); // center rail on grid cell
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{CELLSIZE * posX, CELLSIZE * posY, 0});
	myEngine.mvMatrixStack.addRotation(rotation, STP3D::Vector3D{0, 0, 1});
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, -6, 0});
		for (int i = 0; i < 5; i++)
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 2, 0});
			myEngine.updateMvMatrix();
			drawBallast();
		}
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2, 0, 1.f}); // translate on the z axis to make the rails on top of the ballast
		myEngine.updateMvMatrix();
		drawOneRail();
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{4, 0, 0});
		myEngine.updateMvMatrix();
		drawOneRail();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawCompleteCurvedRail(int posX, int posY, float rotation, STP3D::Vector3D offset)
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{CELLSIZE * posX, CELLSIZE * posY, 0});
	myEngine.mvMatrixStack.addRotation(rotation, STP3D::Vector3D{0, 0, 1});
	myEngine.mvMatrixStack.addTranslation(offset); // need an offset to keep the rail in the right grid cell
	for (int i = 1; i < 6; i += 2)
	{
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addRotation(-M_PI + (M_PI * i / 12), STP3D::Vector3D{0, 0, 1});
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-5, 0, 0});
			myEngine.updateMvMatrix();
			drawBallast();
		}
		myEngine.mvMatrixStack.popMatrix();
	}

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, 1.f}); // translate on the z axis to make the rails on top of the ballast
		myEngine.updateMvMatrix();
		drawOneCurvedRail(10, 3);
		drawOneCurvedRail(20, 7);
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawFrame()
{
	somePoints.changeNature(GL_LINES);
	somePoints.drawSet();
}

void drawSphere(double time_ellapsed)
{
	STP3D::Vector3D rotation_origin = {0.0, 0.0, 5.0};
	myEngine.setFlatColor(1.0, 1.0, 1.0);
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(rotation_origin);
		myEngine.mvMatrixStack.addHomothety(3.0);
		rotateSphere(time_ellapsed, 4.0, rotation_origin);
		myEngine.updateMvMatrix();
		sphere->draw();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void rotateSphere(double time_ellapsed, float radius, STP3D::Vector3D /* origin_point = {0.0, 0.0, 0.0} */)
{
	STP3D::Vector3D new_position(sin(time_ellapsed) * radius, cos(time_ellapsed) * radius, 1.0);
	myEngine.mvMatrixStack.addTranslation(new_position);
	myEngine.updateMvMatrix();
}

void drawRailRoad()
{
	for (int i = 0; i < config.path.size(); i++)
	{
		Position prev = i - 1 >= 0 ? config.path[i - 1] : config.path[config.path.size() - 1]; // position of the previous rail : if it exists -> previous pos in the list , else -> last item of the list
		Position next = i + 1 < config.path.size() ? config.path[i + 1] : config.path[0];	   // position of the next rail : if it exists -> next pos in the list , else -> first item of the list

		if (prev.x != next.x && prev.y != next.y) // check for a turn
		{
			Orientation curve = defineCurveDir(prev, {config.path[i].x, config.path[i].y}, next);
			switch (curve)
			{
			case BottomRight:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, -M_PI / 2, STP3D::Vector3D{-CELLSIZE, 0, 0});
				break;
			case BottomLeft:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, M_PI, STP3D::Vector3D{-CELLSIZE, -CELLSIZE, 0});
				break;
			case TopRight:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, 0);
				break;
			case TopLeft:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, M_PI / 2, STP3D::Vector3D{0, -CELLSIZE, 0});
				break;

			default:
				break;
			}
		}
		else
		{
			if (next.y != config.path[i].y) // vertical rail
			{
				drawCompleteRail(config.path[i].x, config.path[i].y, 0.f);
			}
			else // next.x != current x : horizontal rail
			{
				drawCompleteRail(config.path[i].x, config.path[i].y, -M_PI / 2);
			}
		}
	}
}

void drawTrainBar()
{
	const float SR = 4.f;
	const float RR = 1.f;
	const int LENGTH = 7;

	myEngine.setFlatColor(0.25f, 0.25f, 0.25f);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{CELLSIZE / 2, CELLSIZE / 2, 5});

	myEngine.mvMatrixStack.pushMatrix(); // train bar
	{
		myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2.5f, 0, -SR / 2});
		// myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.25F, 6, 0.25f});

		myEngine.updateMvMatrix();
		// cylinder->draw();
		drawClosedCylinder(6, 0.25f, 0.25f);
	}

	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix(); // right rear wheel
	{
		myEngine.setFlatColor(0.6f, 0.6f, 0.6f);
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2.5f, -(SR / 2) - 0.25f, -SR / 2});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1, 1, 1});

		myEngine.updateMvMatrix();
		// cylinder->draw();
		drawClosedCylinder(1, 1, 1);
	}

	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix(); // left rear wheel
	{
		myEngine.setFlatColor(0.6f, 0.6f, 0.6f);
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2.5f, +(SR / 2) + 0.25f, -SR / 2});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1, 1, 1});

		myEngine.updateMvMatrix();
		// cylinder->draw();
		drawClosedCylinder(1, 1, 1);
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawTrainLights()
{
	const float tileCenter = CELLSIZE / 2;
	myEngine.setFlatColor(0.25f, 0.25f, 0.25f);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{tileCenter, tileCenter, 5});

	myEngine.setLightPosition(Vector4D(tileCenter + 1, tileCenter + 5.f, 5 + 0.5f, 1), lightsPos.size() + 1);
	myEngine.setLightPosition(Vector4D(tileCenter - 1, tileCenter + 5.f, 5 + 0.5f, 1), lightsPos.size() + 2);

	myEngine.setLightIntensity(Vector3D(10 + fmod(timeEllapsed, 0.5), 10 + fmod(timeEllapsed, 0.5), 5 + fmod(timeEllapsed, 0.5)), lightsPos.size() + 1);
	myEngine.setLightIntensity(Vector3D(10 + fmod(timeEllapsed, 0.5), 10 + fmod(timeEllapsed, 0.5), 5 + fmod(timeEllapsed, 0.5)), lightsPos.size() + 2);

	myEngine.mvMatrixStack.pushMatrix(); // socle lights droit
	{
		myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
		// myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{1, 3.5f, 0.5f});
		// myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.25F, 6, 0.25f});

		myEngine.updateMvMatrix();
		drawClosedCylinder(0.75f, 0.75f, 0.75f);

		metalTexture.attachTexture();
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.setFlatColor(0.84f, 0.74f, 0.14f);
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0.75f, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.65f, 0.40f, 0.65f});
			myEngine.updateMvMatrix();
			sphere->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
		metalTexture.detachTexture();
	}

	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix(); // socle lights gauche
	{
		myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
		// myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-1, 3.5f, 0.5f});
		// myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.25F, 6, 0.25f});

		myEngine.updateMvMatrix();
		drawClosedCylinder(0.75f, 0.75f, 0.75f);

		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.setFlatColor(0.84f, 0.74f, 0.14f);
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0.75f, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.65f, 0.40f, 0.65f});
			myEngine.updateMvMatrix();
			sphere->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
	}

	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.popMatrix();
}

static void drawGoldNugget(const STP3D::Vector3D &translation, float angle, const STP3D::Vector3D &axis, const STP3D::Vector3D &scale)
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(translation);
	myEngine.mvMatrixStack.addRotation(angle, axis);
	myEngine.mvMatrixStack.addHomothety(scale);
	// myEngine.updateMvMatrix();
	myEngine.activateTexturing(true);
	goldTexture.attachTexture();
	myEngine.updateMvMatrix();
	cube->draw();
	goldTexture.detachTexture();
	myEngine.activateTexturing(false);
	myEngine.mvMatrixStack.popMatrix();
}

void drawGold()
{
	myEngine.setFlatColor(0.84f, 0.74f, 0.14f);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{CELLSIZE / 2, CELLSIZE / 2, 5});

	drawGoldNugget(							   // center nugget
		STP3D::Vector3D{-0.05f, 0.0f, 2.5f},   // translation (x,y,z)
		M_PI / 6,							   // angle (rad)
		STP3D::Vector3D{0, 1, 1},			   // axe (x,y,z)
		STP3D::Vector3D{2.65f, 2.90f, 2.85f}); // homothety (x,y,z)

	drawGoldNugget(							   // front - left nugget
		STP3D::Vector3D{-1.00f, 2.15f, 2.5f},  // translation (x,y,z)
		M_PI / 12,							   // angle (rad)
		STP3D::Vector3D{1, 1, 0},			   // axe (x,y,z)
		STP3D::Vector3D{1.75f, 1.75f, 1.75f}); // homothety (x,y,z)

	drawGoldNugget(							   // front - right nugget
		STP3D::Vector3D{0.85f, 1.85f, 2.30f},  // translation (x,y,z)
		-M_PI / 8,							   // angle (rad)
		STP3D::Vector3D{0, 1, 1},			   // axe (x,y,z)
		STP3D::Vector3D{1.90f, 2.10f, 1.90f}); // homothety (x,y,z)

	drawGoldNugget(							   // back - right nugget
		STP3D::Vector3D{0.85f, -1.85f, 2.30f}, // translation (x,y,z)
		M_PI / 4,							   // angle (rad)
		STP3D::Vector3D{0, 1, 1},			   // axe (x,y,z)
		STP3D::Vector3D{1.90f, 2.10f, 1.90f}); // homothety (x,y,z)

	drawGoldNugget(							   // back - left nugget
		STP3D::Vector3D{-0.95f, -1.f, 2.75f},  // translation (x,y,z)
		-M_PI / 6,							   // angle (rad)
		STP3D::Vector3D{0, 1, 1},			   // axe (x,y,z)
		STP3D::Vector3D{1.05f, 1.30f, 2.85f}); // homothety (x,y,z)

	drawGoldNugget(							   // back - left nugget
		STP3D::Vector3D{-0.95f, -2.4f, 2.25f}, // translation (x,y,z)
		M_PI / 4,							   // angle (rad)
		STP3D::Vector3D{1, 1, 0},			   // axe (x,y,z)
		STP3D::Vector3D{1.95f, 1.90f, 1.85f}); // homothety (x,y,z)

	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawTrain()
{
	const float SR = 4.f;
	const float RR = 1.f;
	const int LENGTH = 7;
	myEngine.setFlatColor(0.25f, 0.25f, 0.25f);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{CELLSIZE / 2, CELLSIZE / 2, 5});

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, -0.5f});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{3.f, 5, 4.f});
		myEngine.updateMvMatrix();
		cube->draw();
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{SR, LENGTH, SR});
		myEngine.activateTexturing(true);
		metalTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		metalTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.setFlatColor(0.6f, 0.6f, 0.6f);
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{SR + 0.75f, LENGTH + 0.75f, 0.5f});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, 3.6f});
		myEngine.updateMvMatrix();
		myEngine.activateTexturing(true);
		lightMetalTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		lightMetalTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawLantern(int posX, int posY, float scale)
{
	const float BaseSize = 2.f;
	const float LightHeight = 2.f;
	const float TopSize = 3.f;
	const float WEIGTH = 0.5f; // base and top weight

	myEngine.setFlatColor(0.3, 0.3, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, WEIGTH / 2}); // place the object on the cell center
		myEngine.mvMatrixStack.addHomothety(scale);
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{BaseSize, BaseSize, 1});

			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1, 1, WEIGTH});
				myEngine.updateMvMatrix();
				cube->draw();
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, WEIGTH / 2});
			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.setFlatColor(1, 1, 0.0);
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1, 1, LightHeight});
				myEngine.updateMvMatrix();
				drawTrapezoid();
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, LightHeight + WEIGTH / 2});
			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.setFlatColor(0.3, 0.3, 0.3);
				metalTexture.attachTexture();
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1.2, 1.2, WEIGTH});
				myEngine.updateMvMatrix();
				cube->draw();
				metalTexture.detachTexture();
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, WEIGTH / 2});
			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, 1});
				myEngine.mvMatrixStack.addRotation(M_PI, STP3D::Vector3D{0, 1, 0});
				myEngine.updateMvMatrix();
				drawTrapezoid();
			}
			myEngine.mvMatrixStack.popMatrix();
		}
		myEngine.mvMatrixStack.popMatrix();
	}
	myEngine.mvMatrixStack.popMatrix();
}

void drawCompleteLantern(int posX, int posY)
{
	const float WEIGTH = 1.f; // weight
	const int HEIGHT = 15;
	const int BeamLenght = HEIGHT / 2.5;
	const int SupportLenght = HEIGHT / 3;
	myEngine.setFlatColor(0.5, 0.4, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, 0}); // place the object on the cell center
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, HEIGHT / 2});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH, WEIGTH, HEIGHT});
			myEngine.updateMvMatrix();
			cube->draw();
		}
		myEngine.mvMatrixStack.popMatrix();

		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, HEIGHT}); // position beam at the top
			myEngine.mvMatrixStack.addRotation(-M_PI / 2, STP3D::Vector3D{1, 0, 0});
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, BeamLenght / 2 - WEIGTH});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH, WEIGTH, BeamLenght});
			myEngine.updateMvMatrix();
			cube->draw();
		}
		myEngine.mvMatrixStack.popMatrix();

		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 1.5 * WEIGTH, HEIGHT - SupportLenght / 2.5});
			myEngine.mvMatrixStack.addRotation(-M_PI / 5, STP3D::Vector3D{1, 0, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH / 1.2, WEIGTH / 1.2, SupportLenght});
			myEngine.updateMvMatrix();
			cube->draw();
		}
		myEngine.mvMatrixStack.popMatrix();

		myEngine.mvMatrixStack.pushMatrix(); // light
		{
			myEngine.setFlatColor(0.6, 0.5, 0.4);
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, BeamLenght * 0.75, HEIGHT - 3});
			myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{1, 0, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{0.1, 3, 0.1});
			myEngine.updateMvMatrix();
			cylinder->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, BeamLenght * 0.75, HEIGHT - 5});
		drawLantern(posX, posY, 0.75);
	}
	myEngine.mvMatrixStack.popMatrix();
}

void drawTrainStation(int posX, int posY, float angle)
{
	const float WEIGTH = 1.f; // weight
	const int HEIGHT = 15;
	const int BeamLenght = HEIGHT / 2.5;
	const int SupportLenght = HEIGHT / 3;
	myEngine.setFlatColor(0.5, 0.4, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, 0}); // place the object on the cell center
	myEngine.mvMatrixStack.addRotation(angle, Vector3D(0, 0, 1));

	for (int i = 0; i < 2; i++)
	{
		myEngine.mvMatrixStack.pushMatrix(); // POTENCES
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, -16.f * i, 0});

			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, HEIGHT / 2});
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH, WEIGTH, HEIGHT});

				myEngine.activateTexturing(true);
				dirtyWoodTexture.attachTexture();
				myEngine.updateMvMatrix();
				cube->draw();
				dirtyWoodTexture.detachTexture();
				myEngine.activateTexturing(false);
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-4, 0, HEIGHT});
				myEngine.mvMatrixStack.addRotation(-M_PI / 2, STP3D::Vector3D{0, 1, 0});
				myEngine.mvMatrixStack.addRotation(M_PI / 12, STP3D::Vector3D{0, 1, 0});
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH / 1.2, WEIGTH * 1.2, SupportLenght * 2.5});

				myEngine.activateTexturing(true);
				dirtyWoodTexture.attachTexture();
				myEngine.updateMvMatrix();
				cube->draw();
				dirtyWoodTexture.detachTexture();
				myEngine.activateTexturing(false);
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2, 0, HEIGHT - SupportLenght / 2.5});
				myEngine.mvMatrixStack.addRotation(-M_PI / 2, STP3D::Vector3D{0, 1, 0});
				myEngine.mvMatrixStack.addRotation(M_PI / 4, STP3D::Vector3D{0, 1, 0});
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{WEIGTH / 1.3, WEIGTH / 1.3, SupportLenght * 1.2});

				myEngine.activateTexturing(true);
				dirtyWoodTexture.attachTexture();
				myEngine.updateMvMatrix();
				cube->draw();
				dirtyWoodTexture.detachTexture();
				myEngine.activateTexturing(false);
			}
			myEngine.mvMatrixStack.popMatrix();
		}
		myEngine.mvMatrixStack.popMatrix();
	}

	for (int i = 0; i < 3; i++)
	{

		myEngine.mvMatrixStack.pushMatrix(); // planches
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, -8, 3 + 3.5 * i});
			myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 1, 0});
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{2.5, 16, 0.25});

			myEngine.activateTexturing(true);
			dirtyWoodTexture.attachTexture();
			myEngine.updateMvMatrix();
			cube->draw();
			dirtyWoodTexture.detachTexture();
			myEngine.activateTexturing(false);
		}
		myEngine.mvMatrixStack.popMatrix();
	}

	myEngine.mvMatrixStack.pushMatrix(); // SOL
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-2.5, -8, 0});
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{20, 8, 0.25});

		myEngine.activateTexturing(true);
		plankWoodTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		plankWoodTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix(); // TOIT
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-4, -8, HEIGHT});

		myEngine.mvMatrixStack.addRotation(M_PI / 12, STP3D::Vector3D{0, 1, 0});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{12, 16, 0.25});

		myEngine.activateTexturing(true);
		dirtyWoodTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		dirtyWoodTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawTexturedCube()
{
	myEngine.setFlatColor(1, 0, 0);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, 5});

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{4, 4, 4});
		myEngine.mvMatrixStack.addRotation(M_PI / 4, STP3D::Vector3D{0, 1, 0});
		myEngine.activateTexturing(true);
		metalTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		metalTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}

void drawScene(double time_ellapsed, bool displayGrid)
{
	timeEllapsed = time_ellapsed;
	drawFrame();
	myEngine.switchToPhongShading();

	// textureTest();
	drawGround(displayGrid);
	drawRailRoad();
	drawTrain();
	drawTrainLights();

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, -0.5f});
	drawTrainBar();
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, CELLSIZE / 2, 0});
	drawTrainBar();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();

	drawGold();
	drawCompleteLantern(1, 0);
	for (Position pos : lightsPos)
	{
		drawLantern(pos.x, pos.y);
	}
	drawTrainStation(config.origin.x, config.origin.y, M_PI);
	drawLever(-1, 2);
	myEngine.switchToFlatShading();
}

Orientation defineCurveDir(Position prev, Position current, Position next)
{
	if ((next.y > current.y && prev.x < current.x) || (next.x < current.x && prev.y > current.y))
	{
		return BottomRight;
	}
	if ((next.x > current.x && prev.y > current.y) || (next.y > current.y && prev.x > current.x))
	{
		return BottomLeft;
	}
	if ((next.x < current.x && prev.y < current.y) || (next.y < current.y && prev.x < current.x))
	{
		return TopRight;
	}
	if ((next.y < current.y && prev.x > current.x) || (next.x > current.x && prev.y < current.y))
	{
		return TopLeft;
	}
}
