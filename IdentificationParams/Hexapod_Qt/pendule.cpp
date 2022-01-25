#include "pendule.h"

#include <QDebug>

PenduleItem::PenduleItem(double anglePendule,double currentpos):
  pendule(new QGraphicsPixmapItem(QPixmap(":/image/pendule.png")))

{
    pendule->setPos(currentpos-5,345);

    QPointF offsetPendule = pendule->boundingRect().topRight();
    QTransform transPendule;

    transPendule.translate(offsetPendule .x(), offsetPendule .y());
    transPendule.rotate(anglePendule);
    transPendule.translate(-offsetPendule .x(), -offsetPendule .y());

    pendule->setTransform(transPendule);

    addToGroup(pendule);


}

PenduleItem::~PenduleItem()//tester si efface
{
    //qDebug()<< "Pendule est mort";
}
