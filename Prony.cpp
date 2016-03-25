#include "Prony.h"

Prony::Prony()
{

}

Prony::Prony(vector< double > expTemp, vector< double > expTime, vector< vector< double > > expData, double T0User, int nuser) : T0(T0User), n(nuser)
{
  //initialization of the temperature of experiment
  for(int i=0;i<expTemp.size();i++)
  {
    temperature.push_back(expTemp[i]);
  }
  //initialization  of the time (frequence) of experiment
  for(int i=0;i<expTime.size();i++)
  {
    time.push_back(expTime[i]);
  }
  vector<double> emptyTable(0);
  //initialization of the corresponding experimental data
  for(int i=0;i<expData.size();i++)
  {
    experimentalData.push_back(emptyTable);
    for(int j=0;j<expData[0].size();j++)
    {
      experimentalData[i].push_back(expData[i][j]);
    }
  }
  //initialization of parameters C1 and C2 and Emax
  C1=20;
  C2=400;
  Emax = 0;
  for(int i=0;i<expData.size();i++)
  {
    for(int j=0;j<expData[0].size();j++)
    {
      if (expData[i][j] > Emax)
            Emax = expData[i][j];
    }
  }
  for(int i=0;i<n;i++)
  {
    //initialization of the tau vector for the Prony Serie
    vectorTau.push_back(0.02*pow(10,i));
    //initialization of the E vector for the Prony Serie
    if(i==0)
    {
      vectorE.push_back(Emax);
    }
    else
    {
      vectorE.push_back(Emax/10.0);
    }
    //Initialization of the limits for E parameters
    maxLim.push_back(Emax);
    //Initialization of the variances for the E parameters
    variance.push_back(Emax/100);
  }
  for(int i=1;i<n;i++)
  {
      //Initialization of the limits for Tau parameters
      maxLim.push_back(0.04*pow(10,i));
      //Initialization of the variances for the Tau parameters
      variance.push_back(0.04*pow(10,i-2));
  }
  //Lim and Var for C1
  maxLim.push_back(30);
  variance.push_back(0.1);
  //Lim and Var for C2
  maxLim.push_back(500);
  variance.push_back(2);
}

//Display function for the state of the Prony class
void Prony::print()
{
  cout << "Avaible temperatures : " << endl;
  for(int i=0;i<temperature.size();i++)
  {
    cout << temperature[i] << " ; " ;
  }
  cout << endl << endl;

  cout << "Avaible times : " << endl;
  for(int i=0;i<time.size();i++)
  {
    cout << time[i] << " ; " ;
  }
  cout << endl << endl;

  cout << "Availble experimental data : " << endl;
  for(int i=0;i<experimentalData.size();i++)
  {
    for(int j=0;j<experimentalData[0].size();j++)
    {
      cout << "Temperature : " << temperature[i] << " ; Time : " << time[j] << " ; Young Modulus : " << experimentalData[i][j] << endl;
    }
  }
  cout << endl;

  cout << "Number of Prony terms :  " << n << endl << endl;

  cout << "WLF constants : " << endl;
  cout << "T0 : " << T0 << " ; C1 : " << C1 << " ; C2 : " << C2 << endl << endl;

  cout << "Prony linear Coefficients Ei : " << endl;
  for(int i=0;i<vectorE.size();i++)
  {
      cout << "E" << i << " = " << vectorE[i] << " ; " ;
  }
  cout << endl << endl;

  cout << "Prony non linear Coefficients Taui : " << endl;
  for(int i=1;i<vectorTau.size();i++)
  {
      cout << "Tau" << i << " = " << vectorTau[i] << " ; " ;
  }
  cout << endl << endl;

  cout << "Current Energy : " << this->evaluate() << endl << endl;
}

//Return the alphaT value (see the Williams–Landel–Ferry equation)
double Prony::alphaT(double T)
{
    return (exp(-C1*(T-T0)/(C2+T-T0)));
}

//Return the value of the Prony serie for a given time and temperature
double Prony::evalProny(double time, double temperature)
{
    double aT = alphaT(temperature);
    double total = vectorE[0];
    for(int i = 1;i<vectorE.size();i++)
    {
        total += vectorE[i] * exp(-(time)/(aT*vectorTau[i]));
    }
    return total;
}

//Return the Energy to minimize (distance between the Prony Serie and the experimental data)
double Prony::evaluate()
{
    double total = 0.00001;
    double eD, pD = 0; // Experimental Data, Prony Data
    for(int i=0;i<experimentalData.size();i++)
    {
        for(int j=0;j<experimentalData[0].size();j++)
        {
            eD = experimentalData[i][j];
            pD = evalProny(time[j],temperature[i]);
            total += ((eD - pD)*(eD-pD))/(eD*eD);
        }
    }
    return total;
}

//Set new parameters to the model
//parameters : (E0,..,E(n-1),Tau1,..,Tau(n-1),C1,C2)
void Prony::setParameters(vector<double> parameters)
{
    assert(parameters.size()==2*n+1);
    for(int i=0;i<n;i++)
    {
        vectorE[i] = parameters[i];
    }
    for(int i=1;i<n;i++)
    {
        vectorTau[i] = parameters[i+n-1];
    }
    C1 = parameters[2*n-1];
    C2 = parameters[2*n];
}

//Get the current parameters of the model
//parameters : (E0,..,E(n-1),Tau1,..,Tau(n-1),C1,C2)
vector<double> Prony::getParameters()
{
    vector<double> param;
    for(int i=0;i<n;i++)
    {
        param.push_back(vectorE[i]);
    }
    for(int i=1;i<n;i++)
    {
        param.push_back(vectorTau[i]);
    }
    param.push_back(C1);
    param.push_back(C2);
    return param;
}

vector<double> Prony::getLimit()
{
  return maxLim;
}

vector<double> Prony::getVariance()
{
  return variance;
}

double Prony::getTO()
{
  return T0;
}

vector< vector< double > > Prony::getExperimentalData()
{
  return experimentalData;
}

vector< double > Prony::getTemperature()
{
  return temperature;
}

vector< double > Prony::getTime()
{
  return time;
}

int Prony::getN()
{
  return n;
}

double* Prony::getVectorE()
{
  return &(vectorE[0]);
}
