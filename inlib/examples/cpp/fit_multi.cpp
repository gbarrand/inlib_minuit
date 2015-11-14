// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

#include <inlib/random>
#include <inlib/func>

#include <inlib/f2cmn>

class fit_g_1 : public virtual inlib::fit_func {
public:
  virtual double value(const inlib::fit_func::point& a_pt) const {
    float x = (float)a_pt[0];    
    return m_g.value(x);
  }
  //WARNING : multi function and data set fit, then number of
  //          a_params is not the number of parameters of a gaussian.
  virtual bool set(const double a_params[])  {
    m_g.set(a_params[0],a_params[2],a_params[1]);
    return true;
  }
public:
  inlib::func::gaussian m_g;
};

class fit_g_2 : public virtual inlib::fit_func {
public:
  virtual double value(const inlib::fit_func::point& a_pt) const {
    float x = (float)a_pt[0];    
    return m_g.value(x);
  }
  //WARNING : multi function and data set fit, then number of
  //          a_params is not the number of parameters of a gaussian.
  virtual bool set(const double a_params[])  {
    m_g.set(a_params[0],a_params[3],a_params[1]);
    return true;
  }
public:
  inlib::func::gaussian m_g;
};

#include <inlib/histo/h1d>

#include <iostream>

int main(int,char**) {

  // first histo :
  inlib::histo::h1d h1("G1",100,-5,5);
 {inlib::random::gauss rg(0,1);
  for(unsigned int index=0;index<100000;index++)
    h1.fill(rg.shoot(),1);}

  std::vector< inlib::f2cmn::point > data1;
 {int nbin = h1.axis().bins();
  for(int index=0;index<nbin;index++) { 
    int nent = h1.bin_entries(index);
    if(nent==0) continue;  // Do not take bin without entry.
    // Take middle of bin.
    double xx = (h1.axis().bin_lower_edge(index)+h1.axis().bin_upper_edge(index))/2.;
    double val = h1.bin_height(index);
    double err = ::sqrt(::fabs(val)); // Take statistical error.
    if(err==0.) { 
      std::cout << "null error for bin " << index 
                << " x " << xx << " value " << val
                << ". Set error to 0.01."
                << std::endl;
      //return 0;
      err = 0.01; //FIXME
    }
    inlib::f2cmn::point fpt;
    fpt.m_coords.resize(1);
    fpt.m_coords[0] = xx;
    fpt.m_value = val;
    fpt.m_error = err;
    data1.push_back(fpt); 
  }}

  // second histo :
  inlib::histo::h1d h2("G2",100,0,10);
 {inlib::random::gauss rg(5,1);
  for(unsigned int index=0;index<100000;index++)
    h2.fill(rg.shoot(),1);}

  std::vector< inlib::f2cmn::point > data2;
 {int nbin = h2.axis().bins();
  for(int index=0;index<nbin;index++) { 
    int nent = h2.bin_entries(index);
    if(nent==0) continue;  // Do not take bin without entry.
    // Take middle of bin.
    double xx = (h2.axis().bin_lower_edge(index)+h2.axis().bin_upper_edge(index))/2.;
    double val = h2.bin_height(index);
    double err = ::sqrt(::fabs(val)); // Take statistical error.
    if(err==0.) { 
      std::cout << "null error for bin " << index 
                << " x " << xx << " value " << val
                << ". Set error to 0.01."
                << std::endl;
      //return 0;
      err = 0.01; //FIXME
    }
    inlib::f2cmn::point fpt;
    fpt.m_coords.resize(1);
    fpt.m_coords[0] = xx;
    fpt.m_value = val;
    fpt.m_error = err;
    data2.push_back(fpt); 
  }}

  if(data1.empty()||data2.empty()) {
    std::cout << "no starting points." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector< inlib::f2cmn::parameter > params(4);
  params[0].m_name = "H";
  params[0].m_start = 300;
  params[0].m_step = 0.01;

  params[1].m_name = "W";
  params[1].m_start = 4;
  params[1].m_step = 0.01;

  params[2].m_name = "M1";
  params[2].m_start = 0;
  params[2].m_step = 0.01;

  params[3].m_name = "M2";
  params[3].m_start = 4;
  params[3].m_step = 0.01;

  typedef inlib::f2cmn::command cmd;
  std::vector<cmd> cmds;
  //cmds.push_back(cmd("SET PRI",1,0));
  cmds.push_back(cmd("SET PRI",1,-1));
  cmds.push_back(cmd("MIGRAD"));
  //cmds.push_back(cmd("SHOW COV"));

  std::vector<double> out;

  // df is for a pair (data,function).

  //NOTE : the size of out is : 
  //         1 + dfs.size() + params.size() * 4
  //       with :
  //         out[0] = chi2
  //         out[1] = number of degree of freedom for first data set.
  //         out[2] = number of degree of freedom for scnd data set.
  //         ....
  //         out[1+ds.size()+iparam*4+0,1,2,3] value,err,bmin,bmax

  typedef std::vector<inlib::f2cmn::point> data_t;
  typedef std::pair<data_t*,inlib::fit_func*> df_t;

  fit_g_1 f1;
  fit_g_2 f2;

  std::vector<df_t> dfs;
  dfs.push_back(df_t(&data1,&f1));
  dfs.push_back(df_t(&data2,&f2));
  
  if(!inlib::f2cmn::func_fitter::fit(std::cout,dfs,params,out,cmds)) {
    std::cout << "fit failed." << std::endl;
    return EXIT_FAILURE;
  }

  /*
 {double prec = 1e-0;
  double expected = 3997.18;
  if(::fabs(out[1+2+0*4]-expected)>prec){
    std::cout << "unexpected H : " << out[1+2+0*4] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}
  */

 {double prec = 1e-2;
  double expected = 1;
  if(::fabs(out[1+2+1*4]-expected)>prec){
    std::cout << "unexpected W : " << out[7] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = 0;
  if(::fabs(out[1+2+2*4]-expected)>prec){
    std::cout << "unexpected M1 : " << out[11] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

 {double prec = 1e-2;
  double expected = 5;
  if(::fabs(out[1+2+3*4]-expected)>prec){
    std::cout << "unexpected M2 : " << out[15] << "."
              << " Expected " << expected << "."
              << std::endl;
    return EXIT_FAILURE;
  }}

  return EXIT_SUCCESS;
}
