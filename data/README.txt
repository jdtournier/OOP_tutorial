From SPM tutorial data (http://www.fil.ion.ucl.ac.uk/spm/data/auditory/):

> 96 acquisitions were made (RT=7s), in blocks of 6, giving 16 42s blocks. The
> condition for successive blocks alternated between rest and auditory
> stimulation, starting with rest. Auditory stimulation was with bi-syllabic
> words presented binaurally at a rate of 60 per minute. The functional data
> starts at acquisiton 4, image fM00223_004. Due to T1 effects it is advisable to
> discard the first few scans (there were no "dummy" lead-in scans).


So we have 96 fMRI volumes, with 8 alternating blocks of 6 rest + 6 tasks,
along with a T1 structural scan.









# for reference, initial data import was (not necessary for processing):

# convert from Analyze to NIfTI (uses mrconvert from MRtrix3):
mrconvert sM00223/sM00223_002.img structural.nii
mrconvert fM00223/fM00223_[].img fMRI.nii -axes 0,1,2,4 -force

# register fMRI to structural (uses epi_reg from FSL):
epi_reg --epi=fMRI.nii --t1=structural.nii --t1brain=bet --out=epi2struct

# apply transformation to fMRI (uses tools from MRtrix3):
transformconvert epi2struct.mat fMRI.nii structural.nii flirt_import trans.txt
mrtransform fMRI.nii -linear trans.txt -force fMRI_auditory.nii
