#include <cstring>
#include <fstream>
#include <iostream>
#include "image.h"

Image::Image (const std::string& filename) :
  p_data (NULL)
{
  std::cerr << "loading image \"" << filename << "\"... ";
  try {

    std::ifstream in (filename.c_str(), std::ios::binary);
    if (!in)
      throw std::string ("error opening image for reading");

    Header::read (in);

    allocate();

    in.seekg (352);
    in.read (reinterpret_cast<char*>(p_data), num_voxels()*sizeof(float));
    if (!in)
      throw std::string ("error reading data");
  }
  catch (std::string& text) {
    throw text + " (in file \"" + filename + "\")";
  }

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
  try {
    std::ofstream out (filename.c_str(), std::ios::binary);
    if (!out)
      throw std::string ("error opening image for writing");

    Header::write (out);
    out.seekp (352);

    out.write (reinterpret_cast<char*> (p_data), num_voxels()*sizeof(float));
    if (!out)
      throw std::string ("error writing data");
  }
  catch (std::string& text) {
    throw text + " (in file \"" + filename + "\")";
  }

  std::cerr << "ok\n";
}


void Image::allocate ()
{
  p_data = new float [num_voxels()];
  if (!p_data)
    throw std::string ("failed to allocate image data!");
  p_data[0] = 10.0;
  p_data[102000] = 20.0;
}

