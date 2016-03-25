#ifndef FUNCOPT_H_INCLUDED
#define FUNCOPT_H_INCLUDED
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <cstdlib>
#include "truncated_normal.hpp"
using namespace std;

//Random number generator (between 0 and 1)
double getRand()
{
    return ((double) (rand()%10000))/10000;
}

//Function optimizer class which implements the simulated annealing
template<typename typeFuncMin>
class funcOpt
{
    private:
        // Choice of the moving law, see function moveFunc
        int paramMove;
        // Parameters for the optimization: variance of the laws and bounds
        vector<double> parametersVar,valueMax,valueMin;
    public:
        // Template types witch representents the function to minimize
        typeFuncMin* instanceFuncMin;

        funcOpt(typeFuncMin* itemFunc,int newParamMove,const vector<double> newParametersVar,const  vector<double> newValueMax, const vector<double> newValueMin );
        // Function that explores the space
        void moveFunc(double beta);
};

template<typename typeFuncMin>
funcOpt<typeFuncMin>::funcOpt(typeFuncMin* itemFunc,int newParamMove, const vector<double> newParametersVar,const  vector<double> newValueMax,const vector<double> newValueMin )
{
    instanceFuncMin=itemFunc;
    paramMove=newParamMove;
    // Parameters required for the function's laws (for example the variance)
    parametersVar=newParametersVar;

    // Parameters' bounds. We get a new parameter if we get out of the bounds.
    valueMax=newValueMax;
    valueMin=newValueMin;
}


