// This is spl/spl_ios_processes.h
#ifndef spl_ios_processes_h_
#define spl_ios_processes_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief Processes that use vcl_iostreams as sources and sinks
//
// \author Matt Leotta (mleotta@lems.brown.edu)
// \date 1/22/08
//
//          Copyright Matthew Leotta 2006 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// \verbatim
//  Modifications:
// \endverbatim
//--------------------------------------------------------------------------------


#include <spl/spl_process.h>
#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <vcl_fstream.h>
#include <vcl_algorithm.h>
#include <vul/vul_file_iterator.h>
#include <vul/vul_file.h>


//=============================================================================
// Sources
//=============================================================================



//: Read data of type T from an istream
template <class T >
class spl_istream_source : public spl_source
{
 public:
  spl_istream_source(const vcl_istream& istream) : is(istream) {}

  //: Execute the process
  spl_signal execute()
  {
    if(is.eof()){
      return SPL_EOS;
    }
    T data;
    is >> data;
    output(0, data);
    return SPL_VALID;
  }
  vcl_istream is;
};


//: Read data from an ifstream (base class)
class spl_ifstream_source_base : public spl_source
{
  public:
    virtual bool open(const vcl_string& filename) = 0;
};


//: Read data of type T from an ifstream
template <class T >
class spl_ifstream_source : public spl_ifstream_source_base
{
  public:
    spl_ifstream_source(const vcl_string& filename) { open(filename.c_str()); }
        
    bool open(const vcl_string& filename)
    {
      ifs.open(filename.c_str());
      return ifs.is_open();
    }
    
    //: Execute the process
    spl_signal execute()
    {
      if(ifs.eof()){
        return SPL_EOS;
      }
      T data;
      ifs >> data;
      output(0, data);
      return SPL_VALID;
    }
    
    vcl_ifstream ifs;
};


//: Read data of type T using ifstream from a list of files
template <class T >
class spl_ifstream_list_source : public spl_ifstream_source_base
{
  public:
    spl_ifstream_list_source(const vcl_string& glob) 
    : index(0) { open(glob); }
  
    spl_ifstream_list_source(const vcl_vector<vcl_string>& files) 
    : filenames(files), index(0) {}
  
    bool open(const vcl_string& glob)
    {
      filenames.clear();
      index = 0;
      
      for (vul_file_iterator fit=glob; fit; ++fit) {
        // check to see if file is a directory.
        if (vul_file::is_directory(fit()))
          continue;
        filenames.push_back(fit());
      }
      
      // no matching filenames
      if (filenames.empty())
        return false;
      
      // Sort - because the file iterator uses readdir() it does not
      //        iterate over files in alphanumeric order
      vcl_sort(filenames.begin(),filenames.end());
      
      return true;
    }
    
    //: Execute the process
    spl_signal execute()
    {
      if(index >= filenames.size()){
        return SPL_EOS;
      }
      vcl_ifstream ifs(filenames[index++].c_str());
      if(!ifs.is_open()){
        return SPL_INVALID;
      }
      T data;
      ifs >> data;
      ifs.close();
      
      output(0, data);
      return SPL_VALID;
    }
  
    vcl_vector<vcl_string> filenames;
    unsigned int index;
};





//=============================================================================
// Sinks
//=============================================================================


//: Write data of type T to an ostream
template <class T >
class spl_ostream_sink : public spl_sink
{
 public:
  spl_ostream_sink(vcl_ostream& stream) : os(stream) {}

  //: Execute the process
  spl_signal execute()
  {
    assert(input_type_id(0) == typeid(T));
    os << input<T>(0);
    return SPL_VALID;
  }
  vcl_ostream& os;
};




#endif // spl_ios_processes_h_
