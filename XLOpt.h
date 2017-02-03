#ifndef XLOPT_H
#define XLOPT_H

#include <ctime>

//for testing
extern "C" double sumC( double, double);

//**************** Mathematical Utilities ********************
extern "C" {
//Standard Normal Density Function 
double SNormDens(double); 

//Standard Normal Distribution Function
double SNormDist(double); 
};
//************************************************************

//****************** Time Utilities **************************
extern "C" {
//time_t from string in format dd/mm/yy
time_t ConvertDate(char*);

//string from time_t
void ConvToStr(time_t, char*);

//calculate tau variable from start and end time_t
double CalcTau(time_t, time_t);

//calculate Unix date from Excel Date
time_t UnixFromXL(long int);

//calculate Excel date from Unix date
long int XLFromUnix(time_t);
};
//*************************************************************

//************* Black-Scholes-Merton functions ****************
extern "C" {
//d1 and d2 variables used in interim calculations		
double DeeOne(double,double, double, double, 
		double, double);

double DeeTwo(double,double, double, double, 
		double, double);		

//option price		
double BSMPrice(int, double, double, double, double, double, double);

//option delta
double BSMDelta(int, double, double, double, double, double, double);

//option gamma
double BSMGamma(double, double, double, double, double, double);

//option vega
double BSMVega(double, double, double, double, double, double);

//option theta
double BSMTheta(int, double, double, double, double, double, double);

//option rho
double BSMRho(int, double, double, double, double, double, double);
};

//****************************************************************
#endif		