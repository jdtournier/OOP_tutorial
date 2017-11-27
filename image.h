#ifndef __image_h__
#define __image_h__

#include "header.h"

class Image : public Header
{
  public:
    // create new image based on header supplied:
    Image (const Header& header);

    // open existing image:
    Image (const std::string& filename);

    ~Image ();

    // write image to file:
    void save (const std::string& filename);

    // get value at x,y,z,n by reference
    // can be used for both reading and writing
    float& value (int x, int y, int z, int n = 0)
    {
      return p_data[x+size(0)*(y+size(2)*(z+size(3)*n))];
    }

  private:
    float* p_data;

    // allocate RAM for data from number of voxels reported by header:
    void allocate ();
};


#endif // __image_h__

