#include <iostream>
#include "image.h"


int main (int argc, char* argv[])
{
  try {
    if (argc != 3)
      throw std::string ("usage: example input.nii output.nii");

    Image in (argv[1]);

    Header header (in);
    header.set_ndim (3);

    Image out (header);

    std::cerr << "copying data... ";
    for (int z = 0; z < in.size(2); ++z)
      for (int y = 0; y < in.size(1); ++y)
        for (int x = 0; x < in.size(0); ++x)
          out.value(x,y,z) = in.value(x,y,z);
    std::cerr << "ok\n";

    out.save (argv[2]);
  }
  catch (std::string& message) {
    std::cerr << "\nERROR: " << message << std::endl;
    return 1;
  }

  return 0;
}

