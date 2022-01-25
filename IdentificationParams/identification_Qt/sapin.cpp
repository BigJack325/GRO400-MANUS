#include "sapin.h"

SapinItem::SapinItem(double angleSapin,double currentpos,double sapinLacher,double etat):
  sapin(new QGraphicsPixmapItem(QPixmap(":/image/sapin.png")))

{
    sapin->setPos(currentpos+15,365);

    sapin->setTransformOriginPoint(16.5,-28);
    sapin->setRotation(angleSapin);

    groundposition = scenePos().y()+50;
    if(sapinLacher == 0)
    {
        go_anim = 0;

    }

    if(sapinLacher == 1&& go_anim == 0)
    {
    yAnimation = new QPropertyAnimation(this,"y",this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(400);
    yAnimation->setEasingCurve(QEasingCurve::Linear);
    yAnimation->setDuration(100);//compter cmb de temps prend d'aller a une position à l'autre 1100
    connect(yAnimation,&QPropertyAnimation::finished,[=](){

        scene()->removeItem(this);

    });
    yAnimation->start();
    go_anim = 1;
    }

    if(etat != 8)
    {
        addToGroup(sapin);
    }



}

    SapinItem::~SapinItem()//tester si efface
    {
        //qDebug()<< "Pendule est mort";
    }

    qreal SapinItem::getY() {
        return m_y;
    }

    void SapinItem::setY(qreal y)
    {
        //qDebug() << "Sapin position:"<< x;
        moveBy(0,y-m_y);
        m_y = y;
    }


