//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//int main(int argc, char* argv[]) {
//	// open video file
//	VideoCapture cap("../data/megamind.avi");
//
//	if (!cap.isOpened()) {
//		cout << "Cannot open the video file" << endl;
//		return -1;
//	}
//
//	// get video fps
//	double fps = cap.get(CV_CAP_PROP_FPS);
//	cout << "Frames per second" << fps << endl;
//
//	// autosized window
//	namedWindow("Sample Video", CV_WINDOW_AUTOSIZE);
//
//	while (true) {
//		Mat frame;
//		
//		// read new frame from video
//		bool bSuccess = cap.read(frame);
//
//		if (!bSuccess) {
//			cout << "Cannot read the frame from video file" << endl;
//			break;
//		} 
//		// show the frame in the namedWindow
//		imshow("Sample Video", frame);
//		if (waitKey(30) == 27) {
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//	return 0;
//}