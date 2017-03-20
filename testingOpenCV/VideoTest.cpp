#include "ClassifierTraining.h"
#include "croppingTool.h"
#include <iostream>
#include <opencv2/video.hpp>

using namespace std;
using namespace cv;

int main()
{
	string input;
	VideoCapture capture;
	Mat frame;
	Train();

	/*frame = imread("images/test/neg/test-neg-img15.jpg");
	cout << isBee(frame) << endl;*/

	//runTest();





	/*Mat prev_frame;
	Mat sub_frame;
	Vec3b pixelll;
	Ptr <BackgroundSubtractor> BGS;
	BGS = createBackgroundSubtractorMOG2();
*/



	char c;
	//capture.open("images/full video.mp4");
	capture.open("images/14s-20s.mp4");
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n");cin >> input; return -1; }
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			cin >> input;
			break;
		}
		imshow("video", DetectInFrame(frame));
		c  = waitKey(1);
		

	//	if (prev_frame.empty())
	//	{

	//	}
	//	else
	//	{
	//		sub_frame = frame - prev_frame;

	//		///Background subtract attempt
	//		//BGS->apply(frame, sub_frame);



	//		///Manual subtract attempt
	//		//sub_frame.convertTo(sub_frame, CV_32FC1);
	//		//imshow("frame", frame);
	//		//imshow("prev_frame", prev_frame);
	//		//cout << sub_frame << endl;
	//		//for (int i = 0; i <= sub_frame.rows; i++)
	//		//{
	//		//	for (int j = 0; j < sub_frame.cols; j++)
	//		//	{
	//		//		//pixelll = frame.at<Vec3b>(Point(j, i));
	//		//		//cout << pixelll << endl;
	//		//		if (frame.at<uchar>(i,j) > 0)
	//		//		{
	//		//			cout << j << endl;
	//		//			frame.at<uchar>(i, j) = 255;
	//		//		}
	//		//	}
	//		//}

	//		imshow("video", sub_frame);
	//		c = waitKey(1);
	//	}
	//	prev_frame = frame.clone();



	}
	cin >> input;
	return 0;
}