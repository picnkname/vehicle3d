// This is spl/filters/vil_diff_process.h
#ifndef vil_diff_process_h_
#define vil_diff_process_h_

//:
// \file
// \brief Process that computes absolute difference between images
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 12/21/04
//
//          Copyright Matthew Leotta 2006 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// \verbatim
//  Modifications
// \endverbatim

#include <spl/spl_process.h>
#include <spl/spl_process_factory.h>
#include <vcl_vector.h>
#include <vcl_string.h>

//: Process that computes absolute difference between images
class vil_diff_process : public spl_filter {

public:
  //: Constructor
  vil_diff_process() {}
  //: Destructor
  virtual ~vil_diff_process() {}

  class factory : public spl_process_factory
  {
    public:
    //: Return the default set of parameters for the process
      virtual spl_parameters_sptr default_params() const;

    //: Construct a process from a set of parameters
      virtual spl_process_sptr create(const spl_parameters_sptr& params) const;

    //: The name of the process
      virtual vcl_string name() const { return "Image Difference"; }

      virtual ~factory() {}
  };


  //: Execute this process
  spl_signal execute();

};

#endif
