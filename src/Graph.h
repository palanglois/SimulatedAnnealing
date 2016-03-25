#ifndef GRAPH_H
#define GRAPH_H

#include <QLabel>
#include <QApplication>
#include <QImage>
#include <QGridLayout>
#include <QPainter>
#include <vector>
#include <cmath>
#include <string>
#include <float.h>
#include <cmath>
#include "Prony.h"

class Graph : public QLabel
{
  Q_OBJECT
signals:
  void updatePix(QPixmap pix);
public:
  Graph(int numb);
 
  //Updating the Graph with the Prony serie's data
  void updateData(Prony* function, int curIt);
  
  void drawGraph();
 
private:
  int n;
  double rate;
  std::vector<QLabel*> ELabels;
  std::vector<QLabel*> EValues;
  std::vector<QLabel*> tauLabels;
  std::vector<QLabel*> tauValues;
  QLabel* COneLabel;
  QLabel* COneValue;
  QLabel* CTwoLabel;
  QLabel* CTwoValue;
  QGridLayout* layout;
  std::vector<double> iterations;
  std::vector<double> energy;
  QLabel* graph;
};


#endif // GRAPH_H
