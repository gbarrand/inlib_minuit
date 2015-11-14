// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/random>
#include <inlib/func>

#include <inlib/f2cmn>

class poly_func : public virtual inlib::fit_func {
public:
  virtual double value(const inlib::fit_func::point& a_pt) const {
    float x = (float)a_pt[0];    
    return m_p.value(x);
  }
  virtual bool set(const double a_params[])  {
    m_p.set(a_params);
    return true;
  }
public:
  poly_func(){
    std::vector<double> ps(3,0);
    m_p.set(ps);
  }
public:
  inlib::func::polynomial m_p;
};

#include <inlib/histo/h1d>

#include <iostream>

int main(int,char**) {

  inlib::histo::h1d h("G distribution.",100,-5,5);

  inlib::random::gauss rg(0,1);
 {for(unsigned int index=0;index<100000;index++) h.fill(rg.shoot(),1);}

  //A*x*x+B*x+C

  std::vector< inlib::f2cmn::parameter > params(3);
  params[0].m_name = "C";
  params[0].m_start = 3000;
  params[0].m_step = 0.01;

  params[1].m_name = "B";
  params[1].m_start = 0;
  params[1].m_step = 0.01;

  params[2].m_name = "A";
  params[2].m_start = -1;
  params[2].m_step = 0.01;

  typedef inlib::f2cmn::command cmd;
  std::vector<cmd> cmds;
  cmds.push_back(cmd("SET PRI",1,0));
  //cmds.push_back(cmd("SET PRI",1,-1));
  cmds.push_back(cmd("MIGRAD"));
  //cmds.push_back(cmd("SHOW COV"));

  poly_func ff;
  std::vector<double> out;
  if(!inlib::f2cmn::fit_1D<inlib::histo::h1d>(std::cout,h,params,ff,out,cmds)){
    std::cout << "fit failed." << std::endl;
    return EXIT_FAILURE;
  }


  /*
 {double prec = 1e-0;
  double expected = 259.4;
  if(::fabs(out[1+1+0*4]-expected)>prec){
    std::cout << "unexpected C : " << out[2] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = -2.62;
  if(::fabs(out[1+1+1*4]-expected)>prec){
    std::cout << "unexpected B : " << out[6] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = -16.54;
  if(::fabs(out[1+1+2*4]-expected)>prec){
    std::cout << "unexpected A : " << out[10] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}
  */

  return 0;
}
