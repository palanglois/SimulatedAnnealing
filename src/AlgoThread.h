#ifndef ALGOTHREAD_H
#define ALGOTHREAD_H

#include <QThread>
#include "funcOpt.h"
#include <vector>
#include "Prony.h"
#include "Graph.h"
#include <QMutex>

class AlgoThread : public QThread
{
public:
  AlgoThread(QMutex* mut, Prony* ser,int* ci, bool* stopSig, int maxIt, int refreshIt) : mutex(mut), 
  serie(ser), maxIteration(maxIt), refreshIteration(refreshIt), curIt(ci), stopSignal(stopSig)
  {
    
  }
protected:
  void run()
  {
    double temp;
    bestEnergy = DBL_MAX;
    //Setting minimal value of the parameters of the Prony serie to 0
    vector<double> valueMin;
    for(int i=0;i<serie->getLimit().size();i++)
        valueMin.push_back(0.);
    //Creating a Prony serie optimizer
    funcOpt<Prony> optimizer(serie, 0, serie->getVariance(),serie->getLimit(),valueMin );
    for(int i = 0; i<maxIteration;i++) //Loop of the algorithm's iterations
    {
	mutex->lock();
        optimizer.moveFunc(300*pow(i,0.23));
        /*if(i%refreshIteration==0)
        {
            optimizer.instanceFuncMin->print();
        }*/
        (*curIt) = i;
	temp = optimizer.instanceFuncMin->evaluate();
	if (temp < bestEnergy)
	{
	  bestEnergy = temp;
	  std::cout << "New Energy : " << temp << std::endl;
	  bestParam = optimizer.instanceFuncMin->getParameters();
	}
	mutex->unlock();
    }
    mutex->lock();
    optimizer.instanceFuncMin->setParameters(bestParam);
    *stopSignal = true;
    mutex->unlock();
    exit();
  }
private:
  QMutex* mutex;
  Prony* serie;
  int maxIteration;
  int refreshIteration;
  int* curIt;
  bool* stopSignal;
  vector<double> bestParam;
  double bestEnergy;
};

#endif
