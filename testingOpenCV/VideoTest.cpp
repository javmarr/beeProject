#include "ClassifierTraining.h"
#include "croppingTool.h"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{

	VideoCapture capture;
	Mat frame;
	Train();

	capture.open("20170215192927.mp4");
	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}
		(frame);	
	}


	


	string input;
	cin >> input;
	return 0;
}