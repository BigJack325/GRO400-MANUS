#include "pipe.h"

PipeItem::PipeItem(double position,double hauteur) :
    pipe(new QGraphicsPixmapItem(QPixmap(":/image/pipe.png")))
{
    positionObstacle = position;
    pipe->setPos(position,395-hauteur);
    addToGroup(pipe);

    if(collidesWithSapin()){
            emit pipecollided();
    }
}


PipeItem::~PipeItem()
{

}
bool PipeItem::collidesWithSapin()
{
    QList<QGraphicsItem*> collidingItems = pipe->collidingItems();

    foreach(QGraphicsItem * item,collidingItems){
        SapinItem * sapin = dynamic_cast<SapinItem*>(item);
        if(sapin){
            return true;
        }

    }
    return false;
}

