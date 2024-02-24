#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Mat img = imread("newimg.jpeg"); //Read image
	Mat imgGray, imgBlur, edges, imgready, mask; Mat copy; //Define variables
	resize(img, img, Size(307.2,409.6 ), INTER_LINEAR); //resize image to 307x409

	imshow("original image", img); //Show original resized image

	img.copyTo(copy);  //make a copy of img
	int canny = 30; //set canny treshold number

	cvtColor(img, imgGray, COLOR_BGR2GRAY); //convert image to grayscale

	GaussianBlur(imgGray, imgBlur, Size(17, 17), 0); //Gaussian blur 

	Canny(imgBlur, edges, canny, (3 * canny)); //Canny edge detection

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3)); // Dilate and erode
	dilate(edges, edges, kernel);
	erode(edges, edges, kernel);

	vector<vector<Point>> contours; //create vector "contours"
	vector<Vec4i>hierarchy; //create vector hierarchy

	findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);  //Find contours, external (only the largest in a group), keep it simple (save only important points)
	cout << "NUMBER OF COINS:  " << contours.size() << endl;  //Print contours sise = number of coins

	int twenty = 0, ten = 0, half = 0; // set number of coins to zero

	for (int i = 0; i < contours.size(); i++) // Iterate through every coin
	{
		if (contourArea(contours[i]) > 3600 && contourArea(contours[i]) < 4900) // check areas and decide what coin is what
		{
			drawContours(img, contours, i, Scalar(255, 0, 0), 3);  //draw contours
			twenty++;     //count +1 twenty coin
		}
		else if (contourArea(contours[i]) > 2900 && contourArea(contours[i]) < 3500)
		{
			drawContours(img, contours, i, Scalar(0, 255, 0), 3); //draw contours
			ten++;     //count + 1 ten coin
		}
		else if (contourArea(contours[i]) > 2300 && contourArea(contours[i]) < 2700)
		{
			drawContours(img, contours, i, Scalar(0, 0, 255), 3); //draw contours
			half++;     // count + 1 0.5 coin
		}

	}
	
	//Print number of coins
	cout << " 20: " << twenty << endl;
	cout << " 10: " <<  ten << endl;
	cout << " 0.5: " << half << endl;
	//Create string - sum of the coins
	string message = ("The sum:" + to_string(int(twenty * 20 + ten * 10 + half * 0.5)) + "DKK");
	//Put sum of the coins into picture
	putText(img, message, Point(10, 400), FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255), 2);

	imshow("result", img); //Show result picture
	waitKey(0);
	return 0;
}