#include "life.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <thread>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

//Copies one array to another.
void copy(uint8_t * array1, uint8_t * array2)
{
  for(int j = 0; j < height; j++)
  {
		for(int i = 0; i < width; i++) 			
			array2[j*width+i] = array1[j*width+i]; 
	}
}

//The life function is the most important function in the program.
//It counts the number of cells surrounding the center cell, and 
//determines whether it lives, dies, or stays the same.

int conworld(int val, int max) {
	if(val >= max) {
		return 0;//val%max;		
	} else if(val < 0) {
		return max-1;//(max + (val%max))%max;
	} else {
		return val;
	}
	
}

inline int getIdx2D(int j, int i, int height, int width) {
	return (conworld((j), height) * (width)) + conworld((i),(width));
}

void life_multi(uint8_t * & array, uint8_t * & temp, char choice) {
	std::vector<std::thread > mWorker;
	size_t numOfWorker_x = 10;
	size_t numOfWorker_y = 10;
	size_t sizePerWorker_x = width/numOfWorker_x;
	size_t sizePerWorker_y = height/numOfWorker_y;
	//copy(array, temp);
	
	//blub = array;
	//array = temp;
	//temp = blub;

	for (int wx = 0; wx < numOfWorker_x; wx++)
	{
		for (int wy = 0; wy < numOfWorker_y; wy++)
		{
			mWorker.emplace_back(std::thread([=]() {
				int count = 0;

				//std::cout << "wx:" << wx << " wy:" << wy << std::endl;

				for(int j = wy*sizePerWorker_y; j < (wy+1)*sizePerWorker_y; j++) {
					for(int i = wx*sizePerWorker_x; i < (wx+1)*sizePerWorker_x; i++) {					
						count = 
							((array[(conworld((j-1), height) * (width)) + conworld((i  ),(width))] != 0) ? 1 : 0) + 
							((array[(conworld((j-1), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j  ), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j+1), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j+1), height) * (width)) + conworld((i  ),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j+1), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j  ), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0) +
							((array[(conworld((j-1), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0);


						int idx2D = (conworld((j), height) * (width)) + conworld((i),(width));
						
						if(count < 2  || count > 3) temp[idx2D] = 0;
						//else 
						//if(count == 2) temp[idx2D] = array[idx2D];
						//else if(count ==3) temp[idx2D] = array[idx2D] + 1;
						//else temp[idx2D] = 0;
						 						 // The cell dies.
						if(count == 2 || array[idx2D] == 0xFF) {temp[idx2D] = array[idx2D];} // The cell stays the same.
						if(count == 3) {temp[idx2D] = array[idx2D]+1;} 		// The cell either stays alive, or is "born".
												
					} 
				}
			}));
		}
	}

	uint8_t * blub = temp;
	temp = array;
	array = blub;

	for (auto& th : mWorker) th.join();
	//copy(temp, array);
}

void life(uint8_t * array, uint8_t * temp, char choice)
{
  //Copies the main array to a temp array so changes can be entered into a grid
  //without effecting the other cells and the calculations being performed on them.

  copy(array, temp);
	for(int j = 0; j < height; j++) {
 		for(int i = 0; i < width; i++) {	
 			int count = 0;
 		
			if(choice == 'm')	{
        		//The Moore neighborhood checks all 8 cells surrounding the current cell in the array.
				count = 
					((array[(conworld((j-1), height) * (width)) + conworld((i  ),(width))] != 0) ? 1 : 0) + 
					((array[(conworld((j-1), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j  ), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j+1), height) * (width)) + conworld((i-1),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j+1), height) * (width)) + conworld((i  ),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j+1), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j  ), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0) +
					((array[(conworld((j-1), height) * (width)) + conworld((i+1),(width))] != 0) ? 1 : 0);
									
			} else if(choice == 'v') {
				//The Von Neumann neighborhood checks only the 4 surrounding cells in the array,
				//(N, S, E, and W).
				count = 
					((array[((j-1)%height)*width+(i  )%width]>=1)?1:0) +
				  	((array[((j  )%height)*width+(i-1)%width]>=1)?1:0) +
				  	((array[((j+1)%height)*width+(i  )%width]>=1)?1:0) +
				  	((array[((j  )%height)*width+(i+1)%width]>=1)?1:0);
			}
			
			

			int idx2D = (conworld((j), height) * (width)) + conworld((i),(width));
			// The cell dies.
			
			if(count < 2 || count > 3) {
				temp[idx2D] = 0;
			}

			// The cell stays the same.
			if(count == 2 || array[idx2D] == 0xFF) {
				temp[idx2D] = array[idx2D];
			}

			// The cell either stays alive, or is "born".
			if(count == 3) {
				temp[idx2D] = array[idx2D]+1;//array[j][i] + 1;					
			}

			//temp[idx2D] *= 32;
			//cout << (int)temp[idx2D] << " ";

		}


 	}
  //Copies the completed temp array back to the main array.
  copy(temp, array);
}


