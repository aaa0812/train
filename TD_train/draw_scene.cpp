#include "draw_scene.hpp"

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint
bool flag_anim_rot_scale{false};
bool flag_anim_rot_arm{false};

GridConfig config;

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};
GLBI_Convex_2D_Shape grid{3};

IndexedMesh *sphere;
StandardMesh *cone;
IndexedMesh *cube;
IndexedMesh *cylinder;
GLBI_Convex_2D_Shape disk;

void initScene(GridConfig const &gridConfig)
{
	config = gridConfig;
	float r = 1.f;
	std::vector<float> points{0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0};
	std::vector<float> colors{0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

	std::vector<float> disk_origin{};

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

	std::vector<float> trapezoidPoints{};

	for (int i = 0; i <= 100; i++)
	{
		disk_origin.push_back(r * cos(2 * (M_PI / 100.0f) * i));
		disk_origin.push_back(r * sin(2 * (M_PI / 100.0f) * i));
	}
	disk.initShape(disk_origin);

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

	cylinder = basicCylinder(1.f, 1.f);
	cylinder->createVAO();
}

void drawGround(bool displayGrid)
{
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(10);
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
	myEngine.updateMvMatrix();
}

void drawOneRail()
{
	const float SR = 1.f; // rail weight and heigth
	const int LENGTH = 10;
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
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{LENGTH / 2, 0, 0});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{LENGTH, RR, RR});
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});

		myEngine.updateMvMatrix();
		cylinder->draw();
	}
	myEngine.mvMatrixStack.popMatrix();
}

void drawCompleteRail(int posX, int posY, float rotation)
{
	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{5, 5, 0}); // center rail on grid cell
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{10 * posX, 10 * posY, 0});
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
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{10 * posX, 10 * posY, 0});
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
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, -M_PI / 2, STP3D::Vector3D{-10, 0, 0});
				break;
			case BottomLeft:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, M_PI, STP3D::Vector3D{-10, -10, 0});
				break;
			case TopRight:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, 0);
				break;
			case TopLeft:
				drawCompleteCurvedRail(config.path[i].x, config.path[i].y, M_PI / 2, STP3D::Vector3D{0, -10, 0});
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

void drawTrain()
{
	const float SR = 4.f; // rail weight and heigth
	const float RR = 1.f;
	const int LENGTH = 7;
	myEngine.setFlatColor(1.f, 0.f, 0.f);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{5, 5, 5});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{SR, LENGTH, SR});
		myEngine.updateMvMatrix();
		cube->draw();
	}

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.setFlatColor(0.2f, 0.2f, 0.2f);
		myEngine.mvMatrixStack.addRotation(M_PI / 2, STP3D::Vector3D{0, 0, 1});
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, -RR / 4, -SR / 8 - RR / 8});
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{RR / 16, SR / 8, RR / 8});

		myEngine.updateMvMatrix();
		cylinder->draw();
	}

	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}


void drawScene(double time_ellapsed, bool displayGrid)
{
	drawFrame();
	drawGround(displayGrid);
	drawRailRoad();
	drawTrain();
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

