#include "train_items.hpp"

void drawTrain(int posX, int posY, float angle)
{
    myEngine.mvMatrixStack.pushMatrix();
    {
        myEngine.mvMatrixStack.addTranslation(Vector3D(posX * CELLSIZE + CELLSIZE / 2, posY * CELLSIZE + CELLSIZE / 2, 2.f));
        myEngine.mvMatrixStack.addRotation(angle, Vector3D(0, 0, 1));
        myEngine.mvMatrixStack.addHomothety(Vector3D(3.f, 5.f, 2.f));
        myEngine.updateMvMatrix();
        baseCube->draw();
    }
    myEngine.mvMatrixStack.popMatrix();
}