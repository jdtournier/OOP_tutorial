#include <cstring>
#include <fstream>
#include <iostream>
#include "image.h"

Image::Image (const std::string& filename)
{
  std::cerr << "loading image \"" << filename << "\"... ";
  try {

    std::ifstream in (filename.c_str(), std::ios::binary);
    if (!in)
      throw std::string ("error opening image for reading");

    Header::read (in);

    p_data.resize (num_voxels());

    in.seekg (352);
    in.read (reinterpret_cast<char*>(&p_data[0]), num_voxels()*sizeof(float));
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
  p_data.resize (num_voxels(), 0.0);
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

    out.write (reinterpret_cast<char*> (&p_data[0]), num_voxels()*sizeof(float));
    if (!out)
      throw std::string ("error writing data");
  }
  catch (std::string& text) {
    throw text + " (in file \"" + filename + "\")";
  }

  std::cerr << "ok\n";
}

