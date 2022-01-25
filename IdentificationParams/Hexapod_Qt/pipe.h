#ifndef PIPE_H
#define PIPE_H

#include <QObject>
#include<QGraphicsItemGroup>
#include<QGraphicsPixmapItem>
#include<QPropertyAnimation>
#include <QJsonObject>
#include <QJsonDocument>
#include"sapin.h"

class PipeItem :public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

public:
    explicit PipeItem(double position,double hauteur);
    ~PipeItem();
signals:
    void pipecollided();

private:
    bool collidesWithSapin();
    double positionObstacle;
    QGraphicsPixmapItem * pipe;
};

#endif // PIPE_H
