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
	//runTest();
	char c;
	capture.open("images/20170215192927.mp4");
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
		c  = waitKey(0);

	}




	cin >> input;
	return 0;
}