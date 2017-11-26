# rough cut analysis (uses tools from MRtrix3):

mrconvert fMRI_auditory.nii -coord 3 0:5,12:17,24:29,36:41,48:53,60:65,72:77,84:89 - | mrmath - -axis 3 mean rest.nii
mrconvert fMRI_auditory.nii -coord 3 6:11,18:23,30:35,42:47,54:59,66:71,78:83,90:95 - | mrmath - -axis 3 mean task.nii
mrcalc task.nii rest.nii -sub rest.nii -div effect_size.nii
mrfilter effect_size.nii smooth effect_size_smooth.nii
mrview structural.nii -overlay.load effect_size_smooth.nii   # then apply lower threshold at 0.01


