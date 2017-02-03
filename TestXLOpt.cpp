#include <cmath>
#include <iostream>
#include <ctime>
#include <cstring>
#include <string>
#include "XLOpt.h"

using namespace std;

int main() {
int CallPut = 1;
double StockPrice = 2100;
double Strike = 2175;
double Vol = 0.12;
long int Eval = 42760;
long int Mat = 42846;
double RiskFree = 0.0075;
double Div = 0.022;
time_t date1 = UnixFromXL(Eval);
time_t date2 = UnixFromXL(Mat);
double tau = CalcTau(date1, date2);
double a = 3.9;
double b = 1.25;

cout << "Tau" << '\t' << tau << endl;

cout << "d1" << '\t' << DeeOne(StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "d2" << '\t' << DeeTwo(StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Price" << '\t' << BSMPrice(CallPut, StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Delta" << '\t' << BSMDelta(CallPut, StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Vega" << '\t' << BSMVega(StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Gamma" << '\t' << BSMGamma(StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Theta" << '\t' << BSMTheta(CallPut, StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Rho" << '\t' << BSMRho(CallPut, StockPrice, Strike, tau, Vol, 
										RiskFree, Div) << endl;
cout << "Addition test" << '\t' << sumC(a, b) << endl;										

return 0;
}
