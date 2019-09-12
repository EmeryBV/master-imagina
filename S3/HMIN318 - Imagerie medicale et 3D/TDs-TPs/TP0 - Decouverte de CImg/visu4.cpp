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

#include "CImg.h"
#include <limits>
#include <string>

using namespace cimg_library;


CImg<float> MIP(CImg<float>& img){

	CImg<float> mip = img;

	for(int y = 0 ; y < img.height() ; y++)
	{
		for(int x = 0 ; x < img.width() ; x++)
		{
			float current_max = std::numeric_limits<float>::min();
			int z_max = 0;

			for(int z = 0 ; z < img.depth() ; z++)
			{
				if(img(x, y, z) > current_max) {
					current_max = img(x, y, z);
					z_max = z;
				}
			}

			mip(x, y, z_max) = current_max;
		}
	}

	return std::move(mip);
}

/* Main program */
int main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("Usage : %s filename.hdr\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Create and load the 3D image */
	CImg<float> img;
	float voxelsize[3];
	/* Load in Analyze format and get the voxel size in an array */
	img.load_analyze(argv[1],voxelsize);
	
	/* Get the image dimensions */
    int dim[]={img.width(),img.height(),img.depth()}; 
	printf("Reading %s. Dimensions=%d %d %d\n",argv[1],dim[0],dim[1],dim[2]);
	printf("Voxel size=%f %f %f\n",voxelsize[0],voxelsize[1],voxelsize[2]);


	unsigned int voxel_coord[] = {256, 256, 12};
	printf("Voxel(%u,%u,%u) = %lf\n", voxel_coord[0], voxel_coord[1], voxel_coord[2], img(voxel_coord[0], voxel_coord[1], voxel_coord[2]));

	/* Create the display window of size 512x512 */
	CImgDisplay disp(512,512,"");
	printf("disp.normalization() = %d\n", disp.normalization());
	
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
	std::string visu_mode = "MPR";
	/* Manage the display windows: ESC, or closed -> close the main window */
	while (!disp.is_closed() && !disp.is_keyESC()) // Main loop
	{
		/* List of events */
		
		/* Resizing */
		if (disp.is_resized()) 
		{
			disp.resize();
		}

		if (disp.is_key("m"))
		{
			img.blur(0.5);
		}

		if (disp.is_key("r"))
		{
			visu_mode = "MPR";
		}

		if (disp.is_key("a"))
		{
			visu_mode = "MIP";
		}

		if (disp.is_key("z"))
		{
			visu_mode = "MinIP";
		}

		if (disp.is_key("e"))
		{
			visu_mode = "AIP";
		}

		/* Movement of the mouse */
		
		/* If the mouse is inside the display window, find the active quadrant 
		and the relative position within the active quadrant */
		if(disp.mouse_x()>=0 && disp.mouse_y()>=0) 
		{
			
			unsigned int mX = disp.mouse_x()*(dim[0]+dim[2])/disp.width();
			unsigned int mY = disp.mouse_y()*(dim[1]+dim[2])/disp.height();
			
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
		
		/* Click Right button to get a position */
		if (disp.button()&2  && (plane!=-1))  
		{
			for(unsigned int i=0;i<3;i++) 
			{
				displayedSlice[i]=coord[i];
			}
			redraw = true;
		}
		/* Wheel interaction */
		if (disp.wheel()) 
		{
			displayedSlice[plane]=displayedSlice[plane]+disp.wheel();
			
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
			
		/* Flush all mouse wheel events in order to not repeat the wheel event */
		disp.set_wheel();
			
		redraw = true;
		}
		
		/* If any change, we need to redraw the main windows */
		if (redraw)
		{
			/* Create a 2D image based on the MPR projections given by a projection point 
			which is the intersection of the displayed slices */
			CImg<float> mpr_img = img.get_projections2d(displayedSlice[0],displayedSlice[1],displayedSlice[2]);
			// CImg<float> mpr_img = img.get_slice(displayedSlice[2]);
			// CImg<float> mpr_img = img.get_slice(displayedSlice[2]);
			
			
			if (visu_mode == "MIP")
			{
				mpr_img = MIP(mpr_img);
			}
	
			/* The MPR image has a given size. It needs to be resized in order to fit at best in the display window */
			mpr_img.resize(512,512); 
			
			/* Display the MPR image in the display windows */
			disp.display(mpr_img);

			
			/* To avoid repetitive continuous redrawing */
			redraw=false;
		}
	}
	return 0;
}


