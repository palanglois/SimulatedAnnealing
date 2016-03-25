#ifndef TESTFUNCOPT_H_INCLUDED
#define TESTFUNCOPT_H_INCLUDED
#include <vector>

//TEST CLASS ONLY USED FOR DEVELOPMENT

using namespace std;

class polynomFunc
{
    private:
        vector<int> degree;
        int dimension;
        vector<vector<double> >  coefficient;
        vector<double> values;

    public:
        polynomFunc();
        polynomFunc(vector<vector<double> > coef);
        void setParameters(vector<double> variable);
        vector<double> getParameters();
        double evaluate();
        double energy();
        void show(int par=0);
};

void testFuncOpt();
#endif // TESTFUNCOPT_H_INCLUDED
