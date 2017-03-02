//#include "opencv2/objdetect.hpp"
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc.hpp"
//#include <iostream>
//
//using namespace std;
//using namespace cv;
//
//static void help()
//{
//    cout << "\nThis program demonstrates the cascade recognizer. Now you can use Haar or LBP features.\n"
//            "This classifier can recognize many kinds of rigid objects, once the appropriate classifier is trained.\n"
//            "It's most known use is for faces.\n"
//            "Usage:\n"
//            "./facedetect [--cascade=<cascade_path> this is the primary trained classifier such as frontal face]\n"
//               "   [--nested-cascade[=nested_cascade_path this an optional secondary classifier such as eyes]]\n"
//               "   [--scale=<image scale greater or equal to 1, try 1.3 for example>]\n"
//               "   [--try-flip]\n"
//               "   [filename|camera_index]\n\n"
//            "see facedetect.cmd for one call:\n"
//            "./facedetect --cascade=\"../../data/haarcascades/haarcascade_frontalface_alt.xml\" --nested-cascade=\"../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml\" --scale=1.3\n\n"
//            "During execution:\n\tHit any key to quit.\n"
//            "\tUsing OpenCV version " << CV_VERSION << "\n" << endl;
//}
//
//void detectAndDraw( Mat& img, CascadeClassifier& cascade,
//                    CascadeClassifier& nestedCascade,
//                    double scale, bool tryflip );
//
//string cascadeName;
//string nestedCascadeName;
//
//int main( int argc, const char** argv )
//{
//    VideoCapture capture;
//    Mat frame, image;
//    string inputName;
//    bool tryflip;
//    CascadeClassifier cascade, nestedCascade;
//    double scale;
//
//    cv::CommandLineParser parser(argc, argv,
//        "{help h||}"
//        "{cascade|../../data/haarcascades/haarcascade_frontalface_alt.xml|}"
//        "{nested-cascade|../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
//        "{scale|1|}{try-flip||}{@filename||}"
//    );
//    if (parser.has("help"))
//    {
//        help();
//        return 0;
//    }
//    //cascadeName = parser.get<string>("cascade");
//	/*cascadeName = "../data/xml/haarcascades/haarcascade_frontalface_alt2.xml";*/
//	cascadeName = "../data/xml/face_cascade_20_stage_ALL.xml";
//
//    nestedCascadeName = parser.get<string>("nested-cascade");
//    scale = parser.get<double>("scale");
//    if (scale < 1)
//        scale = 1;
//    tryflip = parser.has("try-flip");
//    //inputName = parser.get<string>("@filename");
//	inputName = "../data/lena.jpg";
//    if (!parser.check())
//    {
//        parser.printErrors();
//        return 0;
//    }
//    if ( !nestedCascade.load( nestedCascadeName ) )
//        cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
//    if( !cascade.load( cascadeName ) )
//    {
//        cerr << "ERROR: Could not load classifier cascade" << endl;
//        help();
//        return -1;
//    }
//    if( inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1) )
//    {
//        int camera = inputName.empty() ? 0 : inputName[0] - '0';
//        if(!capture.open(camera))
//            cout << "Capture from camera #" <<  camera << " didn't work" << endl;
//    }
//    else if( inputName.size() )
//    {
//        image = imread( inputName, 1 );
//        if( image.empty() )
//        {
//            if(!capture.open( inputName ))
//                cout << "Could not read " << inputName << endl;
//        }
//    }
//    else
//    {
//        image = imread( "../data/lena.jpg", 1 );
//        if(image.empty()) cout << "Couldn't read ../data/lena.jpg" << endl;
//    }
//
//    if( capture.isOpened() )
//    {
//        cout << "Video capturing has been started ..." << endl;
//
//        for(;;)
//        {
//            capture >> frame;
//            if( frame.empty() )
//                break;
//
//            Mat frame1 = frame.clone();
//            detectAndDraw( frame1, cascade, nestedCascade, scale, tryflip );
//
//            char c = (char)waitKey(10);
//            if( c == 27 || c == 'q' || c == 'Q' )
//                break;
//        }
//    }
//    else
//    {
//        cout << "Detecting face(s) in " << inputName << endl;
//        if( !image.empty() )
//        {
//            detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
//            waitKey(0);
//        }
//        else if( !inputName.empty() )
//        {
//            /* assume it is a text file containing the
//            list of the image filenames to be processed - one per line */
//            FILE* f = fopen( inputName.c_str(), "rt" );
//            if( f )
//            {
//                char buf[1000+1];
//                while( fgets( buf, 1000, f ) )
//                {
//                    int len = (int)strlen(buf);
//                    while( len > 0 && isspace(buf[len-1]) )
//                        len--;
//                    buf[len] = '\0';
//                    cout << "file " << buf << endl;
//                    image = imread( buf, 1 );
//                    if( !image.empty() )
//                    {
//                        detectAndDraw( image, cascade, nestedCascade, scale, tryflip );
//                        char c = (char)waitKey(0);
//                        if( c == 27 || c == 'q' || c == 'Q' )
//                            break;
//                    }
//                    else
//                    {
//                        cerr << "Aw snap, couldn't read image " << buf << endl;
//                    }
//                }
//                fclose(f);
//            }
//        }
//    }
//
//    return 0;
//}
//
//void detectAndDraw( Mat& img, CascadeClassifier& cascade,
//                    CascadeClassifier& nestedCascade,
//                    double scale, bool tryflip )
//{
//    double t = 0;
//    vector<Rect> faces, faces2;
//    const static Scalar colors[] =
//    {
//        Scalar(255,0,0),
//        Scalar(255,128,0),
//        Scalar(255,255,0),
//        Scalar(0,255,0),
//        Scalar(0,128,255),
//        Scalar(0,255,255),
//        Scalar(0,0,255),
//        Scalar(255,0,255)
//    };
//    Mat gray, smallImg;
//
//    cvtColor( img, gray, COLOR_BGR2GRAY );
//    double fx = 1 / scale;
//    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
//    equalizeHist( smallImg, smallImg );
//
//    t = (double)getTickCount();
//    cascade.detectMultiScale( smallImg, faces,
//        1.1, 2, 0
//        //|CASCADE_FIND_BIGGEST_OBJECT
//        //|CASCADE_DO_ROUGH_SEARCH
//        |CASCADE_SCALE_IMAGE,
//        Size(30, 30) );
//    if( tryflip )
//    {
//        flip(smallImg, smallImg, 1);
//        cascade.detectMultiScale( smallImg, faces2,
//                                 1.1, 2, 0
//                                 //|CASCADE_FIND_BIGGEST_OBJECT
//                                 //|CASCADE_DO_ROUGH_SEARCH
//                                 |CASCADE_SCALE_IMAGE,
//                                 Size(30, 30) );
//        for( vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r )
//        {
//            faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
//        }
//    }
//    t = (double)getTickCount() - t;
//    printf( "detection time = %g ms\n", t*1000/getTickFrequency());
//    for ( size_t i = 0; i < faces.size(); i++ )
//    {
//        Rect r = faces[i];
//        Mat smallImgROI;
//        vector<Rect> nestedObjects;
//        Point center;
//        Scalar color = colors[i%8];
//        int radius;
//
//        double aspect_ratio = (double)r.width/r.height;
//        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
//        {
//            center.x = cvRound((r.x + r.width*0.5)*scale);
//            center.y = cvRound((r.y + r.height*0.5)*scale);
//            radius = cvRound((r.width + r.height)*0.25*scale);
//            circle( img, center, radius, color, 3, 8, 0 );
//        }
//        else
//            rectangle( img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
//                       cvPoint(cvRound((r.x + r.width-1)*scale), cvRound((r.y + r.height-1)*scale)),
//                       color, 3, 8, 0);
//        if( nestedCascade.empty() )
//            continue;
//        smallImgROI = smallImg( r );
//        nestedCascade.detectMultiScale( smallImgROI, nestedObjects,
//            1.1, 2, 0
//            //|CASCADE_FIND_BIGGEST_OBJECT
//            //|CASCADE_DO_ROUGH_SEARCH
//            //|CASCADE_DO_CANNY_PRUNING
//            |CASCADE_SCALE_IMAGE,
//            Size(30, 30) );
//        for ( size_t j = 0; j < nestedObjects.size(); j++ )
//        {
//            Rect nr = nestedObjects[j];
//            center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
//            center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
//            radius = cvRound((nr.width + nr.height)*0.25*scale);
//            circle( img, center, radius, color, 3, 8, 0 );
//        }
//    }
//    imshow( "result", img );
//}
////namespace
////#include "opencv2/objdetect.hpp"
//////#include "opencv2/videoio.hpp"
////#include "opencv2/highgui.hpp"
////#include "opencv2/imgproc.hpp"
////#include <iostream>
////#include <stdio.h>
////using // std;
////using namespace cv;
////
////void detectAndDisplay(Mat frame);
////void cropImageAndSave(Mat frame);
////
////CascadeClassifier car_cascade;
////String window_name = "Capture - detection";
////String car_cascade_name = "../data/xml/myfacedetector.xml";
////
////int main(void)
////{
////	VideoCapture capture;
////	Mat frame;
////
////	//-- 1. Load the cascades
////	if (!car_cascade.load(car_cascade_name)) { printf("--(!)Error loading car cascade\n"); return -1; };
////
////	//-- 2. Read the video stream
////	//capture.open("../data/Bee.mp4");
////	capture.open("../data/a8.bmp");
////	if (!capture.isOpened()) { printf("--(!)Error opening video capture\n"); return -1; }
////	while (capture.read(frame))
////	{
////		if (frame.empty())
////		{
////			printf(" --(!) No captured frame -- Break!");
////			break;
////		}
////		//-- 3. Apply the classifier to the frame
////
////
////		detectAndDisplay(frame);
////
////		char c = (char)waitKey(0);
////		if (c == 27) { break; } // escape
////
////
////		//cropImageAndSave(frame);
////
////	}
////	return 0;
////}
////
////
/////*
////* Crops and saved a portion of an image, user can choose pos or negative
////* The folders: images/pos and images/neg have to exist for this to work
////
////* Keys:
////esc = escape
////q = quit (basically the same as esc)
////f = fail = negative
////g = good = pos
////s = skip (doesn't save it)
////*/
////void cropImageAndSave(Mat frame) {
////	Mat cleanFrame = frame.clone();
////	Mat croppedImage;
////	String cropped_window = "Cropped image";
////	String filename = "pos/image-";
////	string input = ""; // std string for input
////	Rect slidingWindow = Rect(0, 0, 192, 108);
////	bool saved = false;
////	bool forceQuit = false;
////
////	// how much to much the windows (x and y)
////	int posCount = 0, negCount = 0;
////	int shiftXBy = slidingWindow.width; int shiftYBy = slidingWindow.height;
////
////	for (int row = 0; slidingWindow.y + slidingWindow.height <= frame.size().height && !forceQuit; row++)
////	{
////		cout << "row" << endl;
////		for (int col = 0; slidingWindow.x + slidingWindow.width <= frame.size().width && !forceQuit; col++)
////		{
////			cout << slidingWindow << endl;
////
////			// get smaller image from original frame "clean" version (no rectangle)
////			croppedImage = cleanFrame(slidingWindow);
////
////			// show for user to confirm and label
////			imshow(cropped_window, croppedImage);
////
////			// show rectangles on image
////			if (col % 2 == row % 2) {
////				rectangle(frame, slidingWindow, Scalar(255, 0, 0), 1, 8, 0);
////			}
////			else {
////				rectangle(frame, slidingWindow, Scalar(0, 255, 0), 1, 8, 0);
////			}
////
////			imshow(window_name, frame);
////
////			char c = (char)waitKey(0);
////			if (c == 27) { forceQuit = true; break; } // escape
////
////			while (!saved) {
////				if (c == 'q') {
////					saved = true;
////					forceQuit = true;
////				}
////				else if (c == 'g') {
////					cout << "positive" << endl;
////					imwrite("images/pos/pos-img" + std::to_string(posCount) + ".jpg", croppedImage);
////					posCount++;
////					saved = true;
////				}
////				else if (c == 'f') {
////					cout << "negative" << endl;
////					imwrite("images/neg/neg-img" + std::to_string(negCount) + ".jpg", croppedImage);
////					negCount++;
////					saved = true;
////				}
////				else if (c == 's') {
////					cout << "skipping" << endl;
////					saved = true;
////				}
////			}
////
////			// move sliding window  to the right
////			slidingWindow.x += shiftXBy;
////
////			// new cropped image to save
////			saved = false;
////		}
////		// reset x for new row
////		slidingWindow.x = 0;
////		// move sliding window  to the right
////		slidingWindow.y += shiftYBy;
////
////	}
////}
////
////void detectAndDisplay(Mat frame)
////{
////	Mat frame_gray;
////	Mat resized_frame;
////	Mat resized_frame_gray;
////
////	std::vector<Rect> beesFound;
////
////	// useScaling == detect on small image and draw on bigger one
////	bool useScaling = false; // for detecting
////	bool drawOnOriginal = true; // for drawing
////	const int scale = 4;
////
////	// ==========================================================
////	// scale down image and find objects ========================
////	if (useScaling) {
////		// scale down the image by factor of variable "scale"
////		resized_frame = Mat(cvRound(frame.rows / scale), cvRound(frame.cols / scale), CV_8UC1);
////		resize(frame, resized_frame, resized_frame.size());
////
////		// turn to grayscale
////		cvtColor(resized_frame, resized_frame_gray, COLOR_BGR2GRAY);
////		equalizeHist(resized_frame_gray, resized_frame_gray);
////
////		//-- Detect beesFound on scaled image
////		car_cascade.detectMultiScale(resized_frame_gray, beesFound, 1.5, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
////	}
////	else {
////		// turn to grayscale
////		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
////		equalizeHist(frame_gray, frame_gray);
////
////		// no scaling
////		//car_cascade.detectMultiScale(frame_gray, beesFound, 1.5, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
////
////		// original values
////		car_cascade.detectMultiScale(frame_gray, beesFound, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
////	}
////
////	// ==========================================================
////	// draw on the image to show the items ======================
////
////	for (size_t i = 0; i < beesFound.size(); i++)
////	{
////		// print values for objects found
////		cout << "x : " << beesFound[i].x << " | y : " << beesFound[i].y << " | width : " << beesFound[i].width << " | height : " << beesFound[i].height << endl;
////
////		// draw on non-scaled image
////		if (drawOnOriginal) {
////			// draw rectangle
////			rectangle(frame, Rect((beesFound[i].x*scale), (beesFound[i].y*scale), (beesFound[i].width*scale), (beesFound[i].height*scale)), Scalar(0, 245, 0), 1 * scale, 8, 0);
////		}
////		else {
////			// draw rectangle
////			rectangle(resized_frame, Rect(beesFound[i].x, beesFound[i].y, beesFound[i].width, beesFound[i].height), Scalar(0, 245, 0), 1, 8, 0);
////		}
////	}
////
////	//-- Show what you got
////	if (drawOnOriginal)
////		cv::imshow(window_name, frame);
////	else
////		cv::imshow(window_name, resized_frame);
////}