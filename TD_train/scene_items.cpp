#include "scene_items.hpp"

void drawLever(int posX, int posY, float angle)
{
    float const leverLength = 7.0;
    myEngine.mvMatrixStack.pushMatrix();
    {
        myEngine.mvMatrixStack.addTranslation({posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, 0});
        myEngine.mvMatrixStack.addRotation(angle, Vector3D(0, 0, 1));
        myEngine.mvMatrixStack.pushMatrix();
        {
            myEngine.mvMatrixStack.addTranslation(Vector3D(0, 0, -1));
            drawClosedCylinder(2.0, 3.5, 3.5);
        }
        myEngine.mvMatrixStack.popMatrix();
        myEngine.mvMatrixStack.pushMatrix();
        {
            myEngine.mvMatrixStack.addRotation(M_PI/2, Vector3D(1, 0, 0.4));
            myEngine.mvMatrixStack.addTranslation(Vector3D(0, leverLength - 1, 0));
            drawClosedCylinder(leverLength, 0.3, 0.3);
        }
        myEngine.mvMatrixStack.popMatrix();
    }
    myEngine.mvMatrixStack.popMatrix();
}

void drawLantern(int posX, int posY, float scale, bool simpleLantern)
{
	const float BaseSize = 2.f;
	const float LightHeight = 2.f;
	const float TopSize = 3.f;
	const float WEIGTH = 0.5f; // base and top weight

	myEngine.setFlatColor(0.3, 0.3, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		if (simpleLantern)
		{
			myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, WEIGTH / 2}); // place the object on the cell center
		}
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

void drawCompleteLantern(int posX, int posY, float angle)
{
	const float WEIGTH = 1.f; // weight
	const int HEIGHT = 15;
	const int BeamLenght = HEIGHT / 2.5;
	const int SupportLenght = HEIGHT / 3;
	myEngine.setFlatColor(0.5, 0.4, 0.3);

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, 0}); // place the object on the cell center
		myEngine.mvMatrixStack.addRotation(angle, Vector3D(0, 0, 1));
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

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{0, BeamLenght * 0.75, HEIGHT - 5});
		drawLantern(0, 0, 0.75, false);
	}
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.popMatrix();
}

void drawCrate(int posX, int posY)
{
	myEngine.setFlatColor(1, 0, 0);

	myEngine.mvMatrixStack.pushMatrix();
	myEngine.mvMatrixStack.addTranslation(STP3D::Vector3D{posX, posY, 5});

	myEngine.mvMatrixStack.pushMatrix();
	{
		myEngine.mvMatrixStack.addHomothety(STP3D::Vector3D{4, 4, 4});
		myEngine.mvMatrixStack.addRotation(M_PI / 4, STP3D::Vector3D{0, 1, 0});
		myEngine.activateTexturing(true);
		crateTexture.attachTexture();
		myEngine.updateMvMatrix();
		cube->draw();
		crateTexture.detachTexture();
		myEngine.activateTexturing(false);
	}
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.popMatrix();
}