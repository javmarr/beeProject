#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


void showImageFromVectorRow(Mat images, int rowIndex, int height) {
	Mat temp = images.row(rowIndex).clone();
	imshow("Image" + to_string(rowIndex), temp.reshape(0, height));
	char c = (char)waitKey(200);
}


int main(void)
{

	//Mat mat = imread("neg-img1093.jpg", IMREAD_GRAYSCALE);
	//mat.convertTo(mat, CV_32F);
	//vector<float> arrayimage((float*)mat.data, (float*)mat.data + mat.rows * mat.cols);

	Mat images;

	string pos_name = "images/pos/vertical-pos-img";
	string neg_name = "images/neg/neg-img";
	Mat mat;
	int counter = 0;
	string filename;
	Mat temp;
	char c;
	int imageHeight;

	//for (; counter < counter < 5 /*294*/; counter++)
	//{
	//	filename = pos_name + to_string(counter) + ".jpg";
	//	mat = imread(filename, IMREAD_GRAYSCALE);
	//	mat.convertTo(mat, CV_32F);
	//	vector<float> arrayimage((float*)mat.data, (float*)mat.data + mat.rows * mat.cols);
	//	images.push_back(arrayimage);
	//}


	///store the images into a matrix 
	for (; counter < 100; counter++)
	{
		filename = pos_name + to_string(counter) + ".jpg";

		mat = imread(filename, IMREAD_GRAYSCALE);
		if (mat.empty())
		{
			cout << "empty image" << endl;
			cout << filename;
		}
		else
		{
			images.push_back(mat.reshape(0, 1));
		}

		//showImageFromVectorRow(images, counter, mat.rows);
		
	}

	//imshow("Image_test", images.row(1).clone().reshape(0, mat.rows));


	///get the mean of the matrix in reduced form
	Mat reduced_images;
	reduce(images, reduced_images, 0, CV_REDUCE_AVG, -1);


	//temp = reduced_images;
	//imshow("reduced", temp.reshape(0, mat.rows));
	//c = (char)waitKey(200);
	//if (c == 27) { }


	///subtracted mean from matrix and store in new matrix
	Mat subtracted_matrix;
	Mat temp2;
	for (int i = 0; i < counter; i++)
	{
		subtract(images.row(i), reduced_images, temp2);
		subtracted_matrix.push_back(temp2);

		//temp = subtracted_matrix.row(i).clone();
		//imshow("Image_subtract" + to_string(i), temp.reshape(0, mat.rows));
		//c = (char)waitKey(200);
		//if (c == 27) { break; }
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





	///multiplying the subtracted matrix with eigenvectors
	Mat Eigencolumns = eigenVecTrans.col(0);
	Mat normalizedSub;

	counter = 0;
	for (; counter < 100; counter++)
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
	filename = "images/neg/neg-img149.jpg";

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

	int min = -1;
	int position = -1;
	float current_value = 0;

	for (int i = 0; i < multi.rows; i++)
	{
		current_value = abs(multi.at<float>(i,0) - Test.at<float>(0, 0));
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

	if (position < 100)
	{
		temp = subtracted_matrix.row(position).clone();
		imshow("found image", temp.reshape(0, mat.rows));
	}
	else
	{
		temp = subtracted_matrix.row(position).clone();
		imshow("found image", temp.reshape(0, mat.rows));
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