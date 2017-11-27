#include <cstring>
#include <fstream>
#include <iostream>
#include "image.h"

Image::Image (const std::string& filename)
  : Header (filename),
  p_data (NULL)
{
  std::cerr << "loading image \"" << filename << "\"... ";
  allocate();

  std::ifstream in (filename, std::ios::binary);
  if (!in)
    throw std::string ("error opening file \"") + filename + "\"";

  in.seekg (352);

  in.read (reinterpret_cast<char*>(p_data), num_voxels()*sizeof(float));
  if (!in)
    throw std::string ("error reading data from file  \"") + filename + "\": " + std::strerror (errno);

  std::cerr << "ok\n";
}


Image::Image (const Header& header)
  : Header (header)
{
  allocate();
}


Image::~Image ()
{
  delete [] p_data;
}


void Image::save (const std::string& filename)
{
  std::cerr << "writing image \"" << filename << "\"... ";
  std::ofstream out (filename, std::ios::binary);
  if (!out)
    throw std::string ("error opening output file \"") + filename + "\"";

  Header::write (out);
  out.seekp (352);

  out.write (reinterpret_cast<char*> (p_data), num_voxels()*sizeof(float));
  if (!out)
    throw std::string ("error writing data for output file \"") + filename + "\"";

  std::cerr << "ok\n";
}


unsigned long int Image::num_voxels () const
{
  long unsigned int num_vox = 1;
  for (int n = 0; n < ndim(); ++n)
    num_vox *= size(n);
  return num_vox;
}


void Image::allocate ()
{
  p_data = new float [num_voxels()];
  if (!p_data)
    throw std::string ("error: failed to allocate image data!");
  p_data[0] = 10.0;
  p_data[102000] = 20.0;
}

