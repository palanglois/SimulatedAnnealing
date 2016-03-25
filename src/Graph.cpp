#include "Graph.h"

Graph::Graph(int numb) : n(numb) //n is the number of terms in the Serie
{
  rate = 0.9;
  this->setGeometry(50,50,800,600);
  layout = new QGridLayout(this);
  for(int i=0;i<n;i++)
  {
    //Creations of labels for the E values
    ELabels.push_back(new QLabel(QString(("E"+std::to_string(i)).c_str()),this));
    layout->addWidget(ELabels[i],i,0);
    EValues.push_back(new QLabel(QString(std::to_string(0).c_str()),this));
    layout->addWidget(EValues[i],i,1);
    
    //Creations of labels for the tau values
    if(i!=0)
    {
      tauLabels.push_back(new QLabel(QString(("Tau"+std::to_string(i)).c_str()),this));
      layout->addWidget(tauLabels[i-1],n+i-1,0);
      tauValues.push_back(new QLabel(QString(std::to_string(0).c_str()),this));
      layout->addWidget(tauValues[i-1],n+i-1,1);
    }
  }
    
  //Creations of labels for C1 and C2
  COneLabel = new QLabel("C1",this);
  COneValue = new QLabel("0",this);
  layout->addWidget(COneLabel,2*n-1,0);
  layout->addWidget(COneValue,2*n-1,1);
    
  CTwoLabel = new QLabel("C2",this);
  CTwoValue = new QLabel("0",this);
  layout->addWidget(CTwoLabel,2*n,0);
  layout->addWidget(CTwoValue,2*n,1);
    
  graph = new QLabel();
  QImage graphMap(400,400,QImage::Format_RGB32);
  graphMap.fill(Qt::white);
  graphMap.setPixel(50,50,Qt::black);
  graph->setPixmap(QPixmap::fromImage(graphMap));
  layout->addWidget(graph,1,2,2*n,1);
    
  this->setLayout(layout);
  QObject::connect(this,SIGNAL(updatePix(QPixmap)),graph,SLOT(setPixmap(QPixmap)));
}

//Updating the Graph with the Prony serie's data
void Graph::updateData(Prony* function, int curIt)
{
  //Adding current value of the energy
  iterations.push_back(curIt);
  energy.push_back(log(function->evaluate()));
  //Updating the values labels
  for(int i=0;i<n;i++)
  {
      EValues[i]->setText(QString((std::to_string(function->getParameters()[i])).c_str()));
      if(i!=0)
      {
        tauValues[i-1]->setText(QString((std::to_string(function->getParameters()[i+n-1])).c_str()));
      }
  }
  COneValue->setText(QString((std::to_string(function->getParameters()[2*n-1])).c_str()));
  CTwoValue->setText(QString((std::to_string(function->getParameters()[2*n])).c_str()));
  drawGraph();
  qApp->processEvents();
}

void Graph::drawGraph()
{
  QImage gr(800,800,QImage::Format_RGB32);
  gr.fill(Qt::white);

  double maxX = gr.width() ;
  double maxY = gr.height();

  double maxOrd = -DBL_MAX;
  double minOrd = DBL_MAX;
  double maxAbs = -DBL_MAX;
  double minAbs = DBL_MAX;
    
  for(int i=5;i<iterations.size() && iterations.size() > 5;i++)
  {
    if(iterations[i]>maxAbs)
      maxAbs=iterations[i];
    if(iterations[i]<minAbs)
      minAbs=iterations[i];
    if(energy[i]>maxOrd)
      maxOrd=energy[i];
    if(energy[i]<minOrd)
      minOrd=energy[i];
  }
  int x,y;
  for(int i=5;i<iterations.size() && iterations.size() > 5;i++)
  {
    x = (iterations[i]-minAbs)/(maxAbs-minAbs)*maxX*rate + ((1.0-rate)/2.) * maxX  ;
    y = rate*(maxY - (energy[i]-minOrd)/(maxOrd-minOrd)*maxY) + ((1.0-rate)/2.) * maxY;
    gr.setPixel(x,y,Qt::black);
    gr.setPixel(x,y+1,Qt::black);
    gr.setPixel(x+1,y,Qt::black);
    gr.setPixel(x+1,y+1,Qt::black);
  }
  emit updatePix(QPixmap::fromImage(gr));
}

