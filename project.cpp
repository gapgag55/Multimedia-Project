#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

//Forward Seam Carving
int main()
{
	//Display an image
	Mat img = imread("ironman.jpg", IMREAD_COLOR);
	namedWindow("Example", WINDOW_AUTOSIZE);
	imshow("Example", img);
	int c = cvWaitKey(0);
	Size sz = img.size();
	int height = sz.height;
	int width = sz.width;

	// a, b are for loop
	// min1, min2, min3 are used to find M(i,j)
	// minval is used to find minimum color value for starting point in K matrix
	// minidx is index of minval
	int a, b, min1, min2, min3;
	int cl, cu, cr;
	int minval, minidx;

	//Esc = 27, a = 97, d = 100, s = 115, 2 = 119
	while (c != 27)
	{
		Mat temp;	//for calculating CL, CU, CR
		Scalar value(0);
		Mat M(height, width, CV_8UC1, value); //set all value to 0
		Mat K(height, width, CV_8UC1, value); //set all value to 0
		minval = 256;
		//loop till get the command 'a', 'd', 'w', 's'
		while (c != 97 && c != 100 && c != 115 && c != 119 && c != 27)
		{
			c = cvWaitKey(0);
		}
		
		cvtColor(img, temp, COLOR_BGR2GRAY); //convert img to gray then store in temp
		copyMakeBorder(temp, temp, 1, 1, 1, 1, BORDER_REPLICATE); //add border to temp
		//Keyboard command ::: 'a' and 'd' => vertical best seam, 'w' and 's' => horizontal best seam
		if (c == 97 || c == 100) //finished
		{
			//Construct M matrix and K matrix in the vertical direction
			//*** WRITE YOUR CODE ***
			for (a = 1; a <= height; a++) //move along y-axis
			{
				for (b = 1; b <= width; b++) //move along x-axis
				{
					//just change fomular for cl,cu,cr to find horizontal seam
					cl = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a))) + abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b - 1, a)));
					cu = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a)));
					cr = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a))) + abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b + 1, a)));

					if (a == 1) //first row
					{
						K.at<uchar>(Point(b - 1, a - 1)) = 0;
						M.at<uchar>(Point(b - 1, a - 1)) = cu;
					}
					else if (b == 1) //first column
					{
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;
						if (min2< min3)
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min2;
							K.at<uchar>(Point(b - 1, a - 1)) = 2;
						}
						else
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min3;
							K.at<uchar>(Point(b - 1, a - 1)) = 3;
						}
					}
					else if (b == width - 1) //last column
					{
						min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						if (min1 < min2)
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min1;
							K.at<uchar>(Point(b - 1, a - 1)) = 1;
						}
						else
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min2;
							K.at<uchar>(Point(b - 1, a - 1)) = 2;
						}
					}
					else if (a < height && b < width) //prevent index out of bound
					{
						min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;

						if (min1 < min2)
						{
							if (min1 < min3)
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min1;
								K.at<uchar>(Point(b - 1, a - 1)) = 1;
							}
							else
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min3;
								K.at<uchar>(Point(b - 1, a - 1)) = 3;
							}
						}
						else
						{
							if (min2 < min3)
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min2;
								K.at<uchar>(Point(b - 1, a - 1)) = 2;
							}
							else
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min3;
								K.at<uchar>(Point(b - 1, a - 1)) = 3;
							}
						}
					}
				}
			}

			//Find the best seam in the vertical direction
			//*** WRITE YOUR CODE ***
			for (a = 0; a < width; a++) //find starting point
			{
				if (M.at<uchar>(Point(a, height - 1)) < minval)
				{
					minval = M.at<uchar>(Point(a, height - 1));
					minidx = a;
				}
			}

			for (a = height - 1; a >= 0; a--) //find best seam
			{
					img.at<Vec3b>(Point(minidx, a))[0] = 0;
					img.at<Vec3b>(Point(minidx, a))[1] = 255;
					img.at<Vec3b>(Point(minidx, a))[2] = 0;

				/*for (c = minidx; c < minidx+10; c++)
				{
					img.at<Vec3b>(Point(c, a))[0] = 255;
					img.at<Vec3b>(Point(c, a))[1] = 255;
					img.at<Vec3b>(Point(c, a))[2] = 255;
				}*/

				if (K.at<uchar>(Point(minidx, a)) == 1)
					minidx--;
				else if (K.at<uchar>(Point(minidx, a)) == 3)
					minidx++;
			}
			imshow("result", img);
			printf("finish the loop\n");
		}

		//'w' => increase height, 's' reduce height
		if (c == 115 || c == 119)
		{
			//Construct M matrix and K matrix in the horizontal direction
			//*** WRITE YOUR CODE ***
			for (a = 1; a <= height; a++) //move along y-axis
			{
				for (b = 1; b <= width; b++) //move along x-axis
				{
					//just change fomular for cl,cu,cr to find horizontal seam
					cl = abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b, a + 1))) + abs(temp.at<uchar>(Point(b - 1, a)) - temp.at<uchar>(Point(b, a + 1)));
					cu = abs(temp.at<uchar>(Point(b, a + 1)) - temp.at<uchar>(Point(b, a - 1)));
					cr = abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b, a + 1))) + abs(temp.at<uchar>(Point(b - 1, a)) - temp.at<uchar>(Point(b, a - 1)));

					if (a == 1) //first row
					{
						K.at<uchar>(Point(b - 1, a - 1)) = 0;
						M.at<uchar>(Point(b - 1, a - 1)) = cu;
					}
					else if (b == 1) //first column
					{
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;
						if (min2< min3)
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min2;
							K.at<uchar>(Point(b - 1, a - 1)) = 2;
						}
						else
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min3;
							K.at<uchar>(Point(b - 1, a - 1)) = 3;
						}
					}
					else if (b == width - 1) //last column
					{
						min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						if (min1 < min2)
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min1;
							K.at<uchar>(Point(b - 1, a - 1)) = 1;
						}
						else
						{
							M.at<uchar>(Point(b - 1, a - 1)) = min2;
							K.at<uchar>(Point(b - 1, a - 1)) = 2;
						}
					}
					else if (a < height && b < width) //prevent index out of bound
					{
						min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
						min2 = M.at<uchar>(Point(b, a - 1)) + cu;
						min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;

						if (min1 < min2)
						{
							if (min1 < min3)
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min1;
								K.at<uchar>(Point(b - 1, a - 1)) = 1;
							}
							else
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min3;
								K.at<uchar>(Point(b - 1, a - 1)) = 3;
							}
						}
						else
						{
							if (min2 < min3)
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min2;
								K.at<uchar>(Point(b - 1, a - 1)) = 2;
							}
							else
							{
								M.at<uchar>(Point(b - 1, a - 1)) = min3;
								K.at<uchar>(Point(b - 1, a - 1)) = 3;
							}
						}
					}
				}
			}
			
			//Find the best seam in the horizontal direction
			//*** WRITE YOUR CODE ***
			for (a = 0; a < height; a++) //find starting point
			{
				if (M.at<uchar>(Point(0, a)) < minval)
				{
					minval = M.at<uchar>(Point(0, a));
					minidx = a;
				}
			}

			for (a = 0; a < width; a++) //find best seam
			{
				img.at<Vec3b>(Point(a, minidx))[0] = 0;
				img.at<Vec3b>(Point(a, minidx))[1] = 255;
				img.at<Vec3b>(Point(a, minidx))[2] = 0;

				/*for (c = minidx; c < minidx+10; c++)
				{
				img.at<Vec3b>(Point(c, a))[0] = 255;
				img.at<Vec3b>(Point(c, a))[1] = 255;
				img.at<Vec3b>(Point(c, a))[2] = 255;
				}*/

				if (K.at<uchar>(Point(a, minidx)) == 1)
					minidx--;
				else if (K.at<uchar>(Point(a, minidx)) == 3)
					minidx++;
			}
			imshow("result", img);
			printf("finish the loop\n");

		}

		//Insert or delete the best seam
		/*if (c == 97)
		{
			//Reduce width or delete seam vertically
			//Copy the pixels into this image
			Mat img_new(height, --width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***
			
			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 100)
		{
			//Increase height or delete seam horizontally
			//Copy the pixels into this image
			Mat img_new(height, ++width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 115)
		{
			//Reduce width or delete seam horizontally
			//Copy the pixels into this image
			Mat img_new(--height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}
		if (c == 119)
		{
			//Increase height or delete seam vertically
			//Copy the pixels into this image
			Mat img_new(++height, width, CV_64FC3, Scalar(0, 0, 0));
			// *** WRITE YOUR CODE ***

			//Show the resized image
			imshow("Example", img_new);
			//Clone img_new into img for the next loop processing
			img.release();
			img = img_new.clone();
			img_new.release();
		}*/

		if (c == 27) break;
		c = cvWaitKey(0);
	}
	return 0;
}

