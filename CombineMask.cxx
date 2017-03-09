/****************************************************************
*      Author: 
*      Dr. Rashed Karim 
*      Department of Biomedical Engineering, King's College London
*      Email: rashed 'dot' karim @kcl.ac.uk  
*      Copyright (c) 2016
*****************************************************************/

// Makes use of the proprietory Image Registration Toolkit library (IRTK) from Imperial Colllege London. 
#include <irtkImage.h>


// Other standard C libraries 
#include <fstream>
#include <vector>


int main(int argc, char **argv)
{
	
       char *mask_fn, *out_file;
       //double t1, t2, iterations, max_dist=3;
       bool skip_neigbours; 
       
       int maxX, maxY, maxZ, maxX2, maxY2, maxZ2;
       int is_debug_mode;  
       
       irtkRealPixel *ptr1, *ptr2;
       
       int label1, label2, output_label = 1;

       short*** out_img; 
		
		if (argc < 6) 
		{
			cout << "\nUsage: watershed2wall <mask_img> <outfile> <mask_label_1> <mask_label_2> <output_mask_label> \n\nThis program outputs" 
            "a mask image by combining pixels with label1 and label2, ***BUT*** only those label2 pixels that are adjacent to label1" << endl;
			exit(1); 
		}

       // get input parameters 
      
       mask_fn = argv[1]; 
       argv++; 
       argc--; 
       out_file = argv[1]; 
       argv++; 
       argc--; 
       
       label1 = atoi(argv[1]); 
       argv++; 
       argc--; 
       label2 = atoi(argv[1]); 
       argv++; 
       argc--; 

       output_label = atoi(argv[1]); 
       argv++; 
       argc--; 
       
       
       // read image
       irtkGreyImage mask_img;
       mask_img.Read(mask_fn);
        
       maxX = mask_img.GetX(); 
       maxY = mask_img.GetY(); 
       maxZ = mask_img.GetZ(); 
    
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
       
       cout << "Initialize data structures, starting image analysis .. " << endl;
       
    
        // iterate through the input image 
        for (i=0;i<maxX;i++)
        {
            for (j=0;j<maxY;j++)
            {
                for (k=0;k<maxZ;k++)
                {
                    // iterate through neighbours 
                    skip_neigbours = false; 
                    int mask_pixel = mask_img.Get(i,j,k);

                    if (mask_pixel != label1)
                    {
                        skip_neigbours = true;    
                    }
                    else 
                    {
                        skip_neigbours = false; 
                        out_img[i][j][k] = output_label;
                    }

                    for (a=-1;a<1 && !skip_neigbours;a++)
                    {
                        for (b=-1;b<1 && !skip_neigbours;b++)
                        {
                            for (c=-1;c<1 && !skip_neigbours;c++)
                            {
                                if (i+a >= 0 && i+a < maxX && j+b >= 0 && j+b < maxY && k+c >= 0 && k+c < maxZ)
                                {
    
                                    int mask_neighbour = mask_img.Get(i+a,j+b,k+c);

                                    if (mask_neighbour == label2)
                                    {
                                            out_img[i+a][j+b][k+c] = output_label;
                                    }
                                    
                                   
                                }
                            }
                        }
                    }
                                                                            
                }
            }
        }   // end image iteration loop 
        
       
       
       cout << "The labels have now been combined, now writing to output image .. " << endl;
       // write the output array to an image 
       irtkGreyImage out_img2;
       //out_img2 = irtkGreyImage(maxX, maxY, maxZ);
       out_img2 = mask_img; 
       
        for (i=0;i<maxX;i++) for (j=0;j<maxY;j++)  for (k=0;k<maxZ;k++) {
            
            out_img2.Put(i,j,k, out_img[i][j][k]);
        }
        
        out_img2.Write(out_file);
       
}