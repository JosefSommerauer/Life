#ifndef LIFE_H
#define LIFE_H 

#define PI 3.14159265

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


static int width = 1920 / 1; ///2; //800;
static int height = 1200 / 1;//1200; ///2;// 00;




//Copies one array to another.
void copy(uint8_t * array1, uint8_t * array2);


int conworld(int val, int max) ;

//int getIdx2D(int j, int i, int height, int width);

void life(uint8_t * array, uint8_t * temp, char choice);

void life_multi(uint8_t * & array, uint8_t * & temp, char choice);

bool compare(uint8_t * array1, uint8_t * array2);

bool matIsEqual(const cv::Mat mat1, const cv::Mat mat2);

int saturate(int val, int min, int max);



cv::Mat print(uint8_t * array, int zoom, int offset_x, int offset_y);
void Seed(uint8_t * game, bool overwrite, bool drawpanets, bool drawimage, bool drawcross, int dist);
void SeedImage(std::string filename, uint8_t * game, bool overwrite);

#endif
