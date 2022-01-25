#ifndef PENDULE_H
#define PENDULE_H


#include <QObject>
#include<QGraphicsItemGroup>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPropertyAnimation>
#include<QtDebug>
#include<QtMath>

class PenduleItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT


public:
    explicit PenduleItem(double anglePendule,double currentpos);
    ~PenduleItem();


signals:

public slots:

private:
    QGraphicsPixmapItem * pendule;

};

#endif // PENDULE_H
