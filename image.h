#ifndef __image_h__
#define __image_h__

#include "header.h"

class Image : public Header
{
  public:
    Image (const std::string& filename);
    Image (const Header& header);
    ~Image ();

    void save (const std::string& filename);

    float& value (int x, int y, int z, int n = 0)
    {
      return p_data[x+size(0)*(y+size(2)*(z+size(3)*n))];
    }

  private:
    float* p_data;

    void allocate ();
    unsigned long int num_voxels () const;
};


#endif // __image_h__

