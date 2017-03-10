#ifndef Cropping
#define Cropping
#include "croppingTool.h"
#include "ClassifierTraining.h"


#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);
void cropImageAndSave(Mat frame);

CascadeClassifier car_cascade;
String window_name = "Capture - detection";
//String car_cascade_name = "../data/xml/cascade-bee.xml";
int counter;
int posCount;
int negCount;


void cropImageAndSave(Mat frame) {
	Mat cleanFrame = frame.clone();
	Mat croppedImage;
	String cropped_window = "Cropped image";
	String filename = "pos/image-";
	string input = ""; // std string for input
	Rect slidingWindow = Rect(0, 0, 300, 300);
	bool saved = false;
	bool forceQuit = false;

	// how much to much the windows (x and y)
	int shiftXBy = slidingWindow.width/4; int shiftYBy = slidingWindow.height/4;

	for (int row = 0; slidingWindow.y + slidingWindow.height <= frame.size().height && !forceQuit; row++)
	{
		cout << "row" << endl;
		for (int col = 0; slidingWindow.x + slidingWindow.width <= frame.size().width && !forceQuit; col++)
		{
			cout << slidingWindow << endl;

			// get smaller image from original frame "clean" version (no rectangle)
			croppedImage = cleanFrame(slidingWindow);

			// show for user to confirm and label
			imshow(cropped_window, croppedImage);

			// show rectangles on image
			if (col % 2 == row % 2) {
				rectangle(frame, slidingWindow, Scalar(255, 0, 0), 1, 8, 0);
			}
			else {
				rectangle(frame, slidingWindow, Scalar(0, 255, 0), 1, 8, 0);
			}

			imshow(window_name, frame);

			char c = (char)waitKey(0);
			if (c == 27) { forceQuit = true; break; } // escape

			while (!saved) {
				if (c == 'q') {
					saved = true;
					forceQuit = true;
				}
				else if (c == 'g') {
					cout << "positive" << endl;
					imwrite("images/pos/pos-img" + std::to_string(posCount) + ".jpg", croppedImage);
					posCount++;
					saved = true;
				}
				else if (c == 'f') {
					cout << "negative" << endl;
					imwrite("images/neg/neg-img" + std::to_string(negCount) + ".jpg", croppedImage);
					negCount++;
					saved = true;
				}
				else if (c == 's') {
					cout << "skipping" << endl;
					saved = true;
				}
				else
				{
					c = (char)waitKey(0);
					if (c == 27) { forceQuit = true; break; }
				}
			}

			// move sliding window  to the right
			slidingWindow.x += shiftXBy;

			// new cropped image to save
			saved = false;
		}
		// reset x for new row
		slidingWindow.x = 0;
		// move sliding window  to the right
		slidingWindow.y += shiftYBy;

	}
}

void detectAndDisplay(Mat frame)
{
	Mat frame_gray;
	Mat resized_frame;
	Mat resized_frame_gray;

	std::vector<Rect> beesFound;

	// useScaling == detect on small image and draw on bigger one
	bool useScaling = true; // for detecting
	bool drawOnOriginal = true; // for drawing
	const int scale = 4;

	// ==========================================================
	// scale down image and find objects ========================
	if (useScaling) {
		// scale down the image by factor of variable "scale"
		resized_frame = Mat(cvRound(frame.rows / scale), cvRound(frame.cols / scale), CV_8UC1);
		resize(frame, resized_frame, resized_frame.size());

		// turn to grayscale
		cvtColor(resized_frame, resized_frame_gray, COLOR_BGR2GRAY);
		equalizeHist(resized_frame_gray, resized_frame_gray);

		//-- Detect beesFound on scaled image
		car_cascade.detectMultiScale(resized_frame_gray, beesFound, 1.5, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	}
	else {
		// turn to grayscale
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		// no scaling
		car_cascade.detectMultiScale(frame_gray, beesFound, 1.5, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		// original values
		//face_cascade.detectMultiScale(frame_gray, beesFound, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
	}

	// ==========================================================
	// draw on the image to show the items ======================

	for (size_t i = 0; i < beesFound.size(); i++)
	{
		// print values for objects found
		cout << "x : " << beesFound[i].x << " | y : " << beesFound[i].y << " | width : " << beesFound[i].width << " | height : " << beesFound[i].height << endl;

		// draw on non-scaled image
		if (drawOnOriginal) {
			// draw rectangle
			rectangle(frame, Rect((beesFound[i].x*scale), (beesFound[i].y*scale), (beesFound[i].width*scale), (beesFound[i].height*scale)), Scalar(0, 245, 0), 1 * scale, 8, 0);
		}
		else {
			// draw rectangle
			rectangle(resized_frame, Rect(beesFound[i].x, beesFound[i].y, beesFound[i].width, beesFound[i].height), Scalar(0, 245, 0), 1, 8, 0);
		}
	}

	//-- Show what you got
	if (drawOnOriginal)
		cv::imshow(window_name, frame);
	else
		cv::imshow(window_name, resized_frame);
}

Mat DetectInFrame(Mat frame)
{
	Mat cleanFrame = frame.clone();
	Mat croppedImage;

	char c;
	//String cropped_window = "Cropped image";
	//String filename = "pos/image-";
	//string input = ""; // std string for input
	//bool saved = false;
	//bool forceQuit = false;

	/*imshow("clean frame", cleanFrame);
	c = waitKey(0);*/

	Rect slidingWindow = Rect(0, 0, getImageHeight(), getImageHeight());
	

	// how much to much the windows (x and y)
	int shiftXBy = slidingWindow.width / 4; int shiftYBy = slidingWindow.height / 4;

	for (int row = 0; slidingWindow.y + slidingWindow.height <= frame.size().height; row++)
	{
		//cout << "row" << endl;
		for (int col = 0; slidingWindow.x + slidingWindow.width <= frame.size().width; col++)
		{
			//cout << slidingWindow << endl;

			// get smaller image from original frame "clean" version (no rectangle)


			croppedImage = cleanFrame(slidingWindow);

			/*imshow("cropped", croppedImage);
			c = waitKey(0);*/


			// move sliding window  to the right
			if (isBee(croppedImage))
			{
				rectangle(frame, slidingWindow, Scalar(255, 0, 0), 1, 8, 0);
			}
			slidingWindow.x += shiftXBy;
		}


		// reset x for new row
		slidingWindow.x = 0;
		// move sliding window  to the right
		slidingWindow.y += shiftYBy;
	}
	return frame;
}


#endif