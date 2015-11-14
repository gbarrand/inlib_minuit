// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <cstdlib>
#include <cmath>

#include <iostream>

#include <inlib/f2cmn>

inline double shoot_random_flat(){
  // Shoot random numbers according a flat distribution.
  double value  = (double)rand();
  value /= (double)RAND_MAX;
  return value;
}

inline double polynomial(double aX,int aParamn,double* aParams){
  // P[0] + P[1] x + P[2] x**2 + ...
  double value = 0.;
  double xxx = 1.;
  for(int i=0;i<aParamn;i++) {
    value += aParams[i] * xxx;
    xxx = xxx * aX;
  }
  return value;
}

static const unsigned int s_npt = 100;
static double s_xxs[s_npt];
static double s_vals[s_npt];
static double s_errs[s_npt];

inline bool function(
 int& aParamn
,double* //aGrad
,double& aValue
,double* aParams
,int //aStatus
,void*
){
  //  aParamn, aParams, aStatus : INPUT
  //  aGrad, aValue : OUTPUT
  double value;
  double chinew;
  double chisqr = 0.;
  for(unsigned int i=0;i<s_npt;i++) {
    value = polynomial(s_xxs[i],aParamn,aParams); 
    chinew = (s_vals[i] - value)/s_errs[i];
    chisqr = chisqr + chinew * chinew;
  }
  aValue = chisqr;
  return true;
}

int f2cmn(int,char**){
  // Fit with A + B * x :
  unsigned int nparam = 2;
  static const char* names[2] = {"A","B"};
  double params[2] = {0.5,2.};
  double steps[2] = {0.1,0.1};

  // Data to fit :
  unsigned int i;
  for(i=0;i<s_npt;i++) {
    s_xxs[i] = i + 0.1 * shoot_random_flat();
    s_vals[i] = 2. * ( i  + 0.1 * shoot_random_flat());
    s_errs[i] = 0.1;
  }

  inlib::f2cmn::fitter minuit(std::cout,0);
  minuit.SetFCN(function,0);

  int status;

  for(i=0;i<nparam;i++) {
    minuit.mnparm(i,names[i],params[i],steps[i],0.,0.,status);
  }

  double args[10];
  args[0] = 0;
  minuit.mnexcm("SET PRI",args,1,status);
  minuit.mnexcm("MIGRAD",args,0,status);
  //minuit.mnexcm("MINOS",args,0,status);
  //
  double bmin,bmax;
  std::string s;
  for(i=0;i<nparam;i++) {
    minuit.mnpout(i,s,params[i],steps[i],bmin,bmax,status);
  }

  return 0;
}

#ifndef __CLING__
int main(int argc,char** argv) {return f2cmn(argc,argv);}
#endif
