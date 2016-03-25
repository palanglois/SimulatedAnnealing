#include "testFuncOpt.h"
#include "funcOpt.h"
#include <cassert>
#include <cmath>
#include <iostream>

//TEST CLASS ONLY USED FOR DEVELOPMENT

using namespace std;

void testFuncOpt()
{
    int n =10;
    // Creation of the polynomial X^2+ 2*X +1
    vector <vector<double> > polyTest(1);
    vector<double> param;
    polyTest[0].push_back(10);
    polyTest[0].push_back(6);
    polyTest[0].push_back(1./2.);
    polyTest[0].push_back(-4./3.);
    polyTest[0].push_back(1./4.);
    //polyTest[0].push_back(4.);
   // polyTest[0].push_back(1.);
    param.push_back(13.);


    polynomFunc poly(polyTest);
    poly.setParameters(param);
    vector<double> parametersVar(n),parametersMin(n),parametersMax(n);
    // Definition of the parameters to use and their bounds
    for(int i=0;i<n;i++)

    {
        parametersVar[i]=3;
        parametersMin[i]=-1000;
        parametersMax[i]=1000;
    }

    funcOpt<polynomFunc> testOpt(&poly,0,parametersVar,parametersMax,parametersMin);
    // Polynomial optimization
    for(int i = 0; i<400000;i++)
    {
        testOpt.moveFunc(i);
        if(i%10000==0)
        {
            testOpt.instanceFuncMin->show();
            cout<<endl;
        }
    }
}


polynomFunc::polynomFunc()
{}
polynomFunc::polynomFunc(vector<vector<double> > coef)
{
    dimension=coef.size();
    for(int i=0;i<dimension;i++)
    {
        degree.push_back(coef[i].size());
        coefficient.push_back(coef[i]);
    }
}

void polynomFunc::setParameters(vector<double> variable)
{
    assert(variable.size()==dimension);
    values=variable;

}

double polynomFunc::evaluate()
{
    double stock=0;
    for(int i=0;i<dimension;i++)
    {
        for(int j=0;j<degree[i];j++)
        {
            stock+=pow(values[i],j)*coefficient[i][j];
        }
    }
    return stock;
}

double polynomFunc::energy()
{
    return exp(-this->evaluate());
}


vector<double> polynomFunc::getParameters()
{
    return values;
}

void polynomFunc::show(int par )
{
    cout <<" Les variables sont:" <<endl;
    for(int i=0;i<dimension;i++)
    {
        cout<< values[i]<< " ";
    }
    cout<<endl;
    cout << "La valeur est:" << endl;
    cout << this->evaluate()<<endl;
    if(par)
    {
        cout <<" Les coefficients sont:" <<endl;
        for(int i=0;i<dimension;i++)
        {
            for(int j=0;j<degree[i];j++)
            {
                cout<< coefficient[i][j]<< " ";
                cout<<endl;
            }
        }
        cout<<endl;

    }


}


