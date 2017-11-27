#ifndef __header_h__
#define __header_h__

#include <string>
#include <fstream>
#include "nifti1.h"

class Header 
{
  public:
    Header (const std::string& filename);

    int  ndim () const                  { return p_header.dim[0]; }
    int  size (int axis) const          { return p_header.dim[axis+1]; }
    void set_ndim (int new_ndim);
    void disp() const;

    void write (std::ofstream& out) const;

  private:
    nifti_1_header p_header;
};

#endif // __header_h__
