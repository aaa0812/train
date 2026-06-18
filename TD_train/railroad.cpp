#include "railroad.hpp"

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