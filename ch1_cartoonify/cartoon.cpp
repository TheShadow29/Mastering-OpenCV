#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void cartoonify_image(Mat srcColor, Mat dst, bool sketch_mode, bool evil_mode, bool alien_mode)
{
	Mat gray;
	Mat srcGray;
	cvtColor (srcColor, gray, CV_BGR2GRAY);
	cvtColor (srcColor, srcGray, CV_BGR2GRAY);
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	medianBlur (gray, gray, MEDIAN_BLUR_FILTER_SIZE);
	Mat edges;
	Mat mask;
	
	if (!evil_mode)
	{
		const int LAPLACIAN_FILTER_SIZE = 5;
		Laplacian (gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

		//since laplacian filter produces edges with varying brightness, so to make the edges 
		//look more like a sketch, we apply binary threshold to make the edges white or black
		
		const int EDGES_THRESHOLD = 80;
		threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
	}
	else
	{
		Mat edges2;
		Scharr (srcGray, edges, CV_8U, 1, 0);
		Scharr (srcGray, edges2, CV_8U, 1, 0, -1);
		edges += edges2; //combine x & y
		const int EVIL_THRESHOLD = 30;
		threshold(edges, mask, EVIL_THRESHOLD, 255, THRESH_BINARY_INV);
		medianBlur(mask, mask, 3);
	}



	//bilateral filtering at a lower resolution
	Size size = srcColor.size();
	Size small_size;
	small_size.width = size.width/4;
	small_size.height = size.height/4;

	Mat small_img = Mat(small_size, CV_8UC3);
	resize(srcColor, small_img, small_size, 0, 0, INTER_LINEAR);

	//Rather than applying a large bilateral filter, we will apply many small bilateral filters
	//to produce a strong cartoon effect in less time
	Mat temp = Mat (small_size, CV_8UC3);
	int repeat_n = 14;
	for (int i = 0; i < repeat_n; i++)
	{
		int ksize = 9; //filter size
		double sigma_color = 9; //filter color strength
		double sigma_space = 7; //spatial strength affects speed

		bilateralFilter(small_img, temp, ksize, sigma_color, sigma_space);
		bilateralFilter(temp, small_img, ksize, sigma_color, sigma_space);
	}

	//need to convert it to bigger image
	Mat big_img;
	resize(small_img, big_img, size, 0, 0, INTER_LINEAR);
	dst.setTo(0);
	big_img.copyTo(dst, mask); //need to copy to only at those points where mask is non-zero

	// if (alien_mode)
	// {
	// 	Mat faceOutline = Mat::zeros(size, CV_8UC3);
	// 	Scalar color = CV_RGB(255,255,0); //yellow color
	// 	int thickness = 4;
	// 	int faceH = size.height/2 * .7; //radius of the ellipse
	// 	int faceW = faceH * .72;
	// 	ellipse (faceOutline, Point(size.width/2, size.height/2), Size(faceW, faceH), 
	// 				0, 0, 360, color, thickness, CV_AA);

	// 	//making the eyes as two truncated ellispse
	// 	int eyeW = faceW * .23;
	// 	int eyeH = faceH * .11;
	// 	int eyeX = faceW * .48;
	// 	int eyeY = faceW * .13;

	// 	Size eyeSize = Size (eyeW, eyeH);
	// 	// Set the angle and shift for the eye half ellipses.
	// 	int eyeA = 15; //angle in degrees
	// 	int eyeYshift = 11;
	// 	//top of right eye;
	// 	ellipse (faceOutline, Point(size.width/2 - eyeX, size.height/2 - eyeY), eyeSize, 
	// 				0, 180 + eyeA, 360 - eyeA, color, thickness, CV_AA);
	// 	//bottom of right eye
	// 	ellipse (faceOutline, Point(size.width/2 - eyeX, size.height/2 - eyeY - eyeYshift), eyeSize, 
	// 				0, 0 + eyeA, 180 - eyeA, color, thickness, CV_AA);	

	// 	//top fo left eye
	// 	ellipse(faceOutline, Point(size.width/2 + eyeX, size.height/2 - eyeY), eyeSize, 
	// 				0, 180+eyeA, 360-eyeA, color, thickness, CV_AA);
	// 	//bottom of the left eye.
	// 	ellipse(faceOutline, Point(size.width/2 + eyeX, size.height/2 - eyeY - eyeYshift), eyeSize, 
	// 				0, 0+eyeA, 180-eyeA, color, thickness, CV_AA);

	// 	// Draw anti-aliased text.
	// 	int fontFace = FONT_HERSHEY_COMPLEX;
	// 	float fontScale = 1.0f;
	// 	int fontThickness = 2;
	// 	char szMsg[100] = "Put your face here";
	// 	putText(faceOutline, szMsg, Point(size.width * 23/100, size.height * 10/100),
	// 	fontFace, fontScale, color, fontThickness, CV_AA);

	// 	addWeighted (dst, 1.0, faceOutline, 0.7, 0, dst, CV_8UC3);

	// 	//skin color change doesn't work well in RGB, because we allow brightness to change, but not
	// 	//the skin color. HSV pains that it wraps around the red color.
	// 	//
		
	// 	Mat yuv = Mat(small_size, CV_8UC3);
	// 	cvtColor(small_img, CVBGR2YCrCb);

	// }
	if (sketch_mode)
	{
		cvtColor(mask, dst, CV_GRAY2BGR);
		return;
	}


}