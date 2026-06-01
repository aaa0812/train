#include "custom_shapes.hpp"

GLBI_Convex_2D_Shape trapezoid{3};
IndexedMesh *cylinder;
GLBI_Convex_2D_Shape disk;
GLBI_Engine myEngine;

void initShapes()
{
    std::vector<float> disk_origin{};
    float r = 1.f;
    for (int i = 0; i <= 100; i++)
    {
        disk_origin.push_back(r * cos(2 * (M_PI / 100.0f) * i));
        disk_origin.push_back(r * sin(2 * (M_PI / 100.0f) * i));
    }
    disk.initShape(disk_origin);
    disk.changeNature(GL_TRIANGLE_FAN);

    std::vector<float> trapezoidPoints{
        -0.3f,
        -0.3f,
        0.0f,
        -0.5f,
        -0.5f,
        1.0f,
        0.5f,
        -0.5f,
        1.0f,
        0.3f,
        -0.3f,
        0.0f,
    };
    trapezoid.initShape(trapezoidPoints);
    trapezoid.changeNature(GL_TRIANGLE_FAN);

    cylinder = basicCylinder(1.f, 1.f);
    cylinder->createVAO();
}

void drawTrapezoid()
{
    myEngine.setNormalForConvex2DShape({0, -1, 0});
    {
        for (int i = 0; i < 4; i++)
        {
            myEngine.mvMatrixStack.pushMatrix();
            myEngine.mvMatrixStack.addRotation(i * M_PI / 2, STP3D::Vector3D(0, 0, 1));
            myEngine.updateMvMatrix();
            trapezoid.drawShape();
            myEngine.mvMatrixStack.popMatrix();
        }
    }
    myEngine.setNormalForConvex2DShape({0, 0, 0});
}

void drawClosedCylinder(float const lenght, float const width, float const height)
{
    myEngine.setNormalForConvex2DShape({0, 1, 0}); //normals not defined by default for the disk
    myEngine.mvMatrixStack.addTranslation(Vector3D{0, -lenght / 2, 0}); //center the cylinder
    myEngine.mvMatrixStack.pushMatrix();
	{
        //first disk
        myEngine.mvMatrixStack.pushMatrix();
        {
            myEngine.mvMatrixStack.addRotation(M_PI/2, Vector3D(1, 0, 0));
            myEngine.mvMatrixStack.addHomothety(Vector3D(width, height, 0));
            myEngine.updateMvMatrix();
            disk.drawShape();
        }
        myEngine.mvMatrixStack.popMatrix();
        //second disk
        myEngine.mvMatrixStack.pushMatrix();
        {
            myEngine.mvMatrixStack.addRotation(M_PI/2, Vector3D(1, 0, 0));
            myEngine.mvMatrixStack.addTranslation(Vector3D{0, 0, -lenght});
            myEngine.mvMatrixStack.addHomothety(Vector3D(width, height, 0));
            myEngine.updateMvMatrix();
            disk.drawShape();
        }
        myEngine.mvMatrixStack.popMatrix();
		myEngine.mvMatrixStack.addHomothety(Vector3D{width, lenght, height});
		myEngine.updateMvMatrix();
		cylinder->draw();
	}
    myEngine.mvMatrixStack.popMatrix();
}