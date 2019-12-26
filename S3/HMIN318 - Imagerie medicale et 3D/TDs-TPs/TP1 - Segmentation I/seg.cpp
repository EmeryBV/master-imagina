/* 	HMIN318
	2018-2019
	Gérard Subsol
	(adapted of a program written by Benjamin Gilles)
	
	Compilation:
	(Windows version) g++ -o visu4.exe visu4.cpp -O2 -lgdi32
	(Linux  version) g++  -o visu4.exe visu4.cpp -O2 -L/usr/X11R6/lib  -lm  -lpthread -lX11

	Execution :
	visu4.exe knix.hdr
*/
	
/* Warning! the MPR value does not deal with the voxel size. It is assumed isotropic. */ 	

#include "pch.h"

using namespace std;
using namespace cimg_library;

/* Main program */
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		cerr << "Usage : " << argv[0] << " image.hdr\n";
		exit(EXIT_FAILURE);
	}

	int threshold = 43;

	cerr << "Lecture de l'image...\n";
	CImg<float> image_in;
	float voxelsize[3];
	image_in.load_analyze(argv[1], voxelsize);
	
	cerr << "Recupération des dimensions\n";
    int dim[]={image_in.width(),image_in.height(),image_in.depth()}; 
	printf("%s. Dimensions=%d %d %d\n",argv[1],dim[0],dim[1],dim[2]);
	printf("Voxel size=%f %f %f\n",voxelsize[0],voxelsize[1],voxelsize[2]);

	cerr << "Seuillage pour séparer l'image du fond...\n";
	CImg<unsigned char> image_thresholded = image_in.get_threshold(threshold, true, false);

	cerr << "Erosion de l'image\n";
    CImg<unsigned char> image_eroded = image_thresholded.get_erode(3, 3, 3);
    image_eroded.get_erode(3, 3, 3);

	cerr << "Detection de la MCC...\n";
	CImg<unsigned char> image_mcc = image_eroded.get_label(true);

	cerr << "Creation de l'histogramme...\n";
	array<size_t, 500> histogram = {0};

	cimg_for(image_mcc, intensity, unsigned char)
	{
        histogram[*intensity]++;
    }

    cerr << "Récuperation de l'intensitée de voxel ayant le plus d'occurence dans l'image...\n";
    // l'index 0 corréspond au fond de l'image qui est a exclure donc on commence à 1.
    size_t i_max = (max_element(histogram.begin() + 1, histogram.end()) - histogram.begin());

    cerr << "Detection de la composante de l'image...\n";
    unsigned char voxel;

    cimg_for (image_mcc, intensity, unsigned char)
    {
        voxel = *intensity;
        if (voxel == i_max)
            *intensity = 255;
        else
            *intensity = 0;
    }

	cerr << "Dilatation de l'image...\n";

	CImg<unsigned char> image_dilated = image_mcc.get_dilate(3, 3, 3);
	image_dilated.dilate(3, 3, 3);

	cerr << "Affichage de l'image...\n";

	CImgDisplay image_disp(image_dilated, "Display");
	// /printf("disp.normalization() = %d\n", disp.normalization());
	
	/* The 3 displayed slices of the MPR visualisation */
	int displayedSlice[]={dim[0]/2,dim[1]/2,dim[2]/2}; 
		
	/* Slice corresponding to mouse position: */
	unsigned int coord[]={0,0,0};
	/* The display window corresponds to a MPR view which is decomposed into the following 4 quadrants: 
	2=original slice size=x y        0 size=z y
	1= size=x z                     -1 corresponds to the 4th quarter where there is nothing displayed */
	int plane=2;
	
	/* For a first drawing, activate the redrawing flag */
	bool redraw=true;

	// CImg<unsigned char> current_img = image_in;


	/* Manage the display windows: ESC, or closed -> close the main window */
	while (!image_disp.is_closed() && !image_disp.is_keyESC()) // Main loop
	{
		if(image_disp.mouse_x()>=0 && image_disp.mouse_y()>=0) 
		{
			unsigned int mX = image_disp.mouse_x()*(dim[0]+dim[2])/image_disp.width();
			unsigned int mY = image_disp.mouse_y()*(dim[1]+dim[2])/image_disp.height();
			
			if (mX>=dim[0] && mY<dim[1]) 
			{ 
				plane = 0; 
				coord[1] = mY; 
				coord[2] = mX - dim[0];   
				coord[0] = displayedSlice[0]; 
			}
			else 
			{
				if (mX<dim[0] && mY>=dim[1]) 
				{ 
					plane = 1; 
					coord[0] = mX; 
					coord[2] = mY - dim[1];   
					coord[1] = displayedSlice[1]; 
				}
				else 
				{
					if (mX<dim[0] && mY<dim[1])       
					{ 
						plane = 2; 
						coord[0] = mX; 
						coord[1] = mY;     
						coord[2] = displayedSlice[2]; 
					}
					else 
					{
						plane = -1; 
						coord[0] = 0;
						coord[1] = 0;
						coord[2] = 0;
					}
				}
			}
			redraw = true;
		}
		
		if (image_disp.button()&2  && (plane!=-1))  
		{
			for(unsigned int i=0;i<3;i++) 
			{
				displayedSlice[i]=coord[i];
			}
			redraw = true;
		}

		if (image_disp.wheel()) 
		{
			displayedSlice[plane]=displayedSlice[plane] + image_disp.wheel();
			
			if (displayedSlice[plane]<0) 
			{
				displayedSlice[plane] = 0;
			}
			else 
			{
				if (displayedSlice[plane]>=(int)dim[plane])
				{
					displayedSlice[plane] = (int)dim[plane]-1;
				}
			}
			
			image_disp.set_wheel();
			redraw = true;
		}
		
		if (redraw)
		{
			CImg<float> mpr_image = image_dilated.get_projections2d(displayedSlice[0],displayedSlice[1],displayedSlice[2]);

			image_disp.display(mpr_image);
			redraw=false;
		}
		
	}
	return 0;
}


