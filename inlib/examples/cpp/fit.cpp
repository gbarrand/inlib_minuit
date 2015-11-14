// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/random>
#include <inlib/func>

#include <inlib/f2cmn>

class fit_g_bw : public virtual inlib::fit_func {
public:  //inlib::fit_func :
  virtual double value(const inlib::fit_func::point& a_pt) const {
    float x = (float)a_pt[0];    
    return m_g.value(x)+m_bw.value(x);
  }
  virtual bool set(const double a_params[])  {
    m_g.set(a_params[0],a_params[1],a_params[2]);
    m_bw.set(a_params[3],a_params[4],a_params[5]);
    return true;
  }
public:
  inlib::func::gaussian m_g;
  inlib::func::bw m_bw;
};

#include <inlib/histo/h1d>
#include <inlib/args>

#include <iostream>

int main(int argc,char** argv) {

  inlib::args args(argc,argv);
  bool verbose = args.is_arg("-verbose");

  inlib::histo::h1d h("G+BW distribution.",100,-5,5);

  inlib::random::gauss rg(0,1);
 {for(unsigned int index=0;index<100000;index++) h.fill(rg.shoot(),1);}

 {inlib::random::bw rbw(3,0.5);
  for(int index=0;index<100000;index++) h.fill(rbw.shoot(),1);}

  std::vector< inlib::f2cmn::parameter > params(6);
  params[0].m_name = "H";
  params[0].m_start = 3000;
  params[0].m_step = 0.01;

  params[1].m_name = "M";
  params[1].m_start = 0;
  params[1].m_step = 0.01;

  params[2].m_name = "W";
  params[2].m_start = 4;
  params[2].m_step = 0.01;

  params[3].m_name = "A";
  params[3].m_start = 500;
  params[3].m_step = 0.01;

  params[4].m_name = "B";
  params[4].m_start = 2.5;
  params[4].m_step = 0.01;

  params[5].m_name = "C";
  params[5].m_start = 0.2;
  params[5].m_step = 0.01;

  typedef inlib::f2cmn::command cmd;
  std::vector<cmd> cmds;
  cmds.push_back(cmd("SET PRI",1,verbose?0:-1));
  cmds.push_back(cmd("MIGRAD"));
  //cmds.push_back(cmd("SHOW COV"));

  fit_g_bw ff;
  std::vector<double> out;
  if(!inlib::f2cmn::fit_1D<inlib::histo::h1d>(std::cout,h,params,ff,out,cmds)){
    std::cout << "fit failed." << std::endl;
    return EXIT_FAILURE;
  }


  /*
 {double prec = 1e-0;
  double expected = 3986.71;
  if(::fabs(out[2]-expected)>prec){
    std::cout << "unexpected H : " << out[2] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}
  */

 {double prec = 1e-2;
  double expected = 0;
  if(::fabs(out[1+1+1*4]-expected)>prec){
    std::cout << "unexpected M : " << out[6] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = 1;
  if(::fabs(out[1+1+2*4]-expected)>prec){
    std::cout << "unexpected W : " << out[10] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 /*
 {double prec = 1e-1;
  double expected = 12620.2;
  if(::fabs(out[1+1+3*4]-expected)>prec){
    std::cout << "unexpected A : " << out[14] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}
 */

 {double prec = 1e-2;
  double expected = 3;
  if(::fabs(out[1+1+4*4]-expected)>prec){
    std::cout << "unexpected B : " << out[18] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = 0.5;
  if(::fabs(out[1+1+5*4]-expected)>prec){
    std::cout << "unexpected C : " << out[22] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

  return 0;
}
