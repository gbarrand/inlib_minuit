/**

@page inlib_minuit_introduction Introduction

  inlib_minuit is the collection of inlib examples related to the inlib/f2cmn minimisation class.
 inlib/f2cmn is a modified version of the CERN-ROOT/TMinuit class in order to have a stand alone
 C++ version of the Minuit package. (CERN-ROOT/TMinuit being itself a FORTRAN to C translation
 of the FORTRAN CERNLIB/MINUIT package).

  In inlib/examples/cpp, there are:
@verbatim
   f2cmn.cpp      # example by using the "raw" minuit like interface.
   fit.cpp        # a "gaussian+breit_wigner" fit by using a inlib::fit_func and
                  # the inlib::f2cmn::fit_1D<> template function.
   fit_poly.cpp   # a polynomial fit by using inlib::f2cmn::fit_1D().
   fit_multi.cpp  # a multiple functions fit, by using the inlib::f2cmn::func_fitter::fit() API.
@endverbatim

  Someone sensitive to software engineering may appreciate the difference in number of lines of
 code to embarque between inlib/f2cmn (around 6 kloc) and CERN-ROOT (around... too much) in order
 to fit an histogram.

  See the README on github for instructions on how to build, install and run.

*/

