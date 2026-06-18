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
	lightsPos.emplace_back(Position(2, -2));
	lightsPos.emplace_back(Position(-3, 2));
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

	loadTextures();
}

void drawGround(bool displayGrid)
{
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
}

void drawFrame()
{
	somePoints.changeNature(GL_LINES);
	somePoints.drawSet();
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
	drawCompleteLantern(-2, -4, -M_PI / 2);
	for (Position pos : lightsPos)
	{
		drawLantern(pos.x, pos.y);
	}
	drawTrainStation(config.origin.x, config.origin.y, M_PI);

	drawLever(-1, 2);
	myEngine.switchToFlatShading();
}
