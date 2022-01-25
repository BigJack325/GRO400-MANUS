#include "car.h"
#include <QDebug>

CarItem::CarItem(double currentpos):
  vehicule(new QGraphicsPixmapItem(QPixmap(":/image/car.png")))

{
    vehicule->setPos(currentpos,270);
    addToGroup(vehicule);

}

CarItem::~CarItem()//tester si efface
{
    //qDebug()<< "Mario est mort";

}
