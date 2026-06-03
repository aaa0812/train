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