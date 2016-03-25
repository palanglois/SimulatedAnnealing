#ifndef PRONY_H
#define PRONY_H

#include <vector>
#include <math.h>
#include <iostream>
#include <cassert>

using namespace std;

//parameters : (E0,..,E(n-1),Tau1,..,Tau(n-1),C1,C2)

class Prony
{
public:
  Prony();
  Prony(vector< double > expTemp, vector< double > expTime, vector< vector< double > > expData, double T0User, int nuser);
  void print();
  double alphaT(double T);
  double evalProny(double time, double temperature);
  double evaluate();
  void setParameters(vector<double> parameters);
  vector<double> getParameters();
  vector<double> getLimit();
  vector<double> getVariance();
  vector<vector<double> > getExperimentalData();
  vector<double> getTemperature();
  vector<double> getTime();
  double getTO();
  int getN();
  double* getVectorE();
private:
  vector<double> temperature;
  vector<double> time;
  vector<vector<double> > experimentalData;
  int n; //truncature
  double C1, C2, T0, Emax;
  vector<double> vectorE;
  vector<double> vectorTau;
  vector<double> maxLim;
  vector<double> variance;
};

#endif
