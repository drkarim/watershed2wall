/****************************************************************
*      Author: 
*      Dr. Rashed Karim 
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk  
*      Copyright (c) 2016
*****************************************************************/

// Makes use of the proprietory Image Registration Toolkit library (IRTK) from Imperial Colllege London. 
#include <irtkImage.h>
#include <irtkEuclideanDistanceTransform.h>

// Other standard C libraries 
#include <fstream>
#include <vector>

void computeEDT(irtkGreyImage* img1, irtkRealImage* edt_img) 
{

	irtkRealImage img3; 
	img3 =* img1; 
	irtkRealPixel *p; 
	p = img3.GetPointerToVoxels(); 
	
	// reverse the segmentation mask for EDT
	for (int i=0;i<img3.GetNumberOfVoxels();i++) 
	{
		if (*p>0)
			*p = 1; 
		else
			*p = 0; 
		p++; 
	}

	irtkEuclideanDistanceTransform<irtkRealPixel> *edt = NULL;
	edt = new irtkEuclideanDistanceTransform<irtkRealPixel>(irtkEuclideanDistanceTransform<irtkRealPixel>::irtkDistanceTransform3D);
	edt->SetInput(&img3);
	edt->SetOutput(edt_img);
	edt->Run();
	//edt_img->Write("edt.gipl");
}


int main(int argc, char **argv)
{
	
       char *in_file, *out_file, *la_mask_file;
       double t1, t2, iterations, max_dist=3;
       bool skip_neigbours; 
       
       int maxX, maxY, maxZ, maxX2, maxY2, maxZ2;
       int is_debug_mode;  
       
       irtkRealPixel *ptr1, *ptr2;
       
       short*** out_img; 
		
		if (argc < 7) 
		{
			cout << "\nUsage: watershed2wall <input_img> <la_mask> <out_img> <intensity_t1> <intensity_t2> <max_dist>\n\nThis program outputs" 
            "the la wall segmented from a watershed segmentation based on intensity values [t1,t2] and dilation with n steps" << endl;
			exit(1); 
		}

       // get input parameters 
       in_file = argv[1];
       argv++;
       argc--;
       la_mask_file = argv[1]; 
       argv++; 
       argc--; 
       out_file = argv[1]; 
       argv++; 
       argc--; 
       t1 = atof(argv[1]); 
       argv++; 
       argc--; 
       t2 = atof(argv[1]); 
       argv++; 
       argc--; 
       max_dist = atof(argv[1]); 
       argv++; 
       argc--; 
       
       
       // read image
       irtkRealImage in_img, edt_img; 
       irtkGreyImage la_mask_img;
       
       in_img.Read(in_file);
       la_mask_img.Read(la_mask_file);
        
       maxX = in_img.GetX(); maxX2 = la_mask_img.GetX(); 
       maxY = in_img.GetY(); maxY2 = la_mask_img.GetY(); 
       maxZ = in_img.GetZ(); maxZ2 = la_mask_img.GetZ(); 
       
       if (maxX != maxX2 && maxY != maxY2 && maxZ != maxZ2)
       {
           cerr << "The dimension of the input image and input masks is a mismatch\n" << endl;
            exit(1);
       }
       
       // initialize output image
       int i,j,k,a,b,c; 
       out_img= (short***) malloc (maxX*sizeof(short**));
	   for (i=0;i<maxX;i++) out_img[i]= (short**) malloc (maxY*sizeof(short*));
	   for (i=0;i<maxX;i++) for (j=0;j<maxY;j++) out_img[i][j]= (short*) malloc (maxZ*sizeof(short));
       
       // Set output image pixels to 0 
       for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
           
           // copy la mask
           //short v = la_mask_img.Get(i,j,k);    
           out_img[i][j][k] = 0;        
           
       }
       
       edt_img = in_img; 
       computeEDT(&la_mask_img, &edt_img); 
        
       cout << "Initialize data structures, starting image analysis .. " << endl;
       
       short this_dilation_label, prev_dilation_label; 
       
       this_dilation_label = 0; 
       prev_dilation_label = 1; 
       
       for (int dilations=0;dilations < iterations;dilations++) {
            
            this_dilation_label = 1+prev_dilation_label;
        
            // iterate through the input image 
            for (i=0;i<maxX;i++)
            {
                for (j=0;j<maxY;j++)
                {
                    for (k=0;k<maxZ;k++)
                    {
        
                        double in_img_pixel = in_img.Get(i,j,k);
                        double edt = edt_img.Get(i,j,k); 

                        if (edt > 0 && edt < max_dist) 
                        {
                            if (in_img_pixel > t1 && in_img_pixel < t2)
                            {
                                out_img[i][j][k] = 1;
                            }
                        }
                                                                                
                    }
                }
            }   // end image iteration loop 
            
            prev_dilation_label = this_dilation_label;      // advance the label
            
       }  // end dilation iteration loop 
  
       
       cout << "Completed extracting wall pixels from Watershed output, now writing to output image .. " << endl;
       // write the output array to an image 
       irtkGreyImage out_img2;
       //out_img2 = irtkGreyImage(maxX, maxY, maxZ);
       out_img2 = la_mask_img; 
       
        for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
            
            out_img2.Put(i,j,k, out_img[i][j][k]);
        }
        
        out_img2.Write(out_file);
       
}