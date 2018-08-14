//A very simple C++ implementation of John Conway's Game of Life.
//This implementation uses several nested for loops as well as two-dimensional
//arrays to create a grid for the cells in the simulation to interact.
//The array that is displayed to the user is 0 x 100, but actual size
//of the array is height x width.  The reason for this is to make the 
//calculations easier for the cells on the outermost "frame" of the grid.

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <cstdlib>
#include <climits>
#include <stdint.h>

#include <thread>

#include <X11/Xlib.h>

#include "life.h"

using namespace cv;
using namespace std;


int main() {
	// * gen0 =   new int[(height*width)];
	uint8_t * game =   new uint8_t[(height*width)];
	//int * backup = new int[(height*width)];
	uint8_t * temp =   new uint8_t[(height*width)];
	char neighborhood;
	char again;
	char cont = 'y';
	bool comparison;
	string decoration;

   //Instructions on how the program is used, along with the rules of the game.
	cout << endl << "This program is a C++ implementation of John Conway's Game of Life."
	     << endl << "With it, you can simulate how \"cells\" interact with each other." << endl 
	     << endl << "There are two types of neighborhoods you can choose, the"
	     << endl << "Moore, and the Von Neumann.  The Moore neighborhood checks"
		 << endl << "all 8 surrounding cells, whereas the Von Neumann checks"
		 << endl << "only the 4 cardinal directions: (N, S, E, and W)." << endl
		 << endl << "The rules of the \"Game of Life\" are as follows:" << endl
		 << endl << "1. Any live cell with fewer than two live neighbors dies, as if caused by under-population."
		 << endl << "2. Any live cell with two or three live neighbors lives on to the next generation."
		 << endl << "3. Any live cell with more than three live neighbors dies, as if by overcrowding."
		 << endl << "4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction." 
		 << endl << endl;

  //namedWindow( "game", CV_WINDOW_AUTOSIZE); // | WINDOW_FREERATIO );
  namedWindow( "game", WINDOW_NORMAL  ); // Create a window for display.	
  setWindowProperty("game", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
  
  neighborhood = 'm';
  static int zoom = 1;
  static int offset_x = 0;
  static int offset_y = 0;


  bool exit = false;
  bool resetWorld = true;
  int dist = 2;
  bool overwrite = false;
  bool drawpanets = false; //true;
  double time = 0;
  double time_print = 0;
  double time_life_start = 0;
  TickMeter tmLife;
  TickMeter tmPrint;
  Mat game_zoom, game_resize;
  bool isValid = false;

  std::thread mGUI([&]() {
  	while(!exit) {
  		if(isValid) imshow( "game", game_resize);  
  		char k = waitKey(1) & 0xFF; // Wait for a keystroke in the window
			
			switch(k) {
				case 'r': 	
					resetWorld = true;
					break;
				case 'm': 	
					resetWorld = true;
					neighborhood = 'm';
					break;
				case 'v': 	
					resetWorld = true;
					neighborhood = 'v';
					break;
				case 'q':
					destroyAllWindows(); 
					resetWorld = true;	
					exit = true;
				case '1':
					dist--;
					if(dist == 0) dist = 1; 
					resetWorld = true;	
					break;
				case '2':
					dist++;
					resetWorld = true;	
					break;
				case '4': 
					zoom++;
					break;
				case '3':
					zoom--; zoom = (zoom<1)?1:zoom;
					break;
				case 'o':
					overwrite = true;
					resetWorld = true;
					break;
				case 'x':
					drawpanets = false;
					break;
				case 'p':
					drawpanets = true;
					break;

				// zoom navigate
				case 'd':
					offset_x -= 10;
					break;
				case 'a':
					offset_x += 10;
					break;
				case 'w':
					offset_y += 10;
					break;
				case 's':
					offset_y -= 10;
					break;
			}
		}

  });

	
	/* windows
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	*/
	


	Display* disp = XOpenDisplay(NULL);
	Screen*  scrn = DefaultScreenOfDisplay(disp);
	int scrheight = scrn->height;
	int scrwidth  = scrn->width;
	
  	Size size(scrwidth,scrheight);//the dst image size,e.g.100x100

	do {	

		Seed(game, overwrite, drawpanets, dist);
		//SeedImage("../img/ship.png", game, overwrite);
		overwrite = false;
		resetWorld = false;
		int i = 0; // generation

		do {		
			//cout << "Generation " << i << "sin:" << sin(i) << " cos:" << cos(i) << endl;

			//if(i == 0) {copy(gen0, game);}

			//copy(game, backup);	
			tmPrint.reset();
			tmPrint.start();
			isValid = false;	
			game_zoom = print(game, zoom, offset_x, offset_y);


			resize(game_zoom,game_resize,size,0,0,cv::INTER_NEAREST);//resize image


			isValid = true;	
			tmPrint.stop();
			
			double frame_rate = ((double) getTickCount() - time) / getTickFrequency();
			std::stringstream ss; ss.precision(3);
			ss  << "gen: " << i
			    << " FPS: " << 1/frame_rate 
				<< " print: " << tmPrint.getTimeMilli() << " ms"
				<< " life: " << tmLife.getTimeMilli() << " ms";
			cv::Point pos(50,65); 
			
			cv::putText(game_resize, ss.str(), pos, FONT_HERSHEY_PLAIN, 1.2, Scalar(128, 255, 255));
			
			tmLife.reset(); 
			tmLife.start();	
			life_multi(game, temp, neighborhood);
			tmLife.stop();
			time = (double) getTickCount();	
			i++;

			// static world
			//comparison = compare(game, backup);
			}while(comparison == false && !resetWorld);
		}while(!exit);

		mGUI.join();
		
		//delete [] gen0;
		delete [] game;
		//delete [] backup;
		delete [] temp;
		
		return 0;
}

