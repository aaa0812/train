#include "draw_scene.hpp"

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint
bool flag_anim_rot_scale{false};
bool flag_anim_rot_arm{false};

GridConfig config;

GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape grid{3};

IndexedMesh *sphere;
StandardMesh *cone;
IndexedMesh *cube;

const int CELLSIZE = 10.f;

void initScene(GridConfig const &gridConfig)
{
	myEngine.switchToPhongShading();
	myEngine.setLightPosition({0, 0, 20, 1});
	myEngine.setLightIntensity({400, 300, 100});
	myEngine.switchToPhongShading();

	config = gridConfig;

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

	std::vector<float> trapezoidPoints{
		-0.3,
		-0.3,
		0.0,
		-0.5,
		-0.5,
		1.0,
		0.5,
		-0.5,
		1.0,
		0.3,
		-0.3,
		0.0,
	};

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
					myEngine.updateMvMatrix();
					myEngine.setFlatColor(0.5, 0.5, 0.5);
					ground.drawShape();
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
				myEngine.setFlatColor(0.6, 0.5, 0.0);
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1, 1, LightHeight});
				myEngine.updateMvMatrix();
				drawTrapezoid();
			}
			myEngine.mvMatrixStack.popMatrix();

			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, 0, LightHeight + WEIGTH / 2});
			myEngine.mvMatrixStack.pushMatrix();
			{
				myEngine.setFlatColor(0.3, 0.3, 0.3);
				myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{1.2, 1.2, WEIGTH});
				myEngine.updateMvMatrix();
				cube->draw();
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

void drawScene(double time_ellapsed, bool displayGrid)
{
	drawFrame();
	myEngine.switchToPhongShading();
	drawGround(displayGrid);
	drawRailRoad();
	drawCompleteLantern(1, 0);
	drawLantern(1, -3);
	drawLantern(-3, -3);
	drawLever(-1, 2);
	drawTrain(0, 0, 0);
	myEngine.switchToFlatShading();
}
