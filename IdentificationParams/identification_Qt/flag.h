#ifndef FLAG_H
#define FLAG_H

#include <QObject>
#include<QGraphicsItemGroup>
#include<QGraphicsPixmapItem>
#include<QPropertyAnimation>

class FlagItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    explicit FlagItem(double position);
    ~FlagItem();

private:
    double positionDepot;
    QGraphicsPixmapItem * flag;
};

#endif // FLAG_H
