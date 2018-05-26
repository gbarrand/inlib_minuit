/**

@page inexlib_py_introduction Introduction

  inexlib_py is the collection of exlib examples related to Python put in a standalone package
 and distribution.

  There are little applications written in C++, that activates Python through its library and
 a set of .py scripts that demonstrate how to activate inlib/exlib from a Python prompt. All
 these use the SWIG wrapping of some (but not all) classes of inlib and exlib.

  C++ apps (in 1.0.0) (source code in inexlib_py/inexlib/exlib/examples/cpp) :
@verbatim
    hello_py.cpp            # to check Python activation from C++.
    h1d_spy.cpp             # inlib histogram wrapped with swig.
    plotter_spy_screen.cpp  # exlib plotting wrapped with swig.
    mandel_py.cpp           # check calling a C++ function from Python.
@endverbatim
 and .py scripts :
@verbatim
    h1d.py          # inlib histo.
    rroot.py        # read an histo in a root file.
    plotter.py      # plot an histo by using softinex graphics and plotting.
    tree.py         # project a ntuple found in the pawdemo.root file and plot the histo.
    waxml.py        # write histos at the AIDA XML file format.
    polyhedron.py   # visualise a solid/shape boolean operation.
    cfitsio_hst.py  # visualise an image in a fits file.
@endverbatim

  See the README on github for instructions on how to build, install and run.

*/

