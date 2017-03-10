#ifndef ClassifierTraining
#define ClassifierTraining
#include "ClassifierTraining.h"
#include "croppingTool.h"

#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat Global_multi;
Mat Global_eigencols;
Mat Global_reduced;

int imageHeight;
int numPos = 100; // number of positive images
int numNeg = 300; // number of negatives images

String posTestName = "images/test/pos/test-pos-img";
String negTestName = "images/test/neg/test-neg-img";

void showImageFromVector(Mat image, int height) {
	Mat temp = image.clone();
	imshow("Image", temp.reshape(0, height));
	char c = (char)waitKey(200);
}

int getImageHeight()
{
	return imageHeight;
}

void showImageFromVectorRow(Mat images, int rowIndex, int height) {
	Mat temp = images.row(rowIndex).clone();
	imshow("Image" + to_string(rowIndex), temp.reshape(0, height));
	char c = (char)waitKey(200);
}

bool isBee(Mat passed_image) {

	//grey scale and reshape the image
	Mat test_image;
	cvtColor(passed_image, test_image, cv::COLOR_BGR2GRAY);
	test_image = test_image.reshape(0, 1);

	// subtract mean from test image
	Mat outMat;
	Mat subtracted_test;
	subtract(test_image.row(0), Global_reduced, outMat);
	subtracted_test.push_back(outMat);

	// multiply subtracted test data with eigen vector
	Mat normalizedTest;
	Mat Test;
	subtracted_test.convertTo(normalizedTest, CV_32FC1);
	Test = normalizedTest * Global_eigencols;

	// get Euclidean distance of test image and data
	float min = -1;
	int position = -1;
	float current_value = 0;

	// double dist = norm(a, b, NORM_L2);
	for (int i = 0; i < Global_multi.rows; i++)
	{
		current_value = norm(Global_multi.row(i), Test.row(0));  //abs(multi.at<float>(i,0) - Test.at<float>(0, 0));
		if (min == -1) {
			min = current_value;
			position = i;
		}
		else if (current_value < min) {
			min = current_value;
			position = i;
		}
	}

	// results for image
	//cout << "min: " << min << endl;
	//cout << "position: " << position << endl;

	/*
	positioon < numPos | positive
	position >= numPos | negative
	*/
	if (position < numPos) {
		//temp = subtracted_matrix.row(position).clone();
		//imshow("positive", temp.reshape(0, imageHeight));
		//cout << "positive" << endl;
		//cout << position << endl;
		return true;
	}
	else {
		//temp = subtracted_matrix.row(position).clone();
		//imshow("negative", temp.reshape(0, imageHeight));
		//cout << "negative" << endl;
		return false;
	}

	return false;
}

double runTest() {

	Mat mat;
	Mat test_image;
	String filename;

	int posTestNum = 280; // 0-279
	int negTestNum = 63; // 0-62
	int numTested = posTestNum + negTestNum;
	double correct = 0;
	double accuracy = 0;

	// loop through each pos test (0->posNum-1)
	for (int i = 0; i < posTestNum; i++) {
		filename = posTestName + to_string(i) + ".jpg";
		//cout << "reading file: " << filename << endl;
		// get the test image
		Mat mat = imread(filename, IMREAD_GRAYSCALE);

		// check for empty image
		if (mat.empty()) {
			cout << "empty image" << endl;
		}
		else {
			test_image = mat.reshape(0, 1);

			if (isBee(test_image)) {
				correct++;
			}
		}
	}


	for (int i = 0; i < negTestNum; i++) {
		filename = negTestName + to_string(i) + ".jpg";
		//cout << "reading file: " << filename << endl;
		// get the test image
		Mat mat = imread(filename, IMREAD_GRAYSCALE);

		// check for empty image
		if (mat.empty()) {
			cout << "empty image" << endl;
		}
		else {
			test_image = mat.reshape(0, 1);

			// negatives are not bees
			if (!isBee(test_image)) {
				correct++;
			}
		}

		/*
		show test image
		imshow("test image", mat);
		char c = (char)waitKey(0);
		if (c == 27) {}
		*/
	}


	accuracy = correct / numTested;
	cout << " correct: " << correct << endl;
	cout << " accuracy: " << accuracy << endl;

	return accuracy;
}