/*
int main()
{
	/*Mat A = (Mat_<int>(3, 4) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 23, 45);
	cout << "Mat A\n" << A << endl << endl;
	cout << A.at<int>(Point(2, 1)) << endl;
	Mat img = imread("ironman.jpg", IMREAD_COLOR);
	Size sz = img.size();
	int height = sz.height;
	int width = sz.width;
	// a, b are for loop
	// min1, min2, min3 are used to find M(i,j)
	// minval is used to find minimum color value for starting point in K matrix
	// minidx is index of minval
	int a, b,min1,min2,min3;
	int cl, cu, cr;
	int minval = 256, minidx;

	Mat temp;	//for calculating CL, CU, CR
	Scalar value(0);
	Mat M(height, width, CV_8UC1, value);
	Mat K(height, width, CV_8UC1, value);

	cvtColor(img, temp, COLOR_BGR2GRAY);
	//imshow("gray", temp);
	copyMakeBorder(temp, temp, 1, 1, 1, 1, BORDER_REPLICATE);
	//imshow("copy border", temp);
	//for vertical forward seam carving
	for (a = 1; a <= height; a++) //move along y-axis
	{
		for (b = 1; b <= width; b++) //move along x-axis
		{
			//just change fomular for cl,cu,cr to find horizontal seam
			cl = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a))) + abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b - 1, a)));
			cu = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a)));
			cr = abs(temp.at<uchar>(Point(b + 1, a)) - temp.at<uchar>(Point(b - 1, a))) + abs(temp.at<uchar>(Point(b, a - 1)) - temp.at<uchar>(Point(b + 1, a)));

			if (a == 1) //first row
			{
				K.at<uchar>(Point(b - 1, a - 1)) = 0;
				M.at<uchar>(Point(b - 1, a - 1)) = cu;
			}
			else if (b == 1) //first column
			{
				min2 = M.at<uchar>(Point(b, a - 1)) + cu;
				min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;
				if (min2< min3)
				{
					M.at<uchar>(Point(b - 1, a - 1)) = min2;
					K.at<uchar>(Point(b - 1, a - 1)) = 2;
				}
				else
				{
					M.at<uchar>(Point(b - 1, a - 1)) = min3;
					K.at<uchar>(Point(b - 1, a - 1)) = 3;
				}
			}
			else if (b == width-1) //last column
			{
				min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
				min2 = M.at<uchar>(Point(b, a - 1)) + cu;
				if (min1 < min2)
				{
					M.at<uchar>(Point(b - 1, a - 1)) = min1;
					K.at<uchar>(Point(b - 1, a - 1)) = 1;
				}
				else
				{
					M.at<uchar>(Point(b - 1, a - 1)) = min2;
					K.at<uchar>(Point(b - 1, a - 1)) = 2;
				}
			}
			else if(a < height && b < width) //prevent index out of bound
			{
				min1 = M.at<uchar>(Point(b - 1, a - 1)) + cl;
				min2 = M.at<uchar>(Point(b, a - 1)) + cu;
				min3 = M.at<uchar>(Point(b + 1, a - 1)) + cr;

				if (min1 < min2)
				{
					if (min1 < min3)
					{
						M.at<uchar>(Point(b - 1, a - 1)) = min1;
						K.at<uchar>(Point(b - 1, a - 1)) = 1;
					}
					else
					{
						M.at<uchar>(Point(b - 1, a - 1)) = min3;
						K.at<uchar>(Point(b - 1, a - 1)) = 3;
					}
				}
				else
				{
					if (min2 < min3)
					{
						M.at<uchar>(Point(b - 1, a - 1)) = min2;
						K.at<uchar>(Point(b - 1, a - 1)) = 2;
					}
					else
					{
						M.at<uchar>(Point(b - 1, a - 1)) = min3;
						K.at<uchar>(Point(b - 1, a - 1)) = 3;
					}
				}
			}
			//printf("a = %d, b = %d, %d\n",a,b,K.at<uchar>(Point(b - 1, a - 1)));
		}
	}
	//cout << "finished";

	for (a = 0; a < width; a++) //find starting point
	{
		if (M.at<uchar>(Point(a, height - 1)) < minval)
		{
			minval = M.at<uchar>(Point(a, height - 1));
			minidx = a;
		}
	}
		
	int c; //for testing

	for (a = height - 1; a >= 0; a--) //find best seam
	{
		img.at<Vec3b>(Point(minidx, a))[0] = 0;
		img.at<Vec3b>(Point(minidx, a))[1] = 255;
		img.at<Vec3b>(Point(minidx, a))[2] = 0;

		/*for (c = minidx; c < minidx+10; c++)
		{
			img.at<Vec3b>(Point(c, a))[0] = 255;
			img.at<Vec3b>(Point(c, a))[1] = 255;
			img.at<Vec3b>(Point(c, a))[2] = 255;
		}

		if (K.at<uchar>(Point(minidx, a)) == 1)
			minidx--;
		else if (K.at<uchar>(Point(minidx, a)) == 3)
			minidx++;
	}
	imshow("finished", img);
	waitKey();
	//_getch();
	//imshow("M matrix", M);
}*/

