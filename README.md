# worked example

#### Compile and run code
```
./build
./main data/fMRI_auditory.nii out.nii
```


#### What is does
This example copies the first volume of an input 4D (uncompressed) nifti image
into a new 3D nifti image. The point is to demonstrate a quick program to deal
with images.


The code defines a `Header` class, which carries the information about an
image, and an `Image` class that derives from it. An Image can be created from
an existing `Header` with the desired parameters, and a `Header` can be
copy-constructed from an existing `Header` (and hence from an `Image` since
it's a derived class). A typical workflow is therefore:

- open an existing `Image`
- copy-consruct a `Header` from the image
- modify the header as necessary
- constuct a new `Image` based on the modified header
- process data as required
- write output image to file

Access to the voxel values is by reference, meaning a single method is
sufficient to read & write (note that in practice a const version might be
required to allow read-only access to a const `Image`).


#### Limitations

- restricted to uncompressed NIfTI with data stored in 32-bit float with native
  byte-order.
- implementation is bare-bones, with just enough functionality for the purposes
  of this demo; a more mature implementation would also provide access to voxel
  sizes, comments, different data types, etc.


#### Build script

This is a simple script that emulates what a Makefile might do, based on simple
assumptions:

- all files have a `.cpp` or `.h` suffix
- all files resides in a flat structure in the same folder
- user-supplied headers are `#include`d in inverted commas (i.e. `#include
  "my_header.h"`), while system header are included in angled brackets (i.e.
  `#include <iostream>`); this is important if the script is work out
  dependencies correctly
- one and only one `.cpp` file will contain the `main()` function; the
  final executable will have the same name as that `.cpp` file (with the suffix
  stripped)


The build script also accepts a couple of special arguments:

- `clean`: remove all generated files (the executable and all objects).
- `nodebug`: create optimised code, not suitable for debugging (best to run
  `./build clean` beforehand to clear out any non-optimised temporaries).


#### Reading & writing NIfTI images

This implements a very restricted set of the NIfTI standard, namely
uncompressed, 32-bit floating point format, native byte-endian storage. The
NIfTI header structure is defined in `nifti1.h`, and is stored as-is at the
beginning of a valid NIfTI-1 file (with suffix `.nii`). Our implementation
therefore reads and writes this structure as a single bulk operation, and
checks that the required assumptions are met (valid NIfTI, supported datatype).
All that's needed after this are convenience functions to access the dimensions
of the dataset. 

The Image class extends this by adding a data buffer, allocated based on the
sizes reported by the header, along with methods to read and write the images.
It also provides a method for access to the voxel data, with offset computed
appropriately based on the image dimensions. 