int Train()
{
	Mat images;

	//	string pos_name = "images/pos/vertical-pos-img";
	string pos_name = "images/pos/pos-img";
	string neg_name = "images/neg/neg-img";
	Mat mat; // used as temp storage when reading images
	Mat temp;
	string filename;
	char c;


	int counter = 0;

	///store the images into a matrix 
	for (; counter < numPos; counter++) {

		filename = pos_name + to_string(counter) + ".jpg";
		mat = imread(filename, IMREAD_GRAYSCALE);
		if (mat.empty()) {
			cout << "empty image" << endl;
			cout << filename;
		}
		else {
			images.push_back(mat.reshape(0, 1));
		}
		//showImageFromVectorRow(images, counter, mat.rows);
	}

	imageHeight = mat.rows;

	//imshow("Image_test", images.row(1).clone().reshape(0, mat.rows));

	///get the mean of the matrix in reduced form
	Mat reduced_images;
	reduce(images, reduced_images, 0, CV_REDUCE_AVG, -1);
	//showImageFromVector(reduced_images, mat.rows);

	///subtracted mean from matrix and store in new matrix
	Mat subtracted_matrix;
	Mat temp2;
	for (int i = 0; i < counter; i++)
	{
		subtract(images.row(i), reduced_images, temp2);
		subtracted_matrix.push_back(temp2);

		//showImageFromVectorRow(subtracted_matrix, i, imageHeight);
	}
	cout << "finished subtracting" << endl;


	///Using PCA to get eigenvalues and vectors
	PCA pca(images, Mat(), PCA::DATA_AS_ROW);
	//cout << "values" << endl; cout << pca.eigenvalues << endl; //cout << "vector" << endl; cout << pca.eigenvectors << endl;


	double eigensum = 0;
	Mat eigen = pca.eigenvalues;
	double eigsum = cv::sum(pca.eigenvalues)[0];

	Mat eigenVec = pca.eigenvectors;
	Mat eigenVecTrans = eigenVec.t();


	int length = eigen.rows;
	float thresh = 0;
	float sum = 0;
	int k95 = 0;

	//cout << eigsum << endl;
	//cout << length << endl;

	///getting how many columns to use
	for (int i = 0; i < length; i++)
	{
		sum = sum + eigen.at<float>(i, 0);
		//cout << sum << endl;
		thresh = sum / eigsum;
		//cout << "Threshold:" << thresh << endl;
		if (thresh > 0.95)
		{
			k95 = i;
			break;
		}
	}

	k95 = 9;
	cout << "Threshold. number of vectors: " << (k95 + 1) << endl;

	///multiplying the subtracted matrix with eigenvectors
	//Mat Eigencolumns = eigenVecTrans.col(0);

	Mat Eigenrows; Mat Eigencolumns;


	// push k95 rows into the eigenvectors
	for (int i = 0; i <= k95; i++) {
		Eigenrows.push_back(eigenVec.row(i));
	}

	// transform to cols (to multiply)
	Eigencolumns = Eigenrows.t();

	Mat normalizedSub;

	// push the negative images into the subtracted matrix
	for (counter = 0; counter < numNeg; counter++) {

		// read neg image
		filename = neg_name + to_string(counter) + ".jpg";
		mat = imread(filename, IMREAD_GRAYSCALE);
		if (mat.empty()) {
			cout << "empty image" << endl;
			cout << filename;
		}
		else {
			// subtract the mean from the negative image before adding
			subtract(mat.reshape(0, 1), reduced_images, temp2);
			subtracted_matrix.push_back(temp2);
		}

		//temp = images.row(counter).clone();
		//imshow("Image" + to_string(counter), temp.reshape(0, mat.rows));
		//c = (char)waitKey(200);
		//if (c == 27) { break; }
	}

	//normalize(subtracted_matrix, normalizedSub, 0, 255, NORM_MINMAX, CV_32FC1);

	// change format from CV_8U to CV_32FC1
	subtracted_matrix.convertTo(normalizedSub, CV_32FC1);

	// reduced features received from finding the product
	Mat multi = normalizedSub * Eigencolumns;

	Global_multi = multi;
	Global_eigencols = Eigencolumns;
	Global_reduced = reduced_images;

	return 0;
}
#endif // !ClassifierTraining