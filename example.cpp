#include <iostream>
#include <vector>
#include <string>
#include "nifti1_io.h"

/***************************************************
 *              CLASS DECLARATIONS                 *
 **************************************************/

class Image; // <== forward declaration to allow later use


class Header 
{
  public:
    Header (const Image& image);
    void set_ndim (int new_ndim);
    void disp() const;
  private:
    nifti_1_header p_header;

    friend class Image;
};






class Image 
{
  public:
    Image ();
    Image (const Image& src);
    ~Image ();
    void load (const std::string& filename); 
    void create (const Header& header, const std::string& filename);
    void save ();

    int ndim () const;
    int size (int axis) const;
    float get_value (int x, int y, int z) const;
    float get_value (int x, int y, int z, int n) const;
    void  set_value (float value, int x, int y, int z);
    void  set_value (float value, int x, int y, int z, int n);
  private:
    nifti_image* p_image;

    friend class Header;
};



/***************************************************
 *                 INLINE MEMBER FUNCTIONS         *
 **************************************************/


// HEADER:


inline Header::Header (const Image& image) 
{
  p_header = nifti_convert_nim2nhdr (image.p_image);
}

inline void Header::set_ndim (int new_ndim)
{
  int old_ndim = p_header.dim[0];
  p_header.dim[0] = new_ndim;
  for (int n = std::min (new_ndim, old_ndim); n < 7; ++n)
    p_header.dim[n+1] = 1;

}

inline void Header::disp() const 
{
  disp_nifti_1_header ("header: ", &p_header);
}





// IMAGE:

inline Image::Image () : p_image (NULL) { }

inline Image::~Image () {
  if (p_image)
    nifti_image_free (p_image); 
}

inline void Image::load (const std::string& filename) 
{
  p_image = nifti_image_read (filename.c_str(), 1);
  if (!p_image) 
    throw std::string ("error reading image \"") + filename + "\"";
  if (p_image->datatype != DT_FLOAT)
    throw std::string ("error: datatype is not in floating-point format!");
  if (!p_image->data)
    throw std::string ("error: failed to load image data!");
}

inline void Image::create (const Header& header, const std::string& filename)
{
  if (!nifti_hdr_looks_good (&header.p_header))
    throw std::string ("header is not valid!");
  p_image = nifti_convert_nhdr2nim (header.p_header, filename.c_str());
  long unsigned int num_vox = 1;
  for (int n = 0; n < ndim(); ++n)
    num_vox *= size(n);
  p_image->data = malloc (sizeof(float)*num_vox);
  if (!p_image->data)
    throw std::string ("error: failed to allocate image data!");
}

inline void Image::save ()
{
  if (!p_image)
    throw std::string ("cannot save image: image is empty");
  nifti_image_write (p_image);
}


inline int Image::ndim () const
{ 
  return p_image->ndim;
}

inline int Image::size (int axis) const
{
  return p_image->dim[axis+1];
}


inline float Image::get_value (int x, int y, int z) const
{
  return reinterpret_cast<float*>(p_image->data)[x+size(0)*(y+size(2)*z)];
}

inline float Image::get_value (int x, int y, int z, int n) const
{
  return reinterpret_cast<float*>(p_image->data)[x+size(0)*(y+size(2)*(z+size(3)*n))];
}

inline void Image::set_value (float value, int x, int y, int z)
{
  reinterpret_cast<float*>(p_image->data)[x+size(0)*(y+size(2)*z)] = value;
}

inline void Image::set_value (float value, int x, int y, int z, int n)
{
  reinterpret_cast<float*>(p_image->data)[x+size(0)*(y+size(2)*(z+size(3)*n))] = value;
}




/***************************************************
 *               BODY OF MAIN PROGRAM              *
 **************************************************/

int main (int argc, char* argv[]) 
{
  try {
    if (argc != 3)
      throw std::string ("usage: example input.nii output.nii");

    Image input;
    input.load (argv[1]);

    std::cout << "found image with dimensions " << input.size(0);
    for (int n = 1; n < input.ndim(); ++n)
      std::cout << " x " << input.size (n);
    std::cout << std::endl;


    Header header (input);
    header.set_ndim (3);

    Image output;
    output.create (header, argv[2]);
    
    for (int z = 0; z < input.size(2); ++z) 
      for (int y = 0; y < input.size(1); ++y) 
        for (int x = 0; x < input.size(0); ++x) 
          output.set_value (input.get_value(x,y,z), x,y,z);

    std::cerr << "saving...\n";
    output.save();
  }
  catch (std::string& message) {
    std::cerr << message << std::endl;
    return 1;
  }

  return 0;
}

