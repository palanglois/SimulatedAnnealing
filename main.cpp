#include <QApplication>
#include <iostream>
#include "testFuncOpt.h"
#include "funcOpt.h"
#include <vector>
#include "Prony.h"
#include "Graph.h"
#include "AlgoThread.h"
#include "DispThread.h"
#include <QMutex>

using namespace std;

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  //Initialization of the random generator
  srand(1);
  
  //Test Data for the Prony serie Optimization
  vector<double> expTemp ;
  expTemp = {-20,-15,-10};
  vector<double> expTime ;
  expTime = {10,20,40};
  vector < vector < double > > expData;
  expData={{19689,19646,19560},{18375,18030,17771},{16750,16005,14764}};
  Prony* serie = new Prony(expTemp,expTime,expData, 20, 6);
  
  //Initialization of the GUI
  Graph* graph = new Graph(serie->getN());
  graph->show();
  
  //Beginning of the two threads : AlgoThread is the computation thread, DispThread is the display thread
  QMutex* mutex = new QMutex();
  int* curIt = new int(0); //current iteration
  bool* stopSignal = new bool(false);
  AlgoThread* algo = new AlgoThread(mutex, serie, curIt, stopSignal,4000000,100000);
  DispThread* disp = new DispThread(mutex, serie, curIt, stopSignal, graph);
  disp->start();
  algo->start();
  
  return app.exec();
}