template<typename typeFuncMin>
void funcOpt<typeFuncMin>::moveFunc(double beta)
{
  // Get the parameters from the past

  vector<double> paramPresent= instanceFuncMin->getParameters();
  int sizeParam = paramPresent.size();
  vector<double> paramFuture(sizeParam);
  assert(sizeParam<=parametersVar.size());
  assert(sizeParam<=valueMin.size());
  assert(sizeParam<=valueMax.size());


  // double energyPresent=0,energyFuture=0;
  int nbTau;
  double evalPresent=0,evalFuture=0;
  double randValue,tailleSeg,segSup,segInf;
  //energyPresent=instanceFuncMin.energy();
    evalPresent = instanceFuncMin->evaluate();

  // Choice of the law to use

  switch(paramMove)
  {
  // If we want more laws, we can add it here
      case 0:
	  // Uniform law, centered on the value
	  for(int i=0;i<sizeParam;i++)
	  {
	      randValue=getRand();
	      // Segment's length
	      segSup=min(12*sqrt(parametersVar[i]),valueMax[i]-paramPresent[i]);
	      segInf=min(12*sqrt(parametersVar[i]),paramPresent[i]-valueMin[i]);
	      tailleSeg=segSup+segInf;
	      paramFuture[i]=paramPresent[i]+(segSup-segInf)/2.+(randValue-1./2.)*tailleSeg;
	      // If we are out of bounds, we draw again
	      while (valueMin[i]>paramFuture[i] || valueMax[i]<paramFuture[i])
	      {
		  randValue=getRand();
		  paramFuture[i]=paramPresent[i]+(segSup-segInf)/2.+(randValue-1./2.)*tailleSeg;
	      }
	      assert(valueMax[i]>=paramFuture[i]);
	      assert(paramPresent[i]>=valueMin[i]);

	  }
	  break;
      case 1:
	  // Truncated normal law on the domain 
	  for(int i=0;i<sizeParam;i++)
	  {
	      assert(valueMax[i]>=paramPresent[i]);
	      randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[i]),valueMin[i]-paramPresent[i],valueMax[i]-paramPresent[i]);
	      paramFuture[i]=paramPresent[i]+randValue;
	      //assert(valueMax[i]>=paramFuture[i]);
	      //assert(paramPresent[i]>=valueMin[i]);
	  }
	  break;
      case 2:
	  // Normal law, with condition on the terms order. Not finished yet
	  nbTau=paramPresent.size()/2 -1.;

	  //Drawing of the sorted Young modulus
	  randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[0]),valueMin[0]-paramPresent[0],valueMax[0]-paramPresent[0]);
	  paramFuture[0]=paramPresent[0]+randValue;
	  for(int i=1;i<nbTau;i++)
	  {
	      assert(valueMin[i]-paramPresent[i]<=min(valueMax[i],paramFuture[i-1])-paramPresent[i]);

	      randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[i]),valueMin[i]-paramPresent[i],min(valueMax[i],paramFuture[i-1])-paramPresent[i]);
	      //cout<< "La valeur aléatoire entre "<< valueMin[i]-paramPresent[i] <<" et " <<min(valueMin[i],paramFuture[i-1])-paramPresent[i]<< " vaut: "<< randValue<<endl;
	      paramFuture[i]=paramPresent[i]+randValue;
		if(paramFuture[i]>paramFuture[i-1] && paramFuture[i]>0.000001 )
	      {
		  cout<< "Les nombres "<<paramFuture[i] << " et "<<paramFuture[i-1]<< " sont trop proche, on effectue un swap"<< endl;
		  cout<< abs(paramFuture[i]-paramFuture[i-1])<<endl;
		  assert(abs(paramFuture[i]-paramFuture[i-1])<0.00001);
		  swap(paramFuture[i],paramFuture[i-1]);
	      }
	      if(paramFuture[i]>0.0001)
	      {
		  assert(paramFuture[i]>=valueMin[i]);
		  assert(paramFuture[i]<=valueMax[i]);
		  assert(paramFuture[i]<=paramFuture[i-1]);
	      }
	  }

	  // Drawing of the sorted tau
	  randValue=truncated_normal_ab_cdf_inv(getRand(),nbTau,sqrt(parametersVar[nbTau]),valueMin[nbTau]-paramPresent[nbTau],valueMax[nbTau]-paramPresent[nbTau]);
	  paramFuture[nbTau]=paramPresent[nbTau]+randValue;

	  for(int i=nbTau+1;i<2*nbTau;i++)
	  {

	      randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[i]),valueMin[i]-paramPresent[i],min(valueMax[i],paramFuture[i-1])-paramPresent[i]);

	      paramFuture[i]=paramPresent[i]+randValue;
	      if(paramFuture[i]>paramFuture[i-1] && paramFuture[i]>0.000001)
	      {
		  cout<< "Les nombres "<<paramFuture[i] << " et "<<paramFuture[i-1]<< " sont trop proche, on effectue un swap"<< endl;
		  assert(abs(paramFuture[i]-paramFuture[i-1])<0.001);
		  swap(paramFuture[i],paramFuture[i-1]);
	      }
	      if(paramFuture[i]>0.000001)
	      {
		  assert(paramFuture[i]>=valueMin[i]);
		  assert(paramFuture[i]<=valueMax[i]);
		  assert(paramFuture[i]<=paramFuture[i-1]);
	      }
	  }
	  // Drawing of C1 and C2
	  randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[2*nbTau]),valueMin[2*nbTau]-paramPresent[2*nbTau],valueMax[2*nbTau]-paramPresent[2*nbTau]);
	  paramFuture[2*nbTau]=paramPresent[2*nbTau]+randValue;
	  randValue=truncated_normal_ab_cdf_inv(getRand(),0,sqrt(parametersVar[2*nbTau+1]),valueMin[2*nbTau+1]-paramPresent[2*nbTau+1],valueMax[2*nbTau+1]-paramPresent[2*nbTau+1]);
	  paramFuture[2*nbTau+1]=paramPresent[2*nbTau+1]+randValue;

  }

  instanceFuncMin->setParameters(paramFuture);
  //energyFuture = instanceFuncMin.energy();
  evalFuture= instanceFuncMin->evaluate();

  // Comparison between future and present, and choice with Hastings Metropolis
  //assert(energyPresent!=0);
  if(evalFuture<evalPresent)
  {
    return;
  }
  else
  {
    randValue=getRand();
    if( exp(-beta*(evalFuture-evalPresent)) > randValue )
    {
      return;
    }
    else
    {
      instanceFuncMin->setParameters(paramPresent);
    }
  }
}
#endif // FUNCOPT_H_INCLUDED
