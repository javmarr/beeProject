#include "opencv2/objdetect.hpp"
//#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
using namespace cv;


string type2str(int type) {
	string r;

	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);

	switch (depth) {
	case CV_8U:  r = "8U"; break;
	case CV_8S:  r = "8S"; break;
	case CV_16U: r = "16U"; break;
	case CV_16S: r = "16S"; break;
	case CV_32S: r = "32S"; break;
	case CV_32F: r = "32F"; break;
	case CV_64F: r = "64F"; break;
	default:     r = "User"; break;
	}

	r += "C";
	r += (chans + '0');

	return r;
}


int main(void)
{

	//Mat mat = imread("neg-img1093.jpg", IMREAD_GRAYSCALE);
	//mat.convertTo(mat, CV_32F);
	//vector<float> arrayimage((float*)mat.data, (float*)mat.data + mat.rows * mat.cols);

	//string pos_name = "images/pos/pos-img";
	string pos_name = "../data/bees/300px/vertical-pos-";

	Mat reduced_images;
	Mat subtracted_matrix;
	Mat temp; Mat temp2;
	Mat images;
	Mat mat;

	char c;
	int counter = 0;
	string filename;


	///store the images into a matrix 
	for (; counter < 9 /*294*/; counter++)
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

	//pca analysis


	PCA pca(images, Mat(), PCA::DATA_AS_ROW);

	double eigensum = 0;
	Mat eigen = pca.eigenvalues;
	double eigsum = cv::sum(pca.eigenvalues)[0];



	int length = eigen.rows;
	float thresh = 0;
	float sum = 0;
	int k95 = 0;

	cout << eigsum << endl;
	cout << length << endl;


	for (int i = 0; i < length; i++)
	{
		sum = sum + eigen.at<float>(i, 0);
		//cout << sum << endl;
		thresh = sum / eigsum;
		cout << "threshold: " << thresh << endl;
		if (thresh > 0.95)
		{
			k95 = i;
			break;
		}
	}

	c = (char)waitKey(0);
	if (c == 27) {}
	//system("PAUSE");


	//cout << pca.mean << endl;
	//cout << pca.eigenvalues << endl;

	cout << "-----" << endl << endl;
	//cout << pca.mean.row(0) << endl;


	///get the mean of the matrix in reduced form
	reduce(images, reduced_images, 0, CV_REDUCE_AVG, -1);
	cout << "-----" << endl << endl;
	//cout << reduced_images.row(0) << endl;

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

	//Mat C = (Mat_<double>(3, 4) <<	1, 11, 7, 1,
	//								2, 5, 5, 2,
	//								3, 2, 9, 4);
	//Mat C_trans = C.t();
	//cout << " TRANSPOSED " << endl << C_trans << endl << endl;
	//Mat reduced; 
	//Mat ConC;
	//
	//cout << "C = " << endl << " " << C << endl << endl;
	//reduce(C, reduced, 0, CV_REDUCE_AVG, -1);
	//////cout << "reduced = " << endl << " " << reduced << endl << endl;
	//
	//calcCovarMatrix(C, ConC, reduced, COVAR_NORMAL| COVAR_ROWS);
	//ConC = ConC / (C.rows - 1);
	//cout << "ConC = " << endl << " " << ConC << endl << endl;
	//cout << "reduced = " << endl << " " << reduced << endl << endl;

	//Mat eiVal, eiVect;
	//eigen(ConC, eiVal, eiVect);
	//cout << "values" << endl;
	//cout << eiVal << endl;
	//cout << "vector" << endl;
	//cout << eiVect << endl;

	//cout << endl;
	//cout << "Using PCA"  << endl;
	//PCA testingPCA(C, Mat(), PCA::DATA_AS_ROW);
	//cout << "values" << endl;
	//cout << testingPCA.eigenvalues << endl;
	//cout << "vector" << endl;
	//cout << testingPCA.eigenvectors << endl;
	
	/*Mat ans = testingPCA.eigenvectors * C.t();
	cout << endl << "possible ans" << endl << ans << endl;
	*/
/*	
	Mat Covar;	
	cout << "depth" << reduced.depth() << endl;
	calcCovarMatrix(subtracted_matrix,Covar, reduced, COVAR_SCRAMBLED | COVAR_ROWS);
	Covar = Covar / (Covar.rows - 1);
	PCA pca();
	
	
	c = (char)waitKey(0);
	if (c == 27) {}

*/

	
	system("PAUSE");
	return 0;
}