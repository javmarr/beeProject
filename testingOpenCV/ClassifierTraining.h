#pragma once
#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

void showImageFromVector(Mat image, int height);

void showImageFromVectorRow(Mat images, int rowIndex, int height);

bool isBee(Mat test_image);

double runTest();

int getImageHeight();

int Train();