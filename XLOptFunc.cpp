#include <cmath>
#include <iostream>
#include <ctime>
#include <cstring>
#include <string>
#include "XLOpt.h"

// ******************* Common constants *******************
#ifndef PI
	#define PI 3.14159265359
#endif

#ifndef YEARSECS
	#define YEARSECS 31557600
#endif

#ifndef UNIXZERO
	#define UNIXZERO 25569
#endif

#ifndef DAYSECS
	#define DAYSECS 86400
#endif
 
//**********************************************************
using namespace std;

//for testing

double sumC( double a, double b) { return a + b; } 

//********************** Maths Functions ********************
double SNormDens(double ZVal) {

	return exp(-pow(ZVal,2.0)/2.0)/sqrt(2.0 * PI);
}

double SNormDist(double ZVal) {
	return 0.5 +0.5* erf(ZVal/sqrt(2.0));
}

//****************************************************************

//*************** Date/Time Utilities *********************************

time_t ConvertToDate(char *Date) {
	time_t rawtime;
	time_t tmp;
	struct tm *TimeStr;
	int Toks[3];
	int i;
	
	for (i = 0; i < 3; ++i) {
		if (i == 0) {
			Toks[i] = stoi(strtok(Date, "/"));
		} else {
			Toks[i] = stoi(strtok(NULL, "/"));
		}
	}
	
	time ( &rawtime );
  	TimeStr = localtime ( &rawtime );
  	TimeStr->tm_year = Toks[2] + 2000 - 1900;
  	TimeStr->tm_mon = Toks[1] - 1;
  	TimeStr->tm_mday = Toks[0];
  	TimeStr->tm_hour = 12;
  	TimeStr->tm_min = 0;
  	TimeStr->tm_sec = 0;
  	
  	tmp = mktime(TimeStr);
  	
  	return tmp;
}

void ConvToStr(time_t iDate, char *DateStr) {
	struct tm *timeDets;
	
	timeDets = localtime(&iDate);
	std::strftime(DateStr, 11,"%d/%m/%y", timeDets);
}

double CalcTau(time_t StartTime, time_t EndTime) {
  	
  	if (EndTime < StartTime) {
  		//cout << "Invalid Evaluation date for Maturity." << endl;
  		return 0;
  	} else {
  		return difftime(EndTime, StartTime)/YEARSECS;
  	}
}

//calculate Unix date from Excel Date
time_t UnixFromXL(long int XLDate) {
	long int tmp;
	tmp = (XLDate - UNIXZERO) * DAYSECS;
	
	return static_cast<time_t>(tmp);
}

//calculate Excel date from Unix date
long int XLFromUnix(time_t UnixTime) {
	long int tmp;
	tmp = static_cast<long int>(UnixTime);
	return UNIXZERO + tmp / DAYSECS;
}

//****************************************************************************
	
//********************* B-S-M functions *****************************
double DeeOne(double StockPrice, double Strike, double tau, double Vol, 
		double RiskFree, double Div) {

	return (log(StockPrice/Strike) + (RiskFree - Div + 
					pow(Vol,2)/2)*tau)/Vol/sqrt(tau);
}

double DeeTwo(double StockPrice, double Strike, double tau, double Vol, 
		double RiskFree, double Div) {

	return (log(StockPrice/Strike) + 
				(RiskFree - Div - pow(Vol,2)/2)*tau)/Vol/sqrt(tau);
}

double BSMPrice(int CallPut, double StockPrice, double Strike, double tau, double Vol, 
				double RiskFree,double Div) {

	double tmp;
	double d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);
	double d2 = DeeTwo(StockPrice, Strike, tau, Vol, RiskFree, Div);
			
	if (CallPut == 1) {
		tmp = StockPrice * exp(-Div * tau) * SNormDist(d1) 
				- Strike * exp(-RiskFree * tau)* SNormDist(d2);
	} else if (CallPut == 0) {
			tmp = Strike * exp(-RiskFree * tau) * SNormDist(-d2)
				- StockPrice * exp(-Div * tau) * SNormDist(-d1);
	} else {
		tmp =0;
	}
	
	return tmp;
}

double BSMDelta(int CallPut, double StockPrice, double Strike, double tau, double Vol, 
				double RiskFree,double Div) {

	double tmp;
	double d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);
	
	
	if (CallPut == 1) {
		tmp = exp(-Div * tau) * SNormDist(d1);
	} else if (CallPut == 0) {
		tmp = exp(-Div * tau) * (SNormDist(d1)-1);
	} else {
		tmp =0;
	}
	
	return tmp;
}


double BSMGamma(double StockPrice, double Strike, double tau, double Vol, 
			double RiskFree,double Div) {

	double d1;
	
	d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);	
	
	return exp(-Div * tau) * SNormDens(d1) / StockPrice / Vol / sqrt(tau);
}

double BSMVega(double StockPrice, double Strike, double tau, double Vol, 
			double RiskFree,double Div) {

	double d1;
	
	d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);	
	
	return StockPrice * sqrt(tau) * SNormDens(d1) * exp(-Div * tau) / 100;
	
}

double BSMTheta(int CallPut, double StockPrice, double Strike, double tau, double Vol, 
			double RiskFree, double Div) {

	int Mult;
	double d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);
	double d2 = DeeTwo(StockPrice, Strike, tau, Vol, RiskFree, Div);
	
	if (CallPut == 1) {
		Mult = 1;
	} else if (CallPut == 0) {
		Mult = -1;
	} else {
		Mult =0;
	}

	return (Mult * Div * StockPrice * exp(-Div * tau) * SNormDist(Mult * d1) 
    	- StockPrice * exp(-Div * tau) * Vol * SNormDens(d1) / 2 / sqrt(tau)
    	- Mult * RiskFree * Strike * exp(-RiskFree * tau) * SNormDist(Mult * d2)) / 365;
    	
}

double BSMRho(int CallPut, double StockPrice, double Strike, double tau, double Vol, 
			double RiskFree, double Div) {

	int Mult;
	double d1;
	double d2;
	

	d1 = DeeOne(StockPrice, Strike, tau, Vol, RiskFree, Div);
	d2 = DeeTwo(StockPrice, Strike, tau, Vol, RiskFree, Div);
	
	if (CallPut == 1) {
		Mult = 1;
	} else if (CallPut == 0) {
		Mult = -1;
	} else {
		Mult =0;
	}

	return Mult * Strike * tau * exp(-RiskFree * tau) * SNormDist(Mult * d2) / 100;
    	
}

//**********************************************************************************