//Checks to see if two arrays are exactly the same. 
//This is used to end the simulation early, if it 
//becomes stable before the 100th generation. This
//occurs fairly often in the Von Neumann neighborhood,
//but almost never in the Moore neighborhood.
bool compare(uint8_t * array1, uint8_t * array2) {
	int count = 0;
	for(int j = 0; j < height; j++) {
		for(int i = 0; i < width; i++) {
			if(array1[j*width+i]==array2[j*width+i])
				count++;	
		}
	}
  //Since the count gets incremented every time the cells are exactly the same,
  //an easy way to check if the two arrays are equal is to compare the count to 
  //the dimensions of the array multiplied together.
	if(count == height*width)
	  return true;
	else
	  return false;
}

 bool matIsEqual(const cv::Mat mat1, const cv::Mat mat2){
    // treat two empty mat as identical as well
    if (mat1.empty() && mat2.empty()) {
        return true;
    }
    // if dimensionality of two mat is not identical, these two mat is not identical
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims) {
        return false;
    }
    cv::Mat diff;
    cv::compare(mat1, mat2, diff, cv::CMP_NE);
    int nz = cv::countNonZero(diff);
    return nz==0;
}

int saturate(int val, int min, int max) {
	val = (val<min)?min:val;
	val = (val>max)?max:val;

	return val;
}

Mat print(uint8_t * array, int zoom,int offset_x, int offset_y)
{

	Mat image(height, width, CV_8U, array); //CV_8UC3, Scalar(0,0,0));
	Mat image_color(height, width, CV_8UC3, Scalar(0,0,0));
	
	for(int j = 0; j < height; j++)	{
 		for(int i = 0; i < width; i++) {	
 			int r = 0;int g = 0;int b = 0;

			int k = array[(j*width)+i];

			// 0 = red; 16 = green; 32 = blue

			if(k>=32) k=32;
			
			if 	   (k == 0x00) r = 0x00;	
			else if(k == 1) {
				g = 0x99;
			} else if(k<16) {
				r = 0xFF; //(sin((((double)k+16)*PI)/32))*0xFF; //k*32; 
			} else {
				b = 0xFF; //(sin((((double)k+48)*PI)/32))*0xFF; //k*32; 
			}
			//b = saturate(b,0,255);
			//r = saturate(r,0,255);       
				
			image_color.at<Vec3b>(j, i) = Vec3b(b,g,r);  

		}
 	}
 	

 	int size_x = width/zoom;
 	int size_y = height/zoom;
 	int center_x = ((width)/2)-offset_x;
 	int center_y = ((height)/2)-offset_y;
 	center_x = saturate(center_x,size_x/2,width-(size_x/2));
 	center_y = saturate(center_y,size_y/2,height-(size_y/2));

 	cv::Rect roi = cv::Rect(
 		center_x-(size_x/2),
 		center_y-(size_y/2),
 		size_x,
 		size_y);
	cv::Mat roiImg;
	roiImg = image_color(roi);

	//roiImg.copyTo(image_color);

 	return roiImg;
}


string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}


void SeedImage(std::string filename, uint8_t * game, bool overwrite) {
	Mat image;
    image = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    string ty =  type2str( image.type() );
	printf("Matrix: %s %dx%d \n", ty.c_str(), image.cols, image.rows );



    for (int i = 0; i < image.rows; ++i)
    {
    	for (int j = 0; j < image.cols; ++j)
    	{
			if(image.at<uint8_t>(i, j) != 0x00) {					
				game[((i+500)*width)+j] = 1;
			} else {
				if(!overwrite) game[((i+500)*width)+j] = 0;
			}
    	}
    }
}

void Seed(uint8_t * game, bool overwrite, bool drawpanets, int dist) {
	Mat mask(height, width, CV_8U, cv::Scalar(0));
	if(drawpanets) {
		circle(mask, Point(width/2,height/2),        100, 255, CV_FILLED, 8,0);
		circle(mask, Point(width/2-300,height/2+100),100, 255, CV_FILLED, 8,0);
		circle(mask, Point(width/2+150,height/2-200), 30, 255, CV_FILLED, 8,0);
		circle(mask, Point(width/2+300,height/2+100), 20, 255, CV_FILLED, 8,0);
	} else {
		int offset_x = (width/2) - (height/2);
		// diagonal lines
		
		line(mask, Point(0+offset_x,0),          Point(height+offset_x,height),   0xFF,1);
		line(mask, Point(0+offset_x,height),     Point(height+offset_x,0),        0xFF,1);
		
		
		// diagonal lines
		/*
		line(mask, Point(0,offset_x),            Point(offset_x,0),               0xFF,1);
		line(mask, Point(0,height - offset_x),   Point(offset_x,height),          0xFF,1);
		line(mask, Point(width-offset_x,0),      Point(width,offset_x),           0xFF,1);
		line(mask, Point(width-offset_x,height), Point(width,height - offset_x),  0xFF,1);
		
		*/
		
		// horizontial lines
		line(mask, Point(width/2, 0),            Point(width/2, height),          0xFF,1);
		line(mask, Point(0,height/2),            Point(width, height/2),          0xFF,1);
		
		/*
		for (unsigned int i = 0; i < height; i += 100)
		{
			line(mask, Point(0,i),            Point(width, i),          0xFF,1);
		}
		*/
	}

	
	srand(time(NULL));

	int x0 = 0;//(width /2) - 200;
	int xN = 0;//(width /2) - 200;
	int y0 = 0;//(height/2) - 200;
	int yN = 0;//(height/2) - 200;

	for(int j = y0; j < (height-yN); j++) {
		for (int i = x0; i < (width-xN); i++) {
			//cout << (int)mask.at<uint8_t>(j, i) << endl;

			if(mask.at<uint8_t>(j, i) == 0xFF) {					
				if(drawpanets) {
					game[(j*width)+i] = ((rand()%dist)==1)?1:0;	
				} else {
					game[(j*width)+i] = 1;
				}				
			} else {
				if(!overwrite) game[(j*width)+i] = 0;
				
			}

		}
	}
}
