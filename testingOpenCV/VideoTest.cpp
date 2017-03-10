#include "ClassifierTraining.h"
#include "croppingTool.h"
#include <iostream>

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

	}




	cin >> input;
	return 0;
}