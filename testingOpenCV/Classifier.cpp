#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;


int main(void)
{

	//Mat mat = imread("neg-img1093.jpg", IMREAD_GRAYSCALE);
	//mat.convertTo(mat, CV_32F);
	//vector<float> arrayimage((float*)mat.data, (float*)mat.data + mat.rows * mat.cols);

	//string pos_name = "images/pos/pos-img";
	string pos_name = "../data/bees/vertical-pos-";

	Mat reduced_images;
	Mat subtracted_matrix;
	Mat temp; Mat temp2;
	Mat Covar;
	Mat images;
	Mat mat;

	char c;
	int counter = 0;
	string filename;


	///store the images into a matrix 
	for (; counter < 5 /*294*/; counter++)
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

		//temp = images.row(counter).clone();
		//imshow("Image" + to_string(counter), temp.reshape(0, mat.rows));
		//c = (char)waitKey(200);
		//if (c == 27) { break; }
	}

	//imshow("Image_test", images.row(1).clone().reshape(0, mat.rows));


	///get the mean of the matrix in reduced form
	reduce(images, reduced_images, 0, CV_REDUCE_AVG, -1);
	//temp = reduced_images;
	//imshow("reduced", temp.reshape(0, mat.rows));
	//c = (char)waitKey(0);
	//if (c == 27) { }




	///subtracted mean from matrix and store in new matrix
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



	///Covariance


	// covariance test

	Mat C = (Mat_<double>(3, 4) << 1, 11, 7, 1, 2, 5, 5, 2, 3, 2, 9, 4);
	cout << "C = " << endl << " " << C << endl << endl;
	Mat reduced;
	cout << "reduced = " << endl << " " << reduced << endl << endl;
	Mat ConC;
	calcCovarMatrix(C, ConC, reduced, COVAR_ROWS);
	ConC = ConC / (C.rows - 1);
	cout << "ConC = " << endl << " " << ConC << endl << endl;
	cout << "reduced = " << endl << " " << reduced << endl << endl;


	Mat other;
	calcCovarMatrix(subtracted_matrix, Covar, other, COVAR_ROWS);
	Covar = Covar / (subtracted_matrix.rows - 1);


	c = (char)waitKey(0);
	if (c == 27) {}



	system("PAUSE");
	return 0;
}