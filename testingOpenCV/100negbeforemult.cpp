#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


void showImageFromVector(Mat image, int height) {
	Mat temp = image.clone();
	imshow("Image", temp.reshape(0, height));
	char c = (char)waitKey(200);
}

void showImageFromVectorRow(Mat images, int rowIndex, int height) {
	Mat temp = images.row(rowIndex).clone();
	imshow("Image" + to_string(rowIndex), temp.reshape(0, height));
	char c = (char)waitKey(200);
}


int main(void)
{
	Mat images;

	string pos_name = "images/pos/vertical-pos-img";
	string neg_name = "images/neg/neg-img";
	Mat mat; // used as temp storage when reading images
	Mat temp;
	string filename;
	char c;

	int imageHeight;
	int counter = 0;
	int numPos = 100; // number of positive images
	int numNeg = 100; // number of negatives images

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
		//cout << thresh << endl;
		if (thresh > 0.95)
		{
			k95 = i;
			break;
		}
	}

	cout << "Threshold 95: vectors - " << k95 << endl;


	///multiplying the subtracted matrix with eigenvectors
	//Mat Eigencolumns = eigenVecTrans.col(0);
	
	Mat Eigenrows;
	Mat Eigencolumns;

	for (int i = 0; i <= k95; i++)
	{
		Eigenrows.push_back(eigenVec.row(i));
	}

	Eigencolumns = Eigenrows.t();

	Mat normalizedSub;

	for (counter = 0; counter < numNeg; counter++)
	{
		filename = neg_name + to_string(counter) + ".jpg";

		mat = imread(filename, IMREAD_GRAYSCALE);
		if (mat.empty())
		{
			cout << "empty image" << endl;
			cout << filename;
		}
		else
		{
			subtract(mat.reshape(0, 1), reduced_images, temp2);
			subtracted_matrix.push_back(temp2);
		}

		//temp = images.row(counter).clone();
		//imshow("Image" + to_string(counter), temp.reshape(0, mat.rows));
		//c = (char)waitKey(200);
		//if (c == 27) { break; }
	}

	//normalize(subtracted_matrix, normalizedSub, 0, 255, NORM_MINMAX, CV_32FC1);
	subtracted_matrix.convertTo(normalizedSub, CV_32FC1);

	Mat multi = normalizedSub * Eigencolumns;
	
	///get the test image
	Mat test_image;
	//filename = "images/pos/pos-img258.jpg";
	filename = "images/test/neg-img108.jpg";

	mat = imread(filename, IMREAD_GRAYSCALE);

	if (mat.empty())
	{
		cout << "empty image" << endl;
		cout << filename;
	}
	else
	{
		test_image.push_back(mat.reshape(0, 1));
	}

	///subtract mean from test image
	Mat subtracted_test;

	subtract(test_image.row(0), reduced_images, temp2);
	subtracted_test.push_back(temp2);

	///multiple subtracted test data with eigen vector
	Mat normalizedTest;
	subtracted_test.convertTo(normalizedTest, CV_32FC1);
	Mat Test = normalizedTest * Eigencolumns;


	///get Euclidean distance of test image and data

	float min = -1;
	int position = -1;
	float current_value = 0;

	//double dist = norm(a, b, NORM_L2);

	for (int i = 0; i < multi.rows; i++)
	{
		current_value = norm(multi.row(i), Test.row(0));  //abs(multi.at<float>(i,0) - Test.at<float>(0, 0));
		if (min == -1)
		{
			min = current_value;
			position = i;
		}
		else
		{
			if (current_value < min)
			{
				min = current_value;
				position = i;
			}
		}
	}


	cout << "min: " << min << endl;
	cout << "position: " << position << endl;

	if (position < numPos)
	{
		temp = subtracted_matrix.row(position).clone();
		imshow("positive", temp.reshape(0, mat.rows));
	}
	else
	{
		temp = subtracted_matrix.row(position).clone();
		imshow("negative", temp.reshape(0, mat.rows));
	}
	
	imshow("test image", mat);

	c = (char)waitKey(0);
	if (c == 27) { }
	
	//c = (char)waitKey(0);
	//if (c == 27) { }
	//system("PAUSE");
	//cout << "ll" << endl;
	//mat.release();
	//reduced_images.release();
	//subtracted_matrix.release();
	//Covar.release();

	return 0;
}