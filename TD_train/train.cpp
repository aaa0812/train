#include "train.hpp"

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