#ifndef Cropping
#define Cropping
#define _CRT_SECURE_NO_DEPRECATE
#include "croppingTool.h"
#include "ClassifierTraining.h"


#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

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


class Scanloop : public ParallelLoopBody
{
public:
	Scanloop(Rect slidingWindow, Mat &croppedImage, Mat &cleanFrame, int scale, Mat &resized_frame, int shiftXBy, vector<Rect>& srcRects)
		: slidingWindow_l(slidingWindow), croppedImage_l(croppedImage), cleanFrame_l(cleanFrame), scale_l(scale), resized_frame_l(resized_frame), shiftXBy_l(shiftXBy), srcRects_l(srcRects)
	{ 
	}

	void operator()(const Range& range) const override
	{
		Mat croppedclone;
		Rect slidingwindowclone = slidingWindow_l;
		Rect scaled_sliding;
		for (int col = range.start; slidingwindowclone.x + slidingwindowclone.width <= range.end; col++)
		{
			//cout << slidingWindow << endl;

			
			// get smaller image from original frame "clean" version (no rectangle)
			croppedclone = cleanFrame_l(slidingwindowclone);

			/*imshow("cropped", croppedImage);
			c = waitKey(0);*/

			// check if there is a bee in the cropped area
			if (isBee(croppedclone))
			{
				// move sliding window  to the right
				scaled_sliding = Rect(slidingwindowclone.x * scale_l, slidingwindowclone.y * scale_l, slidingwindowclone.width * scale_l, slidingwindowclone.height * scale_l);

				// save the rectangles 
				srcRects_l.push_back(scaled_sliding);

				//rectangle(frame, scaled_sliding, Scalar(255, 0, 0), 1, 8, 0);
				//box_counter++;
			}
			slidingwindowclone.x += shiftXBy_l;
		}
	}



private:
	Rect slidingWindow_l;
	Mat croppedImage_l;
	Mat cleanFrame_l;
	int scale_l;
	Mat resized_frame_l;
	int shiftXBy_l;
	vector<Rect>& srcRects_l;
};


void nms(const vector<Rect>& srcRects,vector<cv::Rect>& resRects,float thresh)
{
	resRects.clear();

	const size_t size = srcRects.size();
	if (!size)
	{
		return;
	}

	// Sort the bounding boxes by the bottom - right y - coordinate of the bounding box
	std::multimap<int, size_t> idxs;
	for (size_t i = 0; i < size; ++i)
	{
		idxs.insert(std::pair<int, size_t>(srcRects[i].br().y, i));
	}

	// keep looping while some indexes still remain in the indexes list
	while (idxs.size() > 0)
	{
		// grab the last rectangle
		auto lastElem = --std::end(idxs);
		const cv::Rect& rect1 = srcRects[lastElem->second];

		resRects.push_back(rect1);

		idxs.erase(lastElem);

		for (auto pos = std::begin(idxs); pos != std::end(idxs); )
		{
			// grab the current rectangle
			const cv::Rect& rect2 = srcRects[pos->second];

			float intArea = (rect1 & rect2).area();
			float unionArea = rect1.area() + rect2.area() - intArea;
			float overlap = intArea / unionArea;

			// if there is sufficient overlap, suppress the current bounding box
			if (overlap > thresh)
			{
				pos = idxs.erase(pos);
			}
			else
			{
				++pos;
			}
		}
	}
}




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
	int shiftXBy = slidingWindow.width/10; int shiftYBy = slidingWindow.height/10;

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
	vector<cv::Rect> srcRects;
	Size size(0, 0);
	vector<cv::Rect> resRects;

	int scale = 3;

	int slidefactor = 4;
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


	Mat resized_frame(cvRound(frame.rows / scale), cvRound(frame.cols / scale), CV_32FC1);
	resize(frame, resized_frame, resized_frame.size());

	cleanFrame = resized_frame.clone();

	Rect slidingWindow = Rect(0, 0, getImageHeight(), getImageHeight());
	
	int box_counter = 0;
	// how much to much the windows (x and y)
	int shiftXBy = slidingWindow.width / slidefactor; int shiftYBy = slidingWindow.height / slidefactor;

	for (int row = 0; slidingWindow.y + slidingWindow.height <= resized_frame.size().height; row++)
	{
		///attempt at parallizing 
		Scanloop parallelScan(slidingWindow,croppedImage, cleanFrame, scale, resized_frame, shiftXBy, srcRects);
		parallel_for_(Range( 0, resized_frame.size().width ), parallelScan);




		///previous loop
		////cout << "row" << endl;
		//for (int col = 0; slidingWindow.x + slidingWindow.width <= resized_frame.size().width; col++)
		//{
		//	//cout << slidingWindow << endl;

		//	// get smaller image from original frame "clean" version (no rectangle)
		//	croppedImage = cleanFrame(slidingWindow);

		//	/*imshow("cropped", croppedImage);
		//	c = waitKey(0);*/

		//	// check if there is a bee in the cropped area
		//	if (isBee(croppedImage))
		//	{	
		//		// move sliding window  to the right
		//		Rect scaled_sliding = Rect(slidingWindow.x * scale, slidingWindow.y * scale, slidingWindow.width * scale, slidingWindow.height * scale);

		//		// save the rectangles 
		//		srcRects.push_back(scaled_sliding);

		//		//rectangle(frame, scaled_sliding, Scalar(255, 0, 0), 1, 8, 0);
		//		//box_counter++;
		//	}
		//	slidingWindow.x += shiftXBy;
		//}


		// reset x for new row
		slidingWindow.x = 0;
		// move sliding window  to the right
		slidingWindow.y += shiftYBy;
	}

	// perform non-maximum suppression (reduce rect)
	nms(srcRects, resRects, 0.1f);
	
	// draw the rectangles
	for (auto r : resRects)
	{
		rectangle(frame, r, Scalar(0, 255, 0), 2);
		box_counter++;
	}



	// display rect number
    // total (green)
	putText(frame, to_string(box_counter), cvPoint(30, 150), FONT_HERSHEY_SIMPLEX, 6, cvScalar(0, 255, 0), 5, CV_AA);
    // entering (blue)
    putText(frame, to_string(box_counter), cvPoint(150, 150), FONT_HERSHEY_SIMPLEX, 6, cvScalar(255, 0, 0), 5, CV_AA);
    // leaving (red)
    putText(frame, to_string(box_counter), cvPoint(270, 150), FONT_HERSHEY_SIMPLEX, 6, cvScalar(0, 0, 255), 5, CV_AA);
	//cout << box_counter << endl;
	return frame;
}


#endif
