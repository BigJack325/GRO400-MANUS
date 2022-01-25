#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "car.h"
#include "pipe.h"
#include "flag.h"

class Scene:public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
signals:

public slots:
};

#endif // SCENE_H
