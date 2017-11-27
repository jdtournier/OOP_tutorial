#include <iostream>
#include <fstream>
#include "header.h"

Header::Header (const std::string& filename)
{
  std::cerr << "reading header for image \"" << filename << "\"... ";

  if (filename.substr (filename.size()-4) != ".nii")
    throw std::string ("image should be in NIfTI format with .nii extension");

  std::ifstream in (filename.c_str(), std::ios::binary);
  if (!in)
    throw std::string ("error opening file \"") + filename + "\"";

  in.read (reinterpret_cast<char*> (&p_header), sizeof (p_header));
  if (!in)
    throw std::string ("error reading header for file \"") + filename + "\"";

  in.close();

  if (p_header.sizeof_hdr != 348)
    throw std::string ("invalid header for file \"") + filename + "\"";

  if (p_header.datatype != DT_FLOAT32)
    throw std::string ("invalid datatype for file \"") + filename + "\" (expected float32)";

  std::cerr << "ok\n";
}


void Header::set_ndim (int new_ndim)
{
  int old_ndim = p_header.dim[0];
  p_header.dim[0] = new_ndim;
  for (int n = std::min (new_ndim, old_ndim); n < 7; ++n)
    p_header.dim[n+1] = 1;

}

void Header::disp() const
{
  std::cout << "header: [ ";
  for (int n = 0; n < ndim(); ++n)
    std::cout << size(n) << " ";
  std::cout << "]\n";
}


void Header::write (std::ofstream& out) const
{
  out.write (reinterpret_cast<const char*> (&p_header), sizeof(p_header));
  if (!out)
    throw std::string ("error writing header to output file");
}