/*int main()
{
	Mat img = imread("ironman.jpg", IMREAD_COLOR);

	//Mat img2(img, Rect(0, 0, img.cols, img.rows-1)); //reduce image height
	//Mat img2(img, Rect(0, 0, img.cols-1, img.rows)); //reduce image width
	Mat img2;
	Scalar value(0, 0, 0);
	//increase image width at the left
	//copyMakeBorder(img, img2, 0, 0, 0, 1, BORDER_REPLICATE, value);

	//increase image height at the bottom
	//copyMakeBorder(img, img2, 0, 1, 0, 0, BORDER_REPLICATE, value);
	imshow("original image", img);
	imshow("modified image", img2);
	waitKey();
}*/

/*
Mat src, dst;
int top, bottom, left, right;
int borderType = BORDER_CONSTANT;
const char* window_name = "copyMakeBorder Demo";

int main(int argc, char** argv)
{
	//const char* imageName = argc >= 2 ? argv[1] : "../data/lena.jpg";
	// Loads an image
	src = imread("ironman.jpg", IMREAD_COLOR); // Load an image
										   // Check if image is loaded fine
	if (src.empty()) {
		printf(" Error opening image\n");
		printf(" Program Arguments: [image_name -- default ../data/lena.jpg] \n");
		return -1;
	}
	// Brief how-to for this program
	printf("\n \t copyMakeBorder Demo: \n");
	printf("\t -------------------- \n");
	printf(" ** Press 'c' to set the border to a random constant value \n");
	printf(" ** Press 'r' to set the border to be replicated \n");
	printf(" ** Press 'ESC' to exit the program \n");
	namedWindow(window_name, WINDOW_AUTOSIZE);
	// Initialize arguments for the filter
	Scalar value(0, 0, 0);
	//copyMakeBorder(src, src, 60, 60, 60, 60, BORDER_CONSTANT, value);
	copyMakeBorder(src, src, 60, 60, 60, 60, BORDER_REPLICATE, value);
	imshow(window_name, src);
	waitKey();
	/*for (;;)
	{
		Scalar value(0,0,0);
		
		copyMakeBorder(src, dst, 1, 1, 1, 1, borderType, value);
		imshow(window_name, dst);
		char c = (char)waitKey(500);
		if (c == 27)
		{
			break;
		}
		else if (c == 'c')
		{
			borderType = BORDER_CONSTANT;
		}
		else if (c == 'r')
		{
			borderType = BORDER_REPLICATE;
		}
	}
	return 0;
}*/