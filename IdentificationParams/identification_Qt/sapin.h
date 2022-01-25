#ifndef SAPIN_H
#define SAPIN_H


#include <QObject>
#include<QGraphicsItemGroup>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPropertyAnimation>
#include<QtDebug>
#include<QtMath>

class SapinItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal y READ getY WRITE setY)


public:
    explicit SapinItem(double angleSapin,double currentpos,double sapinLacher,double etat);
    ~SapinItem();
    qreal getY();


signals:


public slots:
     void setY(qreal y);

private:
    qreal m_y;
    qreal groundposition;
    qreal go_anim;

    QGraphicsPixmapItem * pendule;
    QGraphicsPixmapItem * sapin;
    QPropertyAnimation * yAnimation;

};
#endif // SAPIN_H
