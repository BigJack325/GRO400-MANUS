#ifndef THREAD_H
#define THREAD_H

#include <QThread>

class Thread: public QThread
{
public:
    Thread(int temps,QObject *parent = nullptr);
signals:
    void tempspasse();
protected:
    void run() override;
private:
    int millisecondes;
};

#endif // THREAD_H
