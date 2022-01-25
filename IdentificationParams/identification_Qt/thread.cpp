#include "thread.h"

Thread::Thread(int temps,QObject *parent):
    QThread(parent)
{
    millisecondes = temps;

}
void Thread::run()
{
    while(true){
        QThread::msleep(millisecondes);
        emit tempspasse();
    }

}
