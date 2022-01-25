#ifndef CAR_H
#define CAR_H

#include <QObject>
#include<QGraphicsItemGroup>
#include<QGraphicsPixmapItem>
#include<QGraphicsScene>
#include<QPropertyAnimation>

class CarItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT


public:
    explicit CarItem(double currentpos);
    ~CarItem();


signals:

public slots:

private:

    QGraphicsPixmapItem * vehicule;


};

#endif // CAR_H
