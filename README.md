# Post-processing for marker-controlled Watershed segmentation
Marker-controlled Watershed segmentation of atrial wall MRI images is possible. However, it requires post-processing for finalising the atrial wall segmentation. The following repository contains two separate that accomplish these tasks 

## Usage 
The usage for the first tool ```combinmask``` is:
```
combinmask <mask_img> <outfile> <mask_label_1> <mask_label_2> <output_mask_label>
```
This program outputs a mask image by combining pixels with label1 and label2, ***BUT*** only those label2 pixels that are adjacent to label1

The output from the first tool is used as input to the second tool below; 

The usage for the second tool ```watershed2wall``` is:
```
watershed2wall <input_MRI_img> <mask_from_combinmask> <out_img> <intensity_t1> <intensity_t2> <max_dist>
```
where ```intensity_t1``` and ```intensity_t2``` are intensities (min,max) sampled from atrial wall. ```max_dist``` is the limit on the  distance from atrial chamber that this program should search. 

![alt tag](https://postimg.org/image/5x98t9lwf/)

## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
