#ifndef DISPTHREAD_H
#define DISPTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTime>
#include <QCoreApplication>
#include "Graph.h"
#include "Prony.h"

class DispThread : public QThread
{
public:
  DispThread(QMutex* mut, Prony* ser, int* ci, bool* stopSig, Graph* gr) : mutex(mut), 
  serie(ser), curIt(ci), graph(gr), stopSignal(stopSig)
  {
    
  }
protected:
  void run()
  {
    bool run = true;
    while(run)
    {
      //Update the graph after acquiring the lock
      mutex->lock();
      graph->updateData(serie, *curIt);
      run = !(*stopSignal); //We check if the algorithm is over
      mutex->unlock();
      
      //Wait 100 millisecond
      QTime dieTime= QTime::currentTime().addMSecs(100);
      while( QTime::currentTime() < dieTime )
	QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    exit();
  }
private:
  QMutex* mutex;
  Prony* serie;
  int* curIt;
  Graph* graph;
  bool* stopSignal;
};

#endif
