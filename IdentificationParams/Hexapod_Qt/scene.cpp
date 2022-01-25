#include "scene.h"
#include "car.h"
#include "pipe.h"
#include "flag.h"

Scene::Scene(QObject *parent): QGraphicsScene(parent)
{
    //Set Obstacle
    PipeItem * pipe = new PipeItem(positionObstacle);
    addItem(pipe);

    //Set Flag
    FlagItem * flag = new FlagItem(positionDepot);
    addItem(flag);

}

