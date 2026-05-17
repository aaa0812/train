#include "draw_scene.hpp"

/// Camera parameters
float angle_theta{45.0}; // Angle between x axis and viewpoint
float angle_phy{30.0};	 // Angle between z axis and viewpoint
float dist_zoom{30.0};	 // Distance between origin and viewpoint
bool flag_anim_rot_scale{false};
bool flag_anim_rot_arm{false};

GLBI_Engine myEngine;
GLBI_Set_Of_Points somePoints(3);
GLBI_Convex_2D_Shape ground{3};

IndexedMesh *sphere;
StandardMesh *cone;
GLBI_Convex_2D_Shape disk;

void initScene()
{
	float r = 1.f;
	std::vector<float> points{0.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0};
	std::vector<float> colors{0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0};

	std::vector<float> disk_origin{};

	somePoints.initSet(points, colors);

	std::vector<float> square{-1.0, -1.0, 0.0,
							  1.0, -1.0, 0.0,
							  1.0, 1.0, 0.0,
							  -1.0, 1.0, 0.0};

	for (int i = 0; i <= 100; i++)
	{
		disk_origin.push_back(r * cos(2 * (M_PI / 100.0f) * i));
		disk_origin.push_back(r * sin(2 * (M_PI / 100.0f) * i));
	}
	disk.initShape(disk_origin);

	ground.initShape(square);
	ground.changeNature(GL_TRIANGLE_FAN);

	sphere = basicSphere(1.0);
	sphere->createVAO();

	cone = basicCone(1.0, 1.0);
	cone->createVAO();
}

void drawGround()
{
	myEngine.setFlatColor(0.5, 0.5, 0.5);
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(10);
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{-5, -5, 0});
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				myEngine.mvMatrixStack.pushMatrix();
				{
					myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{i, j, 0});
					myEngine.updateMvMatrix();
					ground.drawShape();
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

void drawBase()
{
	myEngine.setFlatColor(235.f / 255.f, 207.f / 255.f, 52.f / 255.f);
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addHomothety(6.0);
			myEngine.updateMvMatrix();
			disk.changeNature(GL_TRIANGLE_FAN);
			disk.drawShape();
		}
		myEngine.mvMatrixStack.popMatrix();

		myEngine.mvMatrixStack.addRotation(M_PI / 180 * 90, STP3D::Vector3D(1.0, 0.0, 0.0));
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(4.0, 10.0, 4.0));
		myEngine.updateMvMatrix();
		cone->draw();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawArm()
{
	myEngine.setFlatColor(245.f / 255.f, 164.f / 255.f, 66.f / 255.f);
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.pushMatrix();
		{
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(1.6));
			myEngine.updateMvMatrix();
			sphere->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
		myEngine.updateMvMatrix();

		myEngine.mvMatrixStack.pushMatrix();
		{
			// first cone
			myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D(1.0, 10.0, 1.0));
			myEngine.updateMvMatrix();
			cone->draw();
			// second cone
			myEngine.mvMatrixStack.addRotation(M_PI, STP3D::Vector3D(0.0, 0.0, 1.0));
			myEngine.updateMvMatrix();
			cone->draw();
		}
		myEngine.mvMatrixStack.popMatrix();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
}

void drawScene(double time_ellapsed)
{
	// glPointSize(10.0);

	drawGround();
	// drawFrame();
	/* drawBase();
	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(0.0, 0.0, 10.0));
		myEngine.updateMvMatrix();
		drawArm();
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(0.0, 10.0, 5.0));
		myEngine.updateMvMatrix();
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D(0.0, -20.0, 0.0));
		myEngine.updateMvMatrix();
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();

	drawSphere(time_ellapsed);

	myEngine.setFlatColor(0.5, 0.5, 0.5); */
}
