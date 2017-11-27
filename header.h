#ifndef __header_h__
#define __header_h__

#include <string>
#include <fstream>
#include "nifti1.h"

class Header
{
  public:
    // read header information for existing file:
    Header (const std::string& filename);

    // number of axes in image (i.e. 3ED, 4D, ...):
    int  ndim () const                  { return p_header.dim[0]; }

    // number of voxels along specified axis (indexed from zero):
    int  size (int axis) const          { return p_header.dim[axis+1]; }

    // set the number of axes in the image:
    void set_ndim (int new_ndim);

    // the number of voxels in the image, based on ndim() & size():
    unsigned long int num_voxels () const;

    // print information to terminal:
    void print () const;

  protected:
    nifti_1_header p_header;

    // default constructor is protected since it leaves the header in
    // unitialised state. A default constructor is required in the Image
    // constructor though, which is why it needs to be defined.
    Header () { }

    // read header from IO stream:
    void read (std::ifstream& in);

    // write header to IO stream:
    void write (std::ofstream& out) const;
};

#endif // __header_h__
